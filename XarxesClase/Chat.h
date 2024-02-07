#pragma once
#include <iostream>
#include <mutex>
#include <SFML/Network.hpp>
#include <vector>

class Chat
{
private:

	std::vector<std::string> _messages;
	std::mutex _messagesMutex;

	sf::IpAddress _serverAddress;

	Chat(){};

	void ShowMessage(std::string message);
	void ShowWarning(std::string message);
	void ShowError(std::string message);

	void ListenClientsConnections(unsigned short port);

public:
	static Chat* Server(unsigned short port);
	static Chat* Client(std::string ip, unsigned short port);
};