#include "network.hpp"
#include "receiver.hpp"
#include <iostream>

//constructor
Network::Network(Queue<Message>& q) : queue(q)
{
	//UDP message to find server 
	sf::UdpSocket udpSocket;
	sf::Packet packet;
	packet << "Are you a server ?";
	unsigned short port = UPORT;
	std::cout << "Sending UPD message discovery: " << port << std::endl;
	auto sendUDP = [&] {return udpSocket.send(packet, sf::Address::Broadcast, UPORT); };
	net_run(sendUDP, "UDP Send");
	std::cout << "Sent UDP message \n";

	//Receive UDP message from server
	packet.clear();
	sf::IpAddress address;
	auto recvUDP = [&] {return udpSocket.receive(packet, address, port);  };
	net_run(recvUDP, "UDP Receive");
	std::cout << "Received UDP message back from server\n";

	//Connect to the address
	socket = std::make_shared<sf::TcpSocket>();
	auto connect = [&] {return socket->connect(address, port);  };
	net_run(connect, "connect");
	std::cout << "Client::main connected\n";

	//Register with the server first
	MsgReg m_reg;
	m_reg.msgtype = MsgType::Register;
	//Gives the player a random ID
	m_reg.id = rand() % 16384;
	//Give the player a name
	m_reg.username = "Gerald";
	sf::Packet m_reg_packet;
	me_reg_packet << m_reg;
	auto send1 = [&] {return socket->send(m_reg_packet);  };
	net_run(send1, "send");

	//Shouldn't do anything with the echo server, should just get our message back
	std::cout << "Sent register\n";

	//Start receiver thread
	Receiver r(socket, queue);
	std::thread(&Receiver::recv_loop, std::move(r)).detach();

}

Network::~Network()
{
}

void Network::send(const Msg& msg)
{
	sf::Packet packet;
	packet << msg;
	socket->send(packet);
}
