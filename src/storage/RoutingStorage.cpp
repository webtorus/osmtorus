#include <iostream>
#include "RoutingStorage.hpp"

using namespace std;

RoutingStorage::RoutingStorage()
{
}

RoutingStorage::RoutingStorage(map<long, Node*>& nodes, map<long, Way*>& ways): nodes(nodes), ways(ways)
{
}

template<typename T>
ostream& RoutingStorage::write(ostream& os, const T& value)
{
    return os.write(reinterpret_cast<const char*>(&value), sizeof(T));
}
 
template<typename T>
istream& RoutingStorage::read(istream& is, T& value)
{
    return is.read(reinterpret_cast<char*>(&value), sizeof(T));
}

void RoutingStorage::write(ostream& output_nodes, ostream& output_ways, ostream& output_edges)
{
	writeNodes(output_nodes);
	writeWays(output_ways);
	writeEdges(output_edges);
}

void RoutingStorage::writeNodes(ostream& output)
{
	map<long, Node*>::iterator it;

	for (it = nodes.begin(); it != nodes.end(); it++) {
		Node* node = it->second;
		write(output, node->id);
		write(output, node->lat);
		write(output, node->lon);
	}
}

void RoutingStorage::writeWays(ostream& output)
{
	map<long, Way*>::iterator it;

	for (it = ways.begin(); it != ways.end(); it++) {
		Way* way = it->second;
		write(output, way->id);
		write(output, way->type);
		long size = way->name.size();
		write(output, size);
		output.write(way->name.data(), size);
	}
}

void RoutingStorage::writeEdges(ostream& output)
{
	map<long, Node*>::iterator it_n;

	for (it_n = nodes.begin(); it_n != nodes.end(); it_n++) {
		Node* node = it_n->second;
		
		set<Edge*>::iterator it_e;

		for (it_e = node->neighbors.begin(); it_e != node->neighbors.end(); it_e++) {
			Edge* edge = *it_e;

			write(output, node->id);
			write(output, edge->to->id);
			write(output, edge->way->id);
			write(output, edge->distance);
		}
	}
}

void RoutingStorage::read(istream& input_nodes, istream& input_ways, istream& input_edges)
{
	nodes.clear();
	ways.clear();

	readNodes(input_nodes);
	readWays(input_ways);
 	readEdges(input_edges);
}

void RoutingStorage::readNodes(istream& input)
{
	input.get();
	while (input.good()) {
		input.unget();
		Node* node = new Node();
		read(input, node->id);
		read(input, node->lat);
		read(input, node->lon);
		nodes[node->id] = node;
		input.get();
	}
}

void RoutingStorage::readWays(istream& input)
{
	input.get();
	while (input.good()) {
		input.unget();
		Way* way = new Way();
		read(input, way->id);
		read(input, way->type);
		long size;
		read(input, size);
		char* name = new char[size];
		input.read(name, (int) size);
		way->name.assign(name, size);
		ways[way->id] = way;
		input.get();
	}
}

void RoutingStorage::readEdges(istream& input)
{
	input.get();
	while (input.good()) {
		input.unget();
		Edge* e = new Edge();
		long from_id = 0;
		long to_id = 0;
		long way_id = 0;
		read(input, from_id);
		read(input, to_id);
		read(input, way_id);
		unsigned int d;
		read(input, d);

		Node* from = nodes[from_id];
		Node* to = nodes[to_id];

		Way* way = ways[way_id];
		e->way = way;
		e->to = to;

		from->neighbors.insert(e);
		from->ways[way->id] = way;
		input.get();
	}
}

map<long, Node*> RoutingStorage::getNodes() const
{
	return nodes;
}

map<long, Way*> RoutingStorage::getWays() const
{
	return ways;
}