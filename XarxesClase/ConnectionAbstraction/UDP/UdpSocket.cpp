#include "UdpSocket.h"
#include "../../ConsoleControl.h"

class ConnectionHandShake: public ICodable
{
public:

	UdpAddress address;
	bool isHello;

	ConnectionHandShake() {};
	ConnectionHandShake(UdpAddress address, bool isHello)
	{
		this->address = address;
		this->isHello = isHello;
	};
	~ConnectionHandShake() {};

	virtual void Code(sf::Packet& packet) override
	{
		packet << address << isHello;
	}
	virtual void Decode(sf::Packet& packet) override
	{
		packet >> address >> isHello;
	}


private:
};

UdpSocket::UdpSocket(UdpAddress::PortNumber port, OnConnectionEnter onConnectionEnter)
{
	_onConnectionEnter = onConnectionEnter;

	sf::IpAddress ipAddress = sf::IpAddress::getLocalAddress();
	_address = UdpAddress(ipAddress.toString(), port);

	if (bind(port) != sf::Socket::Done)
	{

	}
	else
	{
		std::thread* loopThread = new std::thread(&UdpSocket::ReceiveLoop, this);
		loopThread->detach();
	}
}

UdpSocket::~UdpSocket()
{
}

void UdpSocket::ConnectTo(UdpAddress address)
{
	ConnectionHandShake helloHandShake = ConnectionHandShake(GetAddress(), true);
	SendImmediately(address, UdpPacket::Normal(0, helloHandShake));
}

UdpAddress UdpSocket::GetAddress()
{
	return _address;
}

void UdpSocket::ReceiveLoop()
{
	_isRunning = true;
	bool isRunning = true;

	do
	{
		UdpPacket packet;
		sf::IpAddress ip;
		UdpAddress::PortNumber port;

		sf::Socket::Status status = this->receive(packet, ip, port);

		UdpAddress address = UdpAddress(ip.toString(), port);

		switch (status)
		{
		case sf::Socket::Done:
			std::cout << "Packet Received Done From: " << address.Tostring() << std::endl;
			ManageReceivedPacketDone(packet, address);
			break;
		case sf::Socket::NotReady:
			std::cout << "Packet Received NotReady From: " << address.Tostring() << std::endl;
			break;
		case sf::Socket::Partial:
			std::cout << "Packet Received Partial From: " << address.Tostring() << std::endl;
			break;
		case sf::Socket::Disconnected:
			std::cout << "Packet Received Disconnected From: " << address.Tostring() << std::endl;
			break;
		case sf::Socket::Error:
			std::cout << "Packet Received Error From: " << address.Tostring() << std::endl;
			break;
		default:
			break;
		}

		_isRunningMutex.lock();
		isRunning = _isRunning;
		_isRunningMutex.unlock();
	} while (isRunning);
}

void UdpSocket::ManageReceivedPacketDone(UdpPacket packet, UdpAddress address)
{
	std::map<std::string, UdpConnection*>::iterator it;
	std::string addressKey = address.Tostring();
	it = _connectionsMap.find(addressKey);

	if (it != _connectionsMap.end())
	{
		//Connection Exists
		it->second->ManageReceivedPacket(packet);
	}
	else
	{
		//Connection Is New

		sf::Uint8 intType;
		UdpPacket::PacketKey key;
		ConnectionHandShake handShake;

		packet >> intType >> key >> handShake;

		if (handShake.address.Tostring() == addressKey)
		{
			UdpConnection* newConnection = new UdpConnection(this, address);
			_connectionsMap[addressKey] = newConnection;
			_onConnectionEnter(newConnection);

			if (handShake.isHello)
			{
				ConnectionHandShake welcomeHandShake = ConnectionHandShake(GetAddress(), false);
				SendImmediately(address, UdpPacket::Normal(0, welcomeHandShake));

				//TODO Convert to Critical
			}
		}

		
	}
}

void UdpSocket::SendAcumulating(UdpAddress address, UdpPacket packet)
{
	//TODO
}

void UdpSocket::SendImmediately(UdpAddress address, UdpPacket packet)
{
	send(packet, address.ip, address.port);
}
