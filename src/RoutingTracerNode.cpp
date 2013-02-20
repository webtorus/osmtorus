#include "include/RoutingTracerNode.hpp"

RoutingTracerNode::RoutingTracerNode(Node* node): node(node), ingoing_edge(NULL), source_node_time(0.0), target_node_time(0.0), parent(NULL)
{

}

RoutingTracerNode::RoutingTracerNode(Node* node, Edge* ingoing): node(node), ingoing_edge(ingoing), source_node_time(0.0), target_node_time(0.0), parent(NULL)
{

}
