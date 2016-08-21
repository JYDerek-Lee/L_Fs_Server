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

void L::Session::getPath() {
	std::iostream returnData(&postBuffer);

	std::cout << __FUNCTION__ << "경로 요청" << std::endl;

	directory_iterator end;
	for (directory_iterator iterator(current_path()); iterator!=end; iterator++) {
		if (is_directory(*iterator)) returnData << "폴더 : ";
		else returnData << "파일 : ";
		returnData << iterator->path().leaf() << "\n";
	}

	returnData << "123" << "\n\n";
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



//=====================================================================
//=====================================================================
//=====================================================================
//=====================================================================
//=====================================================================
//=====================================================================
void L::Session::PostReceive() {
	std::cout << "● 클라이언트 접속 시작" << std::endl;
	std::cout << "●" << __FUNCTION__ << "\n" << std::endl;

	//memset(&getBuffer, 0x00, sizeof(getBuffer));
	
	async_read_until(m_Socket, getBuffer, "\n\n",
		boost::bind(&L::Session::ReceiveHandle,
		    shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
}

void L::Session::ReceiveHandle(const boost::system::error_code& receiveError, size_t bytes_transferred) {
	if (receiveError) {
		if (receiveError == boost::asio::error::eof) {
			//std::cout << "Client disconnect." << std::endl;
			ErrorHandle(__FUNCTION__, receiveError);
		}
		else {
			//std::cout << "error No : " << receiveError.value() << " error Message : " << receiveError.message() << std::endl;
			ErrorHandle(__FUNCTION__, receiveError);
		}
	}
	else {
		std::iostream getBuffer_(&getBuffer);
		std::string commend;

		getBuffer_ >> commend;

		if (commend == "getPath") {
			getPath();
		}
		else if (commend == "ls") {
			m_WriteMessage = getList();
		}
		/*else if (commend == "mkdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = createDir(cur_name);
		}
		else if (commend == "rmdir") {
			msgBuffer >> cur_name;
			m_WriteMessage = removeDir(cur_name);
		}
		else if (commend == "mkfile") {
			msgBuffer >> cur_name;
			m_WriteMessage = createFile(cur_name);
		}
		else if (commend == "rm") {
			msgBuffer >> cur_name;
			m_WriteMessage = removeFile(cur_name);
		}
		else if (commend == "cp") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
		}
		else if (commend == "rename") {
			msgBuffer >> cur_name;
			msgBuffer >> desc_name;
			m_WriteMessage = reName(cur_name, desc_name);
		}
		else if (commend == "upload") {
			msgBuffer >> cur_name;
			m_WriteMessage = fileUpload(cur_name);
		}
		else {
			m_WriteMessage = "command not found";
		}*/

		std::cout << "클라에게 데이터 전송" << std::endl;
		async_write(m_Socket, postBuffer,
			boost::bind(&Session::WriteHandle,
			    shared_from_this(),
			    boost::asio::placeholders::error,
			    boost::asio::placeholders::bytes_transferred)
		);

		PostReceive();
	}
}

void L::Session::WriteHandle(const boost::system::error_code& writeError, size_t bytes_transferred) {
	//m_WriteMessage = ""; 
}

void L::Session::ErrorHandle(const std::string& functionName, const boost::system::error_code& err) {
	std::cout << "● Error is Found ●" << std::endl;
	std::cout << __FUNCTION__ << " Error : " << functionName << "What for : " << err << " " << err.message() << std::endl;
}