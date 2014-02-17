#include "ClientSocket.h"
#include <strsafe.h>
#include <sstream>

std::string ClientSocket::Recv()
{	
	char buff[RcvBuff + 1];
	std::stringstream result;
	int iResult;
	do 
	{
		iResult = recv(m_socket, buff, RcvBuff, NULL);
        if (iResult > 0)
		{
			if (buff[iResult - 1] == '\0')
			{				
				result << buff;
				return result.str();
			}
			else
			{
				buff[iResult] = '\0';
				result << buff;
			}			
		}		
        else if (iResult < 0)         
			throw std::runtime_error("Connection terminated");
    } 
	while( iResult > 0 );
	if (iResult == 0)
		return "";
	return result.str();
}

std::string ClientSocket::conFrom() const
{
	return m_conFrom;
}

void ClientSocket::Send(std::string msg)
{
	int iResult = send(m_socket, msg.c_str(), msg.length(), 0);
    if (iResult == SOCKET_ERROR) 			
		throw std::runtime_error("send failed with error");            
	iResult = send(m_socket, "\0", 1, 0);
    if (iResult == SOCKET_ERROR) 			
		throw std::runtime_error("send failed with error");            
}

ClientSocket::~ClientSocket()
{
	shutdown(m_socket, SD_SEND);	
}