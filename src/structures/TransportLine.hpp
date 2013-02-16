#ifndef TRANSPORT_LINE_HPP
#define TRANSPORT_LINE_HPP

using namespace std;

enum TransportLineType
{
	BUS_LINE,
	TRAM_LINE,
};

class TransportLine
{
public:
	TransportLine()
	{
	}

	long id;
	TransportLineType type;
	string color;
	string ref;
};

#endif
