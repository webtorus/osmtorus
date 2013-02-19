#ifndef ROUTING_TRACER_NODE_HPP
#define ROUTING_TRACER_NODE_HPP

#include <vector>

class RoutingTracerNode
{
	public:
		RoutingTracerNode(long id);

		long _id;
		double _source_node_time;
		double _target_node_time;
		RoutingTracerNode* _parent;
		std::vector<RoutingTracerNode*> _children;
};

#endif
