#include "MyServer.h"
#include <iostream>

int main()
{
	MyServer server("123.xml");
	try
	{
		server.start(PORT);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
		std::cin.get();
		std::cin.get();
		return 1;
	}
	
	std::cin.get();
	std::cin.get();
}