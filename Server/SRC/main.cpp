#include "stdafx.h"
#include "server.h"

int main() {
	boost::asio::io_service io_service;
	L::TCP_Server server(io_service);
	io_service.run();
	std::cout << "Network Service exit..." << std::endl;
	getchar();
	return 0;
}