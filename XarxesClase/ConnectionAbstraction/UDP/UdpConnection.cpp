#include "UdpConnection.h"

UdpConnection::UdpConnection(UdpSendPacketDelegate* delegate, UdpAddress address)
{
	_delegate = delegate;
	_address = address;
}

UdpConnection::~UdpConnection()
{

}

UdpAddress UdpConnection::GetAddress()
{
	return _address;
}

void UdpConnection::Send(UdpPacket::PacketKey key, ICodable& codable)
{
	UdpPacket packet = UdpPacket::Normal(key, codable);

	_delegate->SendAcumulating(_address, packet);
}

void UdpConnection::Subscribe(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket)
{
	_subscriptionsMutex.lock();
	_subscriptions[key] = onReceivePacket;
	_subscriptionsMutex.unlock();
}

void UdpConnection::SubscribeAsync(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket)
{
	std::thread* subscriveThread = new std::thread(&UdpConnection::Subscribe, this, key, onReceivePacket);
	subscriveThread->detach();
}

void UdpConnection::ManageReceivedPacket(UdpPacket packet)
{
	UdpPacket::PacketType type;
	sf::Uint8 intType;

	packet >> intType;
	type = static_cast<UdpPacket::PacketType>(intType);

	switch (type)
	{
	case UdpPacket::NORMAL:
		ManageNormal(packet);
		break;
	case UdpPacket::CRITICAL:
		ManageCritical(packet);
		break;
	case UdpPacket::CRITICALRESPONSE:
		ManageCriticalResponse(packet);
		break;
	case UdpPacket::ACUMULATED:
		ManageAccumulated(packet);	
		break;
	default:
		break;
	}
}

void UdpConnection::ManageNormal(UdpPacket packet)
{
	UdpPacket::PacketKey key;
	packet >> key;

	std::map<UdpPacket::PacketKey, OnReceivePacket>::iterator it;
	_subscriptionsMutex.lock();

	it = _subscriptions.find(key);
	if (it != _subscriptions.end())
	{
		it->second(packet);
	}

	_subscriptionsMutex.unlock();
}

void UdpConnection::ManageCritical(UdpPacket packet)
{
}

void UdpConnection::ManageCriticalResponse(UdpPacket packet)
{
}

void UdpConnection::ManageAccumulated(UdpPacket packet)
{
}
