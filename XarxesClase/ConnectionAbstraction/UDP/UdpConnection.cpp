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
	std::thread* subscribeThread = new std::thread(&UdpConnection::Subscribe, this, key, onReceivePacket);
	subscribeThread->detach();
}

void UdpConnection::SendCritical(UdpPacket::PacketKey key, ICodable& codable, OnReceiveCriticalResponsePacket onResponse)
{
	UdpPacket::CriticalPacketId outId;
	UdpPacket criticalPacket = UdpPacket::Critical(key, codable, outId); //This out Id Manage, is not correct

	UdpCriticalPerserverator perseverator = UdpCriticalPerserverator(criticalPacket, onResponse);
	_criticalResponsesSubscriptionsMutex.lock();
	_criticalResponsesSubscriptions[outId] = perseverator;
	_criticalResponsesSubscriptionsMutex.unlock();

	_delegate->SendImmediately(_address, criticalPacket);
}

void UdpConnection::SubscribeOnCritical(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket)
{
	_criticalSubscriptionsMutex.lock();
	_criticalSubscriptions[key] = onReceiveCriticalPacket;
	_criticalSubscriptionsMutex.unlock();
}

void UdpConnection::SubscribeOnCriticalAsync(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket)
{
	std::thread* subscribeThread = new std::thread(&UdpConnection::SubscribeOnCritical, this, key, onReceiveCriticalPacket);
	subscribeThread->detach();
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
	UdpPacket::PacketKey key;
	UdpPacket::CriticalPacketId id;
	packet >> key >> id;

	std::map<UdpPacket::PacketKey, OnReceiveCriticalPacket>::iterator it;
	_criticalSubscriptionsMutex.lock();

	it = _criticalSubscriptions.find(key);
	if (it != _criticalSubscriptions.end())
	{
		ICodable* codableResponse = it->second(packet);
		UdpPacket criticalResponse = UdpPacket::CriticalResponse(key, id, *codableResponse);
		_delegate->SendImmediately(GetAddress(), criticalResponse);
	}
	_criticalSubscriptionsMutex.unlock();
}

void UdpConnection::ManageCriticalResponse(UdpPacket packet)
{
	UdpPacket::PacketKey key;
	UdpPacket::CriticalPacketId id;
	packet >> key >> id;

	std::map<UdpPacket::CriticalPacketId, UdpCriticalPerserverator>::iterator it;
	_criticalResponsesSubscriptionsMutex.lock();

	it = _criticalResponsesSubscriptions.find(id);
	if (it != _criticalResponsesSubscriptions.end())
	{
		it->second.onReceiveCriticalResponsePacket(packet);
		_criticalResponsesSubscriptions.erase(it);
	}

	_criticalResponsesSubscriptionsMutex.unlock();
}

void UdpConnection::ManageAccumulated(UdpPacket packet)
{
	CPVector<UdpPacket> packets;
	packet >> packets;

	for (UdpPacket* nextPacket : packets)
	{
		this->ManageReceivedPacket(*nextPacket);
	}

	for (UdpPacket* nextPacket : packets)
	{
		delete nextPacket;
	}
}
