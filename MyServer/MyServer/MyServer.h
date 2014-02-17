#include "SocketFactory.h"
#include "XMLParser.h"

class MyServer
{
	public:		
		void start(std::string serviceName);
		MyServer(std::string path);
	private:
		XMLParser parser;
		std::string m_path;
		int m_clientCounter;
		void socketFunc(std::shared_ptr<ClientSocket> cSocket);
		std::shared_ptr<ListeningSocket> lSocket;
		std::mutex mut;
};