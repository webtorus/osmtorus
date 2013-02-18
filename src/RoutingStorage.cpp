#include "include/RoutingStorage.hpp"
#include "include/Edge.hpp"
#include "include/Node.hpp"
#include "include/RoutingGraph.hpp"
#include "include/TransportLine.hpp"
#include "include/Way.hpp"
#include <cstring>
#include <iostream>
#include <map>

using namespace std;

RoutingStorage::RoutingStorage(RoutingGraph& g): graph(g)
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

void RoutingStorage::write(ostream& output_nodes, ostream& output_lines, ostream& output_ways, ostream& output_edges)
{
	writeNodes(output_nodes);
	writeLines(output_lines);
	writeWays(output_ways);
	writeEdges(output_edges);
}

void RoutingStorage::writeNodes(ostream& output)
{
	write(output, graph.boxes.min_x);
	write(output, graph.boxes.min_y);
	write(output, graph.boxes.max_x);
	write(output, graph.boxes.max_y);

	map<long, Node*>::iterator it;

	for (it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
		Node* node = it->second;
		write(output, node->id);
		write(output, node->lat);
		write(output, node->lon);
	}
}

void RoutingStorage::writeLines(ostream& output)
{
	map<long, TransportLine*>::iterator it;

	for (it = graph.transport_lines.begin(); it != graph.transport_lines.end(); it++) {
		TransportLine* line = it->second;
		write(output, line->id);
		write(output, line->type);

		short size_color = line->color.size() + 1;
		write(output, size_color);
		output.write(line->color.c_str(), size_color);

		short size_ref = line->ref.size() + 1;
		write(output, size_ref);
		output.write(line->ref.c_str(), size_ref);
	}
}

void RoutingStorage::writeWays(ostream& output)
{
	map<long, Way*>::iterator it;

	for (it = graph.ways.begin(); it != graph.ways.end(); it++) {
		Way* way = it->second;
		write(output, way->id);
		write(output, way->type);
		short nb_lines = way->transport_lines.size();
		write(output, nb_lines);
		map<long, TransportLine*>::iterator it_tl;
		for (it_tl = way->transport_lines.begin(); it_tl != way->transport_lines.end(); it_tl++) {
			TransportLine* line = it_tl->second;
			write(output, line->id);
		}

		short size_name = way->name.size() + 1;
		write(output, size_name);
		output.write(way->name.c_str(), size_name);
	}
}

void RoutingStorage::writeEdges(ostream& output)
{
	set<Edge*>::iterator it;
	for (it = graph.edges.begin(); it != graph.edges.end(); it++) {
		Edge* edge = *it;

		write(output, edge->from->id);
		write(output, edge->to->id);
		write(output, edge->way->id);
		write(output, edge->length);
	}
}

void RoutingStorage::read(istream& input_nodes, istream& input_lines, istream& input_ways, istream& input_edges)
{
	graph.nodes.clear();
	graph.transport_lines.clear();
	graph.ways.clear();
	graph.edges.clear();

	readNodes(input_nodes);
	readLines(input_lines);
	readWays(input_ways);
 	readEdges(input_edges);
}

void RoutingStorage::readNodes(istream& input)
{
	read(input, graph.boxes.min_x);
	read(input, graph.boxes.min_y);
	read(input, graph.boxes.max_x);
	read(input, graph.boxes.max_y);

	input.get();
	while (input.good()) {
		input.unget();

		Node* node = new Node();
		read(input, node->id);
		read(input, node->lat);
		read(input, node->lon);
		graph.nodes[node->id] = node;
		graph.boxes.getBoxOf(node->lat, node->lon)->insert(node);

		input.get();
	}
}

void RoutingStorage::readLines(istream& input)
{
	input.get();
	while (input.good()) {
		input.unget();

		TransportLine* line = new TransportLine();
		read(input, line->id);
		read(input, line->type);

		short size_color;
		read(input, size_color);
		char color[size_color];
		input.read(color, size_color);
		line->color = color;

		short size_ref;
		read(input, size_ref);
		char ref[size_ref];
		input.read(ref, size_ref);
		line->ref = ref;

		graph.transport_lines[line->id] = line;

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
		short nb_lines;
		read(input, nb_lines);
		for (int i = 0; i < nb_lines; i++) {
			long line_id;
			read(input, line_id);
			TransportLine* line = graph.transport_lines[line_id];
			way->transport_lines[line->id] = line;
		}
		short size_name;
		read(input, size_name);

		char name[size_name];
		input.read(name, size_name);
		way->name = name;
		
		graph.ways[way->id] = way;

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
		unsigned int length;

		read(input, from_id);
		read(input, to_id);
		read(input, way_id);
		read(input, length);

		Node* from = graph.nodes[from_id];
		Node* to = graph.nodes[to_id];
		Way* way = graph.ways[way_id];

		e->way = way;
		e->to = to;
		e->from = from;
		e->length = length;

		from->neighbors.insert(e);
		from->ways[way->id] = way;
		to->ways[way->id] = way;

		graph.edges.insert(e);
		
		input.get();
	}
}

RoutingGraph& RoutingStorage::getGraph()
{
	return graph;
}
