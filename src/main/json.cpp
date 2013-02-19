#include "../include/JsonResponse.hpp"
#include "../include/OsmLoader.hpp"
#include "../include/RoutingGraph.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
	string osm_filename = "sample/map.osm";

	OsmLoader loader;
	cout << loader.run(osm_filename) << endl;

	vector<long> nodes_ids;

	RoutingGraph& graph = loader.getRoutingGraph();
	int i = 0;
	for (auto node_entry: graph.nodes) {
		nodes_ids.push_back(node_entry.second->id);
		if (i > 10) {
			break;
		}
		i++;
	}

	JsonResponse response;
	response.run(3.85, 43.5, 3.89, 43.17, nodes_ids, loader);

	cout << response.getResponse() << endl;
}