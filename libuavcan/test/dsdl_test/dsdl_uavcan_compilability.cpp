/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <gtest/gtest.h>

#include <uavcan/helpers/ostream.hpp>

#include <uavcan/Timestamp.hpp>
#include <uavcan/protocol/GetTransportStats.hpp>
#include <uavcan/protocol/Panic.hpp>
#include <uavcan/protocol/RestartNode.hpp>
#include <uavcan/protocol/GlobalTimeSync.hpp>
#include <uavcan/protocol/DataTypeKind.hpp>
#include <uavcan/protocol/GetDataTypeInfo.hpp>
#include <uavcan/protocol/NodeStatus.hpp>
#include <uavcan/protocol/GetNodeInfo.hpp>
#include <uavcan/protocol/debug/LogMessage.hpp>
#include <uavcan/protocol/debug/KeyValue.hpp>

#include <root_ns_a/Deep.hpp>


TEST(Dsdl, Streaming)
{
    std::ostringstream os;

    uavcan::protocol::GetNodeInfo::Response get_node_info_rsp;
    os << get_node_info_rsp << std::endl << "==========" << std::endl;

    root_ns_a::Deep ps;
    ps.a.resize(1);
    os << ps << std::endl << "==========" << std::endl;

    static const std::string Reference =
        "status: \n"
        "  uptime_sec: 0\n"
        "  status_code: 0\n"
        "  vendor_specific_status_code: 0\n"
        "software_version: \n"
        "  major: 0\n"
        "  minor: 0\n"
        "  optional_field_mask: 0\n"
        "  vcs_commit: 0\n"
        "  image_crc: 0\n"
        "hardware_version: \n"
        "  major: 0\n"
        "  minor: 0\n"
        "  unique_id: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n"
        "  certificate_of_authenticity: \"\"\n"
        "name: \"\"\n"
        "==========\n"
        "c: 0\n"
        "str: \"\"\n"
        "a: \n"
        "  - \n"
        "    scalar: 0\n"
        "    vector: \n"
        "      - \n"
        "        vector: [0, 0]\n"
        "        bools: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n"
        "      - \n"
        "        vector: [0, 0]\n"
        "        bools: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n"
        "b: \n"
        "  - \n"
        "    vector: [0, 0]\n"
        "    bools: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n"
        "  - \n"
        "    vector: [0, 0]\n"
        "    bools: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]\n"
        "==========\n";
    std::cout << os.str();
    ASSERT_EQ(Reference, os.str());
}
