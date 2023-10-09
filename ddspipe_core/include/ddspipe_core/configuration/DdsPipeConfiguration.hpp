// Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#pragma once

#include <map>
#include <set>

#include <cpp_utils/Formatter.hpp>

#include <ddspipe_core/configuration/IConfiguration.hpp>
#include <ddspipe_core/configuration/RoutesConfiguration.hpp>
#include <ddspipe_core/configuration/TopicRoutesConfiguration.hpp>
#include <ddspipe_core/types/participant/ParticipantId.hpp>
#include <ddspipe_core/types/topic/dds/DistributedTopic.hpp>
#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>

#include <ddspipe_core/library/library_dll.h>

namespace eprosima {
namespace ddspipe {
namespace core {

enum EntityCreationTrigger
{
    ANY = 0,        //! The discovery of both readers and writers triggers the creation of entities.
    READER = 1,     //! The discovery of readers triggers the creation of entities.
    WRITER = 2      //! The discovery of writers triggers the creation of entities.
};

/**
 * Configuration structure encapsulating the configuration of a \c DdsPipe instance.
 */
struct DdsPipeConfiguration : public IConfiguration
{
    /////////////////////////
    // CONSTRUCTORS
    /////////////////////////

    DDSPIPE_CORE_DllAPI DdsPipeConfiguration() = default;

    /////////////////////////
    // METHODS
    /////////////////////////

    /**
     * @brief Override \c is_valid method.
     */
    DDSPIPE_CORE_DllAPI
    virtual bool is_valid(
            utils::Formatter& error_msg) const noexcept override;

    /**
     * @brief Check if a configuration is valid given a list of participants.
     *
     * It calls its own \c is_valid method plus the \c is_valid method of the
     * encapsulated configurations.
     */
    DDSPIPE_CORE_DllAPI
    bool is_valid(
            utils::Formatter& error_msg,
            const std::map<types::ParticipantId, bool>& participants) const noexcept;

    /**
     * @brief Set internal values with the values reloaded.
     */
    DDSPIPE_CORE_DllAPI
    void reload(
            const DdsPipeConfiguration& new_configuration);

    /**
     * @brief Select the \c RoutesConfiguration for a topic.
     *
     * @return The route configuration for a specific topic.
     */
    DDSPIPE_CORE_DllAPI
    RoutesConfiguration get_routes_config(
            const utils::Heritable<types::DistributedTopic>& topic) const noexcept;

    /////////////////////////
    // VARIABLES
    /////////////////////////

    //! Topic lists to build the AllowedTopics
    std::set<utils::Heritable<ddspipe::core::types::IFilterTopic>> allowlist{};
    std::set<utils::Heritable<ddspipe::core::types::IFilterTopic>> blocklist{};

    //! Builtin topics to create at the beggining of the execution
    std::set<utils::Heritable<ddspipe::core::types::DistributedTopic>> builtin_topics{};

    //! Set of fixed topics' QoS
    std::vector<utils::Heritable<ddspipe::core::types::WildcardDdsFilterTopic>> manual_topics {};

    //! Configuration of the generic routes.
    RoutesConfiguration routes{};

    //! Configuration of the routes specific to a topic.
    TopicRoutesConfiguration topic_routes{};

    //! Whether entities should be removed when they have no writers connected to them.
    bool remove_unused_entities = false;

    //! Whether the DDS Pipe should be initialized enabled.
    bool init_enabled = false;
    
    //! The entity type whose discovery should trigger the creation of entities.
    EntityCreationTrigger entity_creation_trigger = EntityCreationTrigger::READER;
};

} /* namespace core */
} /* namespace ddspipe */
} /* namespace eprosima */
