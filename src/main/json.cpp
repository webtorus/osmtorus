#include "../include/JsonResponse.hpp"
#include "../include/OsmLoader.hpp"
#include "../include/RoutingGraph.hpp"
#include "../include/RoutingTracer.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 5) {
		cout << "bin/json 43.622252 3.858335 43.604878 3.880791" << endl;
		return 0;
	}
	string osm_filename = "sample/map.osm";

	OsmLoader loader;
	cout << loader.run(osm_filename) << endl;

	RoutingTracer tracer;
	tracer.run(atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]), 9, loader.getRoutingGraph());

	JsonResponse response;
	response.run(atof(argv[1]), atof(argv[2]), atof(argv[3]), atof(argv[4]), tracer.getRoutingEdges(), loader.getRoutingGraph());

	cout << response.getResponse() << endl;
}

