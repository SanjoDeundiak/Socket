#include "MyClient.h"
#include <iostream>

int main()
{
	std::string address;
	std::cout << "Enter address: ";
	std::cin >> address;
	MyClient client(address);
	client.start();
}