#undef UNICODE

#include "SocketFactory.h"
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <iostream>
#include <sstream>

int SocketFactory::m_cRef = 0;

void SocketFactory::Release()
{
	--m_cRef;

	if (!m_cRef)
		WSACleanup();
}

void SocketFactory::Init()
{
	WSADATA wsaData;
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsaData)) 
		throw std::runtime_error("WSAStartup failed with error");
}

std::shared_ptr<ClientSocket> SocketFactory::createClientSocket(ListeningSocket &socket)
{
	if (m_cRef == 0)
		SocketFactory::Init();
	++m_cRef;	

	struct sockaddr_in clientInfo;
	int addrlen = sizeof(clientInfo);
	// Accept a client socket
	SOCKET clSocket = accept(socket.m_socket, (sockaddr*)&clientInfo, &addrlen);
    if (clSocket == INVALID_SOCKET) 
	{
		std::cout << WSAGetLastError() << std::endl;
		throw std::runtime_error("accept failed with error");  
    }

	std::stringstream ss;
	ss << (int)clientInfo.sin_addr.S_un.S_un_b.s_b1 << '.'
	   << (int)clientInfo.sin_addr.S_un.S_un_b.s_b2 << '.'
	   << (int)clientInfo.sin_addr.S_un.S_un_b.s_b3 << '.'
	   << (int)clientInfo.sin_addr.S_un.S_un_b.s_b4;
	
	return std::shared_ptr<ClientSocket>(new ClientSocket(clSocket, ss.str(), Release));
}

std::shared_ptr<ClientSocket> SocketFactory::createClientSocket(const std::string &serverName, const std::string &serviceName)
{
	if (m_cRef == 0)
		SocketFactory::Init();
	++m_cRef;

    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL, *ptr = NULL, hints;
 
    int iResult;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(serverName.c_str(), serviceName.c_str(), &hints, &result);
    if (iResult != 0)
	{
		if (!result)
			freeaddrinfo(result);
		throw std::invalid_argument("getaddrinfo failed with error");
	}

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
		{
			if (!result)
				freeaddrinfo(result);
			throw std::runtime_error("socket failed with error");
		}
            
        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) 		            
            ConnectSocket = INVALID_SOCKET;                    
		else break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) 
		throw std::runtime_error("Unable to connect to server!");

	return std::shared_ptr<ClientSocket>(new ClientSocket(ConnectSocket, serviceName, Release));
}

std::shared_ptr<ListeningSocket> SocketFactory::createListeningSocket(const std::string &serviceName)
{
	if (m_cRef == 0)
		Init();
	++m_cRef;

    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
	iResult = getaddrinfo(NULL, serviceName.c_str(), &hints, &result);
    if ( iResult != 0 )
	{
		if (!result)
			freeaddrinfo(result);
		throw std::invalid_argument("getaddrinfo failed with error");
	}

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)	
	{
		if (!result)
			freeaddrinfo(result);
		throw std::runtime_error("socket failed with error");
	}

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) 
	{
		freeaddrinfo(result);		
		throw std::runtime_error("bind failed with error");     
    }

    freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) 				
		throw std::runtime_error("listen failed with error");        

	return std::shared_ptr<ListeningSocket>(new ListeningSocket(ListenSocket, Release));
}
