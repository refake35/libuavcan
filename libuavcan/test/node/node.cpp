/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#include <gtest/gtest.h>
#include <uavcan/node/node.hpp>
#include <uavcan/node/sub_node.hpp> // Compilability test
#include <uavcan/protocol/node_status_monitor.hpp>
#include "test_node.hpp"
#include "../protocol/helpers.hpp"

static void registerTypes()
{
    uavcan::GlobalDataTypeRegistry::instance().reset();
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::NodeStatus> _reg2;
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::GetNodeInfo> _reg4;
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::GetDataTypeInfo> _reg5;
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::debug::LogMessage> _reg6;
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::GetTransportStats> _reg7;
    uavcan::DefaultDataTypeRegistrator<uavcan::protocol::RestartNode> _reg8;
}


TEST(Node, Basic)
{
    registerTypes();
    InterlinkedTestNodesWithSysClock nodes;

    uavcan::protocol::SoftwareVersion swver;
    swver.major = 0;
    swver.minor = 1;
    swver.vcs_commit = 0xDEADBEEF;

    /*
     * uavcan::Node
     */
    uavcan::Node<0> node1(nodes.can_a, nodes.clock_a);
    std::cout << "sizeof(uavcan::Node<0>): " << sizeof(uavcan::Node<0>) << std::endl;
    node1.setName("com.example");
    node1.setNodeID(1);
    node1.setSoftwareVersion(swver);

    /*
     * Companion test node
     */
    uavcan::Node<0> node2(nodes.can_b, nodes.clock_b);
    node2.setName("foobar");
    node2.setNodeID(2);
    node2.setSoftwareVersion(swver);

    BackgroundSpinner bgspinner(node2, node1);
    bgspinner.startPeriodic(uavcan::MonotonicDuration::fromMSec(10));

    uavcan::NodeStatusMonitor node_status_monitor(node2);
    ASSERT_LE(0, node_status_monitor.start());

    /*
     * Init the second node - network is empty
     */
    ASSERT_LE(0, node2.start());
    ASSERT_FALSE(node_status_monitor.findNodeWithWorstStatus().isValid());

    /*
     * Init the first node
     */
    ASSERT_FALSE(node1.isStarted());
    ASSERT_EQ(-uavcan::ErrNotInited, node1.spin(uavcan::MonotonicDuration::fromMSec(20)));
    ASSERT_LE(0, node1.start());
    ASSERT_TRUE(node1.isStarted());

    ASSERT_LE(0, node1.spin(uavcan::MonotonicDuration::fromMSec(2000)));

    ASSERT_EQ(1, node_status_monitor.findNodeWithWorstStatus().get());

    /*
     * Some logging
     */
    SubscriberWithCollector<uavcan::protocol::debug::LogMessage> log_sub(node2);
    ASSERT_LE(0, log_sub.start());

    node1.getLogger().setLevel(uavcan::protocol::debug::LogLevel::DEBUG);
    node1.logInfo("test", "6 * 9 = 42");

    ASSERT_LE(0, node1.spin(uavcan::MonotonicDuration::fromMSec(20)));
    ASSERT_LE(0, node2.spin(uavcan::MonotonicDuration::fromMSec(20)));

    ASSERT_TRUE(log_sub.collector.msg.get());
    std::cout << *log_sub.collector.msg << std::endl;
}
