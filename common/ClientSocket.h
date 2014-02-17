#pragma once

#include <WinSock2.h>
#include "MySocket.h"
#include <ws2tcpip.h>
#include <string>

class ClientSocket : public MySocket
{
	friend class SocketFactory;
	public:
		void Send(std::string msg);
		std::string Recv();
		std::string conFrom() const;
		~ClientSocket();
		
	private:
		std::string m_conFrom;
		ClientSocket(SOCKET socket, const std::string &conFrom, voidfunc release) : MySocket(socket, release), m_conFrom(conFrom) {};
};