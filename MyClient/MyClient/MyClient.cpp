#include "SocketFactory.h"
#include <iostream>
#include <sstream>

#include "MyClient.h"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

MyClient::MyClient(std::string address) :
m_address(address)
{
}

void MyClient::start()
{
	std::cout << "Creating socket... ";
	std::shared_ptr<ClientSocket> cSocket;
	try
	{
		cSocket = SocketFactory::createClientSocket(m_address, PORT);		
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		return;
	}
	std::cout << "Success" << std::endl;
	std::cin.get();
	std::string line, command, tagname, tagvalue, feedback;
	while (1)
	{
		std::getline(std::cin, line);
		std::stringstream ss(line);
		command = ""; tagname = ""; tagvalue = "no";
		ss >> command;
		ss >> tagname;
		
		if (command == "add" || command == "set")					
			ss >> tagvalue;
		
		if (command != "" && tagname != "" && tagvalue != "")
			line = command + " " + tagname + " " + tagvalue;
		else
		{
			std::cout << "Bad request" << std::endl;
			continue;
		}
			
		try
		{
			cSocket->Send(line);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;			
			return;
		}
		try
		{
			feedback = cSocket->Recv();
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;			
			return;
		}
		std::cout << feedback << std::endl;
	}	
}