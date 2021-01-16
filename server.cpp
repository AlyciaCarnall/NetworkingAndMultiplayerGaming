#include "util.hpp"
#include <SFML/Network.hpp>
#include <cstring>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <thread>

int main()
{
	sf::TcpListener listener;
	sf::UdpSocket uSocket;
	if (uSocket.bind(UPORT) != sf::Socket::Done)
	{
		std::cerr << "Error binding to UDP port\n";
		return 1;
	}

	//using a thread to be able to send and receive messages
	std::cout << "UDP bound to port: " << usocket.getLocalPort() << std::endl;
	std::thread([&uSocket]) {
		sf::Packet packet;
		sf::IpAddress address;
		unsigned short remote_port;

		//want to loop around forever and receive messages
		while(true)
		{	
			packet.clear();
			sf::Socket::Status status = uSocket.receive(packet, address, remote_port);
			//if we can receive messages
			if (status == sf::Socket::Done)
			{
				std::cout << "Received UDP message; replying with answer\n";
				packet.clear();
				packet << "Request granted";
				//then send back
				uSocket.send(packet, address, remote_port);
				std::cout << "UDP message sent back \n";
			}
			else
				std::cerr << "Receive status: " << status << std::endl;
		}
	}
}).detach();
unsigned int local_port = PORT;
if (listener.listen(localport) != sf::Socket::Done)
{
	std::cerr << "TCP Binding error" << std::endl;
	return 1;
}

std::cout << "TCP socket bound to port: " << listener.getLocalPort() << std::endl;
sf::TcpSocket tScocket;

//only accept one client here
if (listener.accept(tScocket) != sf::Socket::Done)
{
	std::cerr << "Accepting problems" << std::endl;
	return 1;
}
//receive and send messages
sf::Packet packet;
while (true)
{
	//everytime we receive a message to a packet
	packet.clear();
	if (tsocket.receive(packet) != sf::Socket::Done)
	{
		std::cerr << "Receiveing problems" << std::endl;
		return 1;
	}
	//sending the same packet to the same socket
	//not even looking at the message 
	std::cout << "Received TCP message \n";
	if (tSocket.send(packet) != sf::Socket::Done)
	{
		std::cerr << "Problems sending" << std::endl;
		return1;
	}
	std::cout << "Echoed TCP message\n";
}
