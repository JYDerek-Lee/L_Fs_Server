#include "stdafx.h"
#include "session.h"

std::string L::Session::reName(std::string cur_name, std::string desc_name) {
	path cur(cur_name); path desc(desc_name);
	if (exists(cur_name))
		rename(cur, desc);
	else
		return "No such file or directory";
	return "success!!";
}

std::string L::Session::getPath() {
	path my_path(initial_path());
	return my_path.string();
}

std::string L::Session::removeFile(std::string file_name) {
	path my_path(file_name);
	if (exists(my_path)) {
		remove_all(my_path);
	}
	else {
		return "No such file or directory";
	}
	return "success!!";
}

std::string L::Session::removeDir(std::string dir_name) {
	try {
		if (is_directory(dir_name))
			remove_all(dir_name);
	}
	catch (filesystem_error const& e) {
		std::cout << e.what() << std::endl;
		return "No such file or directory";
	}
	return "success!!";
}

std::string L::Session::createFile(std::string file_name) {
	std::ofstream out(file_name);
	return "success!!";
}

std::string L::Session::createDir(std::string dir_name) {
	path dir(dir_name);
	if (create_directory(dir)) {
		return "success!!";
	}
	return "failure!!";
}

std::string L::Session::getList() {
	std::string cur_dir_list = "\n";
	path p(initial_path());
	directory_iterator end_itr;

	for (directory_iterator itr(p); itr != end_itr; ++itr) {
		cur_dir_list += itr->path().stem().string();
		cur_dir_list += "\n";
	}
	return cur_dir_list;
}

std::string L::Session::fileUpload(std::string file_name) {
	char read_buf[1024] = { 0, };
	int fileSize = 0;
	std::string getFileSize;
	std::ofstream ost;
	ost.open(file_name);

	//m_Socket.read_some(boost::asio::buffer(getFileSize));
	//fileSize = atoi(getFileSize.c_str());

	do {
		m_Socket.read_some(boost::asio::buffer(read_buf));	
		if (!strcmp(read_buf, "END")) break;
		ost << read_buf << std::endl;
		std::cout << "get data is : " << read_buf << std::endl;
	} while (sizeof(read_buf) < fileSize);
	
	ost.close();
	std::cout << "fileUpload Get End" << std::endl;
	return "success!!";
}

void L::Session::PostReceive() {
	memset(&m_ReceiveBuffer, 0x00, sizeof(m_ReceiveBuffer));

	m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&Session::handleReceive, 
		    this,
		    boost::asio::placeholders::error,
		    boost::asio::placeholders::bytes_transferred)
	);
}

void L::Session::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
	//m_WriteMessage = ""; 
}

void L::Session::handleReceive(const boost::system::error_code& error, size_t bytes_transferred) {
	if (error) {
		if (error == boost::asio::error::eof) {
			std::cout << "Client disconnect." << std::endl;
		}
		else {
			std::cout << "error No : " << error.value() << " error Message : " << error.message() << std::endl;
		}
	}
	else {
		const std::string strRecvMessage = m_ReceiveBuffer;
		std::cout << "\n\nMessage from Client : " << strRecvMessage << std::endl;
		std::stringstream msgBuffer(strRecvMessage);
		msgBuffer >> mbuffer;

		if (mbuffer == "pwd") {
			m_WriteMessage = getPath();
		}
		else if (mbuffer == "ls") {
			m_WriteMessage = getList();
		}
		else if (mbuffer == "mkdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = createDir(cur_name);
		}
		else if (mbuffer == "rmdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = removeDir(cur_name);
		}
		else if (mbuffer == "mkfile") {
			msgBuffer >> cur_name;
			m_WriteMessage = createFile(cur_name);
		}
		else if (mbuffer == "rm") {
			msgBuffer >> cur_name;
			m_WriteMessage = removeFile(cur_name);
		}
		else if (mbuffer == "cp") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
		}
		else if (mbuffer == "rename") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
			m_WriteMessage = reName(cur_name, desc_name);
		}
		else if (mbuffer == "upload") {
			msgBuffer >> cur_name;
			m_WriteMessage = fileUpload(cur_name);
		}
		else {
			m_WriteMessage = "command not found";
		}
		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&Session::handleWrite, 
			    this,
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred)
		);
		std::cout << "Send Process Ok" << std::endl;
		PostReceive();
	}
}