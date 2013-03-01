#ifndef ROUTING_TRACER_NODE_HPP
#define ROUTING_TRACER_NODE_HPP

#include "Node.hpp"
#include "Edge.hpp"

class RoutingTracerNode
{
	public:
		RoutingTracerNode(Node* node);
		RoutingTracerNode(Node* node, Edge* ingoing);

		Node* node;
		Edge* ingoing_edge;
		double source_node_time;
		double target_node_time;
		RoutingTracerNode* parent;
		// std::vector<RoutingTracerNode*> _children;
};

#endif
