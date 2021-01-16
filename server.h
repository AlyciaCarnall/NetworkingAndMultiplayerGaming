#include <SFML/Network.hpp>
#include <memory>

//A client is a pair of ID and TcpSocket.
typedef std::pair<sf::Uint32, std::shared_ptr<sf::TcpSocket>> Client;

class Server {
public:
	Server();
	void operator()();
private:
	void runUDP();
};