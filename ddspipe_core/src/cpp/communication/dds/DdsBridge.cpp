// Copyright 2022 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cpp_utils/exception/UnsupportedException.hpp>
#include <cpp_utils/Log.hpp>

#include <ddspipe_core/communication/dds/DdsBridge.hpp>

namespace eprosima {
namespace ddspipe {
namespace core {

using namespace eprosima::ddspipe::core::types;

DdsBridge::DdsBridge(
        const utils::Heritable<DistributedTopic>& topic,
        const std::shared_ptr<ParticipantsDatabase>& participants_database,
        const std::shared_ptr<PayloadPool>& payload_pool,
        const std::shared_ptr<utils::SlotThreadPool>& thread_pool,
        const RoutesConfiguration& routes_config,
        const bool remove_unused_entities)
    : Bridge(participants_database, payload_pool, thread_pool)
    , topic_(topic)
{
    logDebug(DDSPIPE_DDSBRIDGE, "Creating DdsBridge " << *this << ".");

    routes_ = routes_config();

    if (remove_unused_entities)
    {
        // The builtin participants and some tests use an empty topic discoverer participant id
        create_writer(topic->topic_discoverer());
    }
    else
    {
        create_all_tracks_();
    }

    logDebug(DDSPIPE_DDSBRIDGE, "DdsBridge " << *this << " created.");
}

DdsBridge::~DdsBridge()
{
    logDebug(DDSPIPE_DDSBRIDGE, "Destroying DdsBridge " << *this << ".");

    // Disable every Track before destruction
    disable();

    logDebug(DDSPIPE_DDSBRIDGE, "DdsBridge " << *this << " destroyed.");
}

void DdsBridge::enable() noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (!enabled_)
    {
        logInfo(DDSPIPE_DDSBRIDGE, "Enabling DdsBridge for topic " << topic_ << ".");

        // ATTENTION: reference needed or it would copy Track
        for (auto& track_it : tracks_)
        {
            track_it.second->enable();
        }

        enabled_ = true;
    }
}

void DdsBridge::disable() noexcept
{
    std::lock_guard<std::mutex> lock(mutex_);

    if (enabled_)
    {
        logInfo(DDSPIPE_DDSBRIDGE, "Disabling DdsBridge for topic " << topic_ << ".");

        // ATTENTION: reference needed or it would copy Track
        for (auto& track_it : tracks_)
        {
            track_it.second->disable();
        }

        enabled_ = false;
    }
}

void DdsBridge::create_all_tracks_()
{
    std::lock_guard<std::mutex> lock(mutex_);

    const auto& ids = participants_->get_participants_ids();

    // Figure out what writers need to be created
    std::set<ParticipantId> writers_to_create;

    for (const ParticipantId& id : ids)
    {
        const auto& routes_it = routes_.find(id);

        if (routes_it != routes_.end())
        {
            // The reader has a route. Create only the writers in the route.

            // We are not going to modify the writers_ids in this route. We can get the writers_ids by reference.
            const auto& writers_ids = routes_it->second;
            writers_to_create.insert(writers_ids.begin(), writers_ids.end());
        }
        else
        {
            // The reader doesn't have a route. Create every writer (+ itself if repeater)
            auto writers_ids = ids;

            if (!participants_->get_participant(id)->is_repeater())
            {
                // The participant is not a repeater. Do not add its writer.
                writers_ids.erase(id);
            }

            writers_to_create.insert(writers_ids.begin(), writers_ids.end());
        }
    }

    // Create the writers.
    std::map<ParticipantId, std::shared_ptr<IWriter>> writers;

    for (const auto& id : writers_to_create)
    {
        std::shared_ptr<IParticipant> participant = participants_->get_participant(id);
        writers[id] = participant->create_writer(*topic_);
    }

    // Add the writers to the tracks they have routes for.
    add_writers_to_tracks_nts_(writers);
}

void DdsBridge::create_writer(
        const ParticipantId& participant_id)
{
    assert(participant_id != DEFAULT_PARTICIPANT_ID);

    std::lock_guard<std::mutex> lock(mutex_);

    // Create the writer.
    std::shared_ptr<IParticipant> participant = participants_->get_participant(participant_id);
    std::shared_ptr<IWriter> writer = participant->create_writer(*topic_);

    // Add the writer to the tracks it has routes for.
    add_writer_to_tracks_nts_(participant_id, writer);
}

void DdsBridge::remove_writer(
        const ParticipantId& participant_id) noexcept
{
    assert(participant_id != DEFAULT_PARTICIPANT_ID);

    std::lock_guard<std::mutex> lock(mutex_);

    for (auto it = tracks_.cbegin(), next_it = it; it != tracks_.cend(); it = next_it)
    {
        ++next_it;

        const auto& track = it->second;

        // If the writer is in the track, remove it.
        track->remove_writer(participant_id);

        if (!track->has_writers())
        {
            // The track doesn't have any writers. Remove it.
            tracks_.erase(it);
        }
    }
}

void DdsBridge::add_writer_to_tracks_nts_(
        const ParticipantId& participant_id,
        std::shared_ptr<IWriter>& writer)
{
    // Create the writer.
    std::map<ParticipantId, std::shared_ptr<IWriter>> writers;
    writers[participant_id] = writer;

    // Add the writer to the tracks it has routes for.
    add_writers_to_tracks_nts_(writers);
}

void DdsBridge::add_writers_to_tracks_nts_(
        std::map<ParticipantId, std::shared_ptr<IWriter>>& writers)
{
    // Add writers to the tracks of the readers in their route.
    // If the readers in their route don't exist, create them with their tracks.
    for (const ParticipantId& id : participants_->get_participants_ids())
    {

        // Select the necessary writers
        std::map<ParticipantId, std::shared_ptr<IWriter>> writers_of_track;

        const auto& routes_it = routes_.find(id);

        if (routes_it != routes_.end())
        {
            // The reader has a route. Add only the writers in the route.
            const auto& writers_in_route = routes_it->second;

            for (const auto& writer_id : writers_in_route)
            {
                if (writers.count(writer_id) >= 1)
                {
                    writers_of_track[writer_id] = writers[writer_id];
                }
            }
        }
        else
        {
            // The reader doesn't have a route. Add every writer (+ itself if repeater)
            writers_of_track = writers;

            if (!participants_->get_participant(id)->is_repeater())
            {
                // The participant is not a repeater. Do not add its writer.
                writers_of_track.erase(id);
            }
        }

        if (writers_of_track.size() == 0)
        {
            // There are no writers in the route. There is nothing to do. Skip participant.
            continue;
        }

        if (tracks_.count(id))
        {
            // The track already exists. Add the writers to it.
            for (const auto& writers_of_track_it : writers_of_track)
            {
                const auto& writer_id = writers_of_track_it.first;
                const auto& writer = writers_of_track_it.second;

                if (!tracks_[id]->has_writer(writer_id))
                {
                    // Add the writer to the track
                    tracks_[id]->add_writer(writer_id, writer);
                }
            }
        }
        else
        {
            // The track doesn't exist. Create it.
            std::shared_ptr<IParticipant> participant = participants_->get_participant(id);
            auto reader = participant->create_reader(*topic_);

            tracks_[id] = std::make_unique<Track>(
                topic_,
                id,
                std::move(reader),
                std::move(writers_of_track),
                payload_pool_,
                thread_pool_);

            if (enabled_)
            {
                tracks_[id]->enable();
            }
        }
    }
}

std::ostream& operator <<(
        std::ostream& os,
        const DdsBridge& bridge)
{
    os << "DdsBridge{" << bridge.topic_ << "}";
    return os;
}

} /* namespace core */
} /* namespace ddspipe */
} /* namespace eprosima */
