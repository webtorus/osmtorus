#ifndef ROUTING_STORAGE_HPP
#define ROUTING_STORAGE_HPP

#include <set>
#include <map>
#include <iostream>

#include "../structures/Edge.hpp"
#include "../structures/Way.hpp"
#include "../structures/Node.hpp"

using namespace std;

struct Edge;
class Way;

class RoutingStorage
{
	public:
		RoutingStorage();
		RoutingStorage(map<long, Node*>& nodes, map<long, Way*>& ways);

		virtual void write(ostream& output_nodes, ostream& output_ways, ostream& output_edges);
		virtual void read(istream& input_nodes, istream& input_ways, istream& input_edges);

		virtual map<long, Node*> getNodes() const;
		virtual map<long, Way*> getWays() const;

		template<class T>
		static istream& read(istream& is, T& value);

		template<class T>
		static ostream& write(ostream& os, const T& value);

	private:
		void writeNodes(ostream& output);
		void writeWays(ostream& output);
		void writeEdges(ostream& output);

		void readNodes(istream& input);
		void readWays(istream& input);
		void readEdges(istream& input);

		map<long, Node*> nodes;
		map<long, Way*> ways;
};

#endif
