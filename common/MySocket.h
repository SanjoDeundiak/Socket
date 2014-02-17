#pragma once

#include <string>
const size_t RcvBuff = 1024;
const std::string PORT = "10777";

#include <WinSock2.h>
#include <functional>

typedef std::function<void()> voidfunc;

class MySocket
{
	friend class SocketFactory;
	public:
		~MySocket();		
	protected:
		voidfunc m_release;		
		MySocket(SOCKET socket, voidfunc release);	
		SOCKET m_socket; //SOCKET to connect with other side
};