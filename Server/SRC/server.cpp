#include "stdafx.h"
#include "server.h"

void L::TCP_Server::StartAccept() {
	std::cout << "Client connect wait....." << std::endl;

	m_pSession = new Session(m_acceptor.get_io_service());

	m_acceptor.async_accept(m_pSession->Socket(),
		boost::bind(&TCP_Server::handle_accept, this,
		    m_pSession,
		    boost::asio::placeholders::error)
	);
}

void L::TCP_Server::handle_accept(Session* pSession, const boost::system::error_code& error) {
	if (!error) {
		std::cout << "Client connect!!!" << std::endl;

		pSession->PostReceive();
	}
}