#pragma once
#include <functional>
#include <mutex>
#include <map>

#include "UdpAdress.h"
#include "UdpPacket.h"

class UdpSendPacketDelegate
{
public:

	virtual void SendAcumulating(UdpAddress address, UdpPacket packet) = 0;
	virtual void SendImmediately(UdpAddress address, UdpPacket packet) = 0;
};

class UdpConnection {
public:
	typedef std::function<void(UdpPacket packet)> OnReceivePacket;
	typedef std::function<UdpPacket(UdpPacket packet)> OnReceiveCriticalPacket;
	typedef std::function<void(UdpPacket packet)> OnReceiveCriticalResponsePacket;

public:

	UdpConnection(UdpSendPacketDelegate* delegate, UdpAddress address);
	~UdpConnection();

	UdpAddress GetAddress();

	void Send(UdpPacket::PacketKey key, ICodable& codable);
	void Subscribe(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket);
	void SubscribeAsync(UdpPacket::PacketKey key, OnReceivePacket onReceivePacket);

	//void SendCritical(UdpPacket::PacketKey, ICodable& codable, OnReceiveCriticalResponsePacket onResponse);
	//void SubscribeOnCritical(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket);
	//void SubscribeOnCriticalAsync(UdpPacket::PacketKey key, OnReceiveCriticalPacket onReceiveCriticalPacket);

	void ManageReceivedPacket(UdpPacket packet);

private:

	UdpSendPacketDelegate* _delegate;
	UdpAddress _address;

	std::map<UdpPacket::PacketKey, OnReceivePacket> _subscriptions;
	std::mutex _subscriptionsMutex;

	void ManageNormal(UdpPacket packet);
	void ManageCritical(UdpPacket packet);
	void ManageCriticalResponse(UdpPacket packet);
	void ManageAccumulated(UdpPacket packet);



};