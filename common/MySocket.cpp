#include "MySocket.h"
#include <stdexcept>

MySocket::MySocket(SOCKET socket, voidfunc release) :	
	m_socket(socket),
	m_release(release)
{
}

MySocket::~MySocket()
{
	closesocket(m_socket);
	m_release();
}