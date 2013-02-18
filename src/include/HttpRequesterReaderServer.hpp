#ifndef HTTP_REQUESTER_READER_SERVER_HPP
#define HTTP_REQUESTER_READER_SERVER_HPP

#include "SocketTCP.hpp"

class HttpRequesterReaderServer
{
	public:
		HttpRequesterReaderServer();

		bool run(SocketTCP& requester_socket);

		double getLatSource() const;
		double getLonSource() const;

		double getLatTarget() const;
		double getLonTarget() const;

		short getRoutingType() const;

	private:
		double _lat_source;
		double _lon_source;

		double _lat_target;
		double _lon_target;

		short _routing_type;
};

#endif
