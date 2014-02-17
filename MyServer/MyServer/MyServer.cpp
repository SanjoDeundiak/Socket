#include "MyServer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <list>

#define xml "ServerXML.xml"

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

MyServer::MyServer(std::string path) :
	m_path(path),
	m_clientCounter(0)
{
}

void MyServer::start(std::string serviceName)
{
	parser.setPath(m_path);
	std::cout << "Creating listening socket... ";
	lSocket = SocketFactory::createListeningSocket(serviceName);
	std::cout << "Success" << std::endl;
			
	std::list<std::shared_ptr<std::thread>> threads;
	while (true)
	{
		std::cout << "Waiting for connection... ";
		std::shared_ptr<ClientSocket> cSocket = SocketFactory::createClientSocket(*lSocket);
		std::cout << "Success" << std::endl << "Working with client " << cSocket->conFrom() << std::endl;
		threads.push_back(std::shared_ptr<std::thread>(new std::thread(&MyServer::socketFunc, this, cSocket)));		
	}
}

void MyServer::socketFunc(std::shared_ptr<ClientSocket> cSocket)
{	
	std::string line, command, tagname, tagvalue, result;
	while (true)
	{
		result = "";
		try
		{
			line = cSocket->Recv();
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}

		std::stringstream ss(line);
		ss >> command;
		ss >> tagname;

		tagvalue = "no";
		if (command == "add" || command == "set")
			ss >> tagvalue;

		mut.lock();
		if (command == "" || tagname == "" || tagvalue == "")
			result = "Bad request";
		else
		{
			if (command == "add")
			{
				try
				{
					parser.Add(tagname, tagvalue);
				}
				catch (std::exception &e)
				{
					result = e.what();
				}
			}
			else if (command == "set")
			{
				try
				{
					parser.Set(tagname, tagvalue);
				}
				catch (std::exception &e)
				{
					result = e.what();
				}
			}
			else if (command == "get")
			{
				try
				{
					result = parser.Get(tagname);
					if (result == "")
						result = "Tag " + tagname + " doesn't exist";
				}
				catch (std::exception &e)
				{
					result = e.what();
				}
			}
			else if (command == "remove")
			{
				try
				{
					parser.Remove(tagname);
				}
				catch (std::exception &e)
				{
					result = e.what();
				}
			}
			else
				result = "Unknown command";
			if (result == "")
				result = "Success";
		}
		mut.unlock();
		try
		{
			cSocket->Send(result);
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			return;
		}
	}
}