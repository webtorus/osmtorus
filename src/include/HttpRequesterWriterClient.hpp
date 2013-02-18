#ifndef HTTP_REQUESTER_WRITER_CLIENT_HPP
#define HTTP_REQUESTER_WRITER_CLIENT_HPP

#include <string>

class HttpRequesterWriterClient
{
	public:
		HttpRequesterWriterClient();
		bool run(std::string url);
};

#endif
