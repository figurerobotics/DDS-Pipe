// Copyright 2021 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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
// limitations under the License\.

#pragma once

#include <cpp_utils/types/Fuzzy.hpp>

#include <ddspipe_core/types/participant/ParticipantId.hpp>

#include <ddspipe_core/configuration/IConfiguration.hpp>
#include <ddspipe_participants/library/library_dll.h>

namespace eprosima {
namespace ddspipe {
namespace participants {

struct ParticipantConfiguration : public core::IConfiguration
{

    /////////////////////////
    // CONSTRUCTORS
    /////////////////////////

    DDSPIPE_PARTICIPANTS_DllAPI
    ParticipantConfiguration() = default;


    /////////////////////////
    // METHODS
    /////////////////////////

    DDSPIPE_PARTICIPANTS_DllAPI
    virtual bool is_valid(
            utils::Formatter& error_msg) const noexcept override;

    //! Participant Id associated with this configuration
    core::types::ParticipantId id {};

    //! Whether this Participant should connect its readers with its writers.
    bool is_repeater {false};

    //! Participant downsampling rate associated with this configuration.
    utils::Fuzzy<unsigned int> downsampling{};

    //! Participant max reception rate associated with this configuration.
    utils::Fuzzy<float> max_reception_rate{};
};

} /* namespace participants */
} /* namespace ddspipe */
} /* namespace eprosima */
