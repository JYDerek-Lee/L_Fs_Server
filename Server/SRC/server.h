#include "stdafx.h"
#include "session.h"

const unsigned short PORT_NUMBER = 6666;

namespace L {
	class TCP_Server {
	public:
		TCP_Server(boost::asio::io_service& io_service)
			: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)) {
			m_pSession = nullptr;
			StartAccept();
		}

		~TCP_Server() {
			if (m_pSession != nullptr) {
				delete m_pSession;
			}
		}

	private:
		void StartAccept();
		void handle_accept(Session* pSession, const boost::system::error_code& error);

		int m_nSeqNumber;
		boost::asio::ip::tcp::acceptor m_acceptor;
		Session* m_pSession;
	};
}