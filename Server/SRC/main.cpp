#include "stdafx.h"
#include "server.h"

int main() {
	std::string input;
	std::cout << "Port�� �Է��ϼ��� : ";
	getline(std::cin, input, '\n');
	
	unsigned short port_ = atoi(input.c_str());
	L::TCPServer server(port_);

	std::cout << "Network Service exit..." << std::endl;
	getchar();

	return 0;
}