#include "stdafx.h"

namespace L {
	class Session : public boost::enable_shared_from_this <Session>{
	private:
		streambuf getBuffer;
		streambuf postBuffer;

		std::string mbuffer;
		std::string cur_name;
		std::string desc_name;
		std::string m_WriteMessage;
		char m_ReceiveBuffer[1024];
		tcp::socket m_Socket;

		void WriteHandle(const boost::system::error_code& writeError, size_t bytes_transferred);
		void ReceiveHandle(const boost::system::error_code& receiveError, size_t bytes_transferred);
		void ErrorHandle(const std::string& functionName, const boost::system::error_code& err);

		void getPath();
		std::string getList();
		std::string removeFile(std::string file_name);
		std::string removeDir(std::string dir_name);
		std::string createFile(std::string file_name);
		std::string createDir(std::string dir_name);
		std::string fileUpload(std::string file_name);
		std::string reName(std::string cur_name, std::string desc_name);

	public:
		//���� �ʱ�ȭ
		Session(io_service& ioService)
			: m_Socket(ioService) {}

		//������ ������ ��ȯ
		tcp::socket& socket() {
			return m_Socket;
		}

		void PostReceive();
	};
}