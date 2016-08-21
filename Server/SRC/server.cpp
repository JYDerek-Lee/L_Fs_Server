#include "stdafx.h"
#include "server.h"

//void L::TCPServer::StartAccept() {
//	std::cout << "Client connect wait....." << std::endl;
//
//	m_pSession = new Session(m_acceptor.get_io_service());
//
//	m_acceptor.async_accept(m_pSession->Socket(),
//		boost::bind(&TCPServer::handle_accept, this,
//		    m_pSession,
//		    boost::asio::placeholders::error)
//	);
//}

void L::TCPServer::acceptHandle(ptrSeesion pSession, const boost::system::error_code& acceptError) {
	if (!acceptError) {
		std::cout << "● 클라이언트 접속" << std::endl;
		std::cout << "●" << __FUNCTION__ << "\n" << std::endl;

		pSession->PostReceive();
	}
}