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

#pragma once

#include <set>
#include <string>

namespace eprosima {
namespace ddspipe {
namespace yaml {

// Yaml related tags
constexpr const char* VERSION_TAG("version");       //! Version to parse the yaml with
constexpr const char* VERSION_TAG_V_1_0("v1.0");    //! Version v1.0
constexpr const char* VERSION_TAG_V_2_0("v2.0");    //! Version v2.0
constexpr const char* VERSION_TAG_V_3_0("v3.0");    //! Version v3.0
constexpr const char* VERSION_TAG_V_3_1("v3.1");    //! Version v3.1

// Topics related tags
constexpr const char* ALLOWLIST_TAG("allowlist");     //! List of allowed topics
constexpr const char* BLOCKLIST_TAG("blocklist");     //! List of blocked topics
constexpr const char* BUILTIN_TAG("builtin-topics");  //! List of builtin topics
constexpr const char* TOPIC_NAME_TAG("name");         //! Name of a topic
constexpr const char* TOPIC_TYPE_NAME_TAG("type");    //! Type name of a topic
constexpr const char* TOPIC_QOS_TAG("qos");           //! QoS of a topic

// QoS related tags
constexpr const char* QOS_RELIABLE_TAG("reliability"); //! The Endpoints of that topic will be configured as RELIABLE
constexpr const char* QOS_TRANSIENT_TAG("durability"); //! The Endpoints of that topic will be configured as TRANSIENT_LOCAL
constexpr const char* QOS_HISTORY_DEPTH_TAG("depth"); //! The Endpoints of that topic will be configured as this History Depth
constexpr const char* QOS_PARTITION_TAG("partitions"); //! The Endpoints of that topic will be configured with partitions
constexpr const char* QOS_OWNERSHIP_TAG("ownership"); //! The Endpoints of that topic will be configured with partitions
constexpr const char* QOS_KEYED_TAG("keyed"); //! Kind of a topic (with or without key)
constexpr const char* QOS_DOWNSAMPLING_TAG("downsampling"); //! Topic specific downsampling factor
constexpr const char* QOS_MAX_RECEPTION_RATE_TAG("max-reception-rate"); //! Topic specific max reception rate

// Participant related tags
constexpr const char* PARTICIPANT_KIND_TAG("kind");   //! Participant Kind
constexpr const char* PARTICIPANT_NAME_TAG("name");   //! Participant Name
constexpr const char* COLLECTION_PARTICIPANTS_TAG("participants"); //! TODO: add comment
constexpr const char* IS_REPEATER_TAG("repeater");   //! Is participant a repeater

// Echo related tags
constexpr const char* ECHO_DATA_TAG("data");            //! Echo Data received
constexpr const char* ECHO_DISCOVERY_TAG("discovery");  //! Echo Discovery received
constexpr const char* ECHO_VERBOSE_TAG("verbose");      //! Echo in verbose mode

// RTPS related tags

// Transport related tags
constexpr const char* WHITELIST_INTERFACES_TAG("whitelist-interfaces"); //! Whitelist interfaces

// Custom transport descriptors tags
constexpr const char* TRANSPORT_DESCRIPTORS_TRANSPORT_TAG("transport"); //! Custom transport descriptors
constexpr const char* TRANSPORT_DESCRIPTORS_BUILTIN_TAG("builtin"); //! Builtin transport (UDP + SHM) (default)
constexpr const char* TRANSPORT_DESCRIPTORS_UDP_TAG("udp"); //! UDP only
constexpr const char* TRANSPORT_DESCRIPTORS_SHM_TAG("shm"); //! Shared Memory only

// Participant discovery settings
constexpr const char* IGNORE_PARTICIPANT_FLAGS_TAG("ignore-participant-flags"); //! Ignore Participant Flags
constexpr const char* IGNORE_PARTICIPANT_FLAGS_NO_FILTER_TAG("no_filter"); //! No filter (default)
constexpr const char* IGNORE_PARTICIPANT_FLAGS_DIFFERENT_HOST_TAG("filter_different_host"); //! Discovery traffic from another host is discarded
constexpr const char* IGNORE_PARTICIPANT_FLAGS_DIFFERENT_PROCESS_TAG("filter_different_process"); //! Discovery traffic from another process on same host is discarded
constexpr const char* IGNORE_PARTICIPANT_FLAGS_SAME_PROCESS_TAG("filter_same_process"); //! Discovery traffic from own process is discarded
constexpr const char* IGNORE_PARTICIPANT_FLAGS_DIFFERENT_AND_SAME_PROCESS_TAG("filter_different_and_same_process"); //! Discovery traffic from own host is discarded

// Simple RTPS related tags
constexpr const char* DOMAIN_ID_TAG("domain"); //! Domain Id of the participant

// Discovery Server related tags
constexpr const char* DISCOVERY_SERVER_GUID_PREFIX_TAG("discovery-server-guid"); //! TODO: add comment
constexpr const char* LISTENING_ADDRESSES_TAG("listening-addresses"); //! TODO: add comment
constexpr const char* CONNECTION_ADDRESSES_TAG("connection-addresses"); //! TODO: add comment
constexpr const char* COLLECTION_ADDRESSES_TAG("addresses"); //! TODO: add comment

// TLS related tags
constexpr const char* TLS_TAG("tls"); //! TLS configuration tag
constexpr const char* TLS_CA_TAG("ca"); //! Certificate Authority Certificate
constexpr const char* TLS_PASSWORD_TAG("password"); //! DDS-Router Password
constexpr const char* TLS_PRIVATE_KEY_TAG("private_key"); //! DDS-Router Certificate Private Key
constexpr const char* TLS_CERT_TAG("cert"); //! DDS-Router Certificate
constexpr const char* TLS_DHPARAMS_TAG("dh_params"); //! Diffie-Hellman (DF) parameters
constexpr const char* TLS_PEER_VERIFICATION_TAG("peer_verification"); //! Peer Verification parameter
constexpr const char* TLS_SNI_HOST_TAG("sni_host"); //! TLS configuration tag

// Address related tags
constexpr const char* ADDRESS_IP_TAG("ip"); //! TODO: add comment
constexpr const char* ADDRESS_DNS_TAG("domain"); //! TODO: add comment
constexpr const char* ADDRESS_PORT_TAG("port"); //! TODO: add comment
constexpr const char* ADDRESS_EXTERNAL_PORT_TAG("external-port"); //! TODO: add comment

constexpr const char* ADDRESS_IP_VERSION_TAG("ip-version"); //! TODO: add comment
constexpr const char* ADDRESS_IP_VERSION_V4_TAG("v4"); //! TODO: add comment
constexpr const char* ADDRESS_IP_VERSION_V6_TAG("v6"); //! TODO: add comment

constexpr const char* ADDRESS_TRANSPORT_TAG("transport"); //! TODO: add comment
constexpr const char* ADDRESS_TRANSPORT_UDP_TAG("udp"); //! TODO: add comment
constexpr const char* ADDRESS_TRANSPORT_TCP_TAG("tcp"); //! TODO: add comment

// Discovery Server Guid related tags
constexpr const char* DISCOVERY_SERVER_GUID_TAG("guid"); //! TODO: add comment
constexpr const char* DISCOVERY_SERVER_ID_TAG("id"); //! TODO: add comment
constexpr const char* DISCOVERY_SERVER_ID_ROS_TAG("ros-discovery-server"); //! TODO: add comment

// Timestamp (std::chrono::time_point) related tags
constexpr const char* TIMESTAMP_DATETIME_TAG("datetime"); //! TODO: add comment
constexpr const char* TIMESTAMP_DATETIME_FORMAT_TAG("format"); //! TODO: add comment
constexpr const char* TIMESTAMP_LOCAL_TAG("local"); //! TODO: add comment
constexpr const char* TIMESTAMP_MILLISECONDS_TAG("milliseconds"); //! TODO: add comment
constexpr const char* TIMESTAMP_MICROSECONDS_TAG("microseconds"); //! TODO: add comment
constexpr const char* TIMESTAMP_NANOSECONDS_TAG("nanoseconds"); //! TODO: add comment

// Custom routes related tags
constexpr const char* ROUTES_TAG("routes"); //! Forwarding routes
constexpr const char* ROUTES_SRC_TAG("src"); //! Route's source participant
constexpr const char* ROUTES_DST_TAG("dst"); //! Route's destination participants
constexpr const char* TOPIC_ROUTES_TAG("topic-routes"); // Topic specific forwarding routes

// Advanced configuration
constexpr const char* SPECS_TAG("specs"); //! Specs options for DDS Router configuration
constexpr const char* NUMBER_THREADS_TAG("threads"); //! Number of threads to configure the thread pool
constexpr const char* MAX_HISTORY_DEPTH_TAG("max-depth"); //! Maximum size (number of stored cache changes) for RTPS History instances
constexpr const char* DOWNSAMPLING_TAG("downsampling"); //! Keep 1 out of every *downsampling* samples received
constexpr const char* MAX_RECEPTION_RATE_TAG("max-reception-rate"); //! Process up to *max_reception_rate* samples in a 1 second bin
constexpr const char* WAIT_ALL_ACKED_TIMEOUT_TAG("wait-all-acked-timeout"); //! Wait for a maximum of *wait-all-acked-timeout* ms until all msgs sent by reliable writers are acknowledged by their matched readers
constexpr const char* REMOVE_UNUSED_ENTITIES_TAG("remove-unused-entities"); //! Dynamically create and delete entities and tracks.

// XML configuration tags
constexpr const char* XML_TAG("xml"); //! Tag to read xml configuration
constexpr const char* XML_RAW_TAG("raw"); //! Xml RAW as string
constexpr const char* XML_FILES_TAG("files"); //! XML files
constexpr const char* XML_PARTICIPANT_PROFILE_TAG("profile"); //! Xml profile to participant

// Old versions tags
constexpr const char* PARTICIPANT_KIND_TAG_V1("type"); //! Participant Kind


} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
