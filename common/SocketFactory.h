#pragma once

#include "ListeningSocket.h"
#include "ClientSocket.h"
#include <memory>
#include <mutex>
#include <string>

class SocketFactory
{
	public:
		static std::shared_ptr<ClientSocket> createClientSocket(ListeningSocket &socket);
		static std::shared_ptr<ClientSocket> createClientSocket(const std::string &serverName, const std::string &serviceName);
		static std::shared_ptr<ListeningSocket> createListeningSocket(const std::string &serviceName);

	private:
		static int m_cRef;
		static void Release();
		static void Init();	
};