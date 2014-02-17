#pragma once

#include <winsock2.h>
#include "MySocket.h"
#include <string>


class ListeningSocket : public MySocket
{
	friend class SocketFactory;

	private:				
		ListeningSocket(SOCKET socket, voidfunc release) : MySocket(socket, release) {};
};