#include <cstring>
#include <iostream>

#include "Edge.hpp"

using namespace std;

Edge::Edge(): distance(0), to(NULL), way(NULL)
{
}

unsigned int Edge::distanceBetween(Node* n1, Node* n2)
{
	double RAD = 0.017453292519943295769236907684886;
	double lt1 = n1->lat / 100000.;
	double ln1 = n1->lon / 100000.;
	double lt2 = n2->lat / 100000.;
	double ln2 = n2->lon / 100000.;
	double dlat1 = lt1 * (RAD);

	double dlong1 = ln1 * (RAD);
	double dlat2 = lt2 * (RAD);
	double dlong2 = ln2 * (RAD);

	double dLong = dlong1 - dlong2;
	double dLat = dlat1-dlat2;

	double aHarv = pow(sin(dLat / 2.0), 2.0) + cos(dlat1) * cos(dlat2) * pow(sin(dLong / 2.), 2);
	double cHarv = 2. * atan2(sqrt(aHarv), sqrt(1.0 - aHarv));

	const double earth = 6372797.560856;
	double distance = earth * cHarv;

	return distance;
}

bool operator<(const Edge& e1, const Edge& e2)
{
	return &e1 < &e2;
	// return e1.to->id < e2.to->id;
}