#include "SocketFactory.h"
#include <iostream>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	SocketFactory *pSocketFactory;
	std::shared_ptr<ListeningSocket> lSocket;
	std::shared_ptr<ClientSocket> cSocket;
	try
	{
		pSocketFactory = SocketFactory::getFactory();

	
		lSocket = pSocketFactory->createListeningSocket("10777");
		cSocket = pSocketFactory->createClientSocket(*lSocket);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cout << "Trying to receive message" << std::endl;
	try
	{
		std::cout << cSocket->Recv() << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
	std::cin.get();

	SocketFactory::Release();
}