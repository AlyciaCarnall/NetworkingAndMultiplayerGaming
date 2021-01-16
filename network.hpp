#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "message.hpp"
#include "queue.hpp"
#include "util.hpp"
#include <SFML/Network.hpp>

class Network {
	public:
		Network(Queue<Message>& q);
		~Network();
		void send(const Msg& msg);

	private:
		Queue<Message>& queue;
		std::shared_ptr<sf::TcpSocket> socket;
};
#endif