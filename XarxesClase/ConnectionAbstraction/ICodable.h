#pragma once
#include <vector>
#include <list>
#include "Packet.h"

class ICodable
{
public:
	virtual void Code(sf::Packet& packet) = 0;
	virtual void Decode(sf::Packet& packet) = 0;
};

inline sf::Packet& operator <<(sf::Packet& packet, ICodable& codable)
{
	codable.Code(packet);
	return packet;
}

inline sf::Packet& operator >>(sf::Packet& packet, ICodable& codable)
{
	codable.Decode(packet);
	return packet;
}

//Nom�s validar la template si T �s filla de ICodable (valida el tipus)
//Saber 100% segur que tots els valors amb qu� treballem dins seran ICodable
//D'aquesta manera, T tindr� Code i Decode
template<class T>
class CPVector : public std::vector<T*>, public ICodable
{
	static_assert(std::is_base_of<ICodable, T>::value, "T must inherit from ICodable");

	virtual void Code (sf::Packet& packet) override
	{
		sf::Uint64 count = this->size();
		packet << count;

		for (T* item : *this)
		{
			ICodable* codable = dynamic_cast<ICodable*>(item);
			codable->Code(packet);
		}
	}

	virtual void Decode (sf::Packet& packet) override
	{
		sf::Uint64 count;
		packet >> count;

		for (sf::Uint64 i = 0; i < count; i++)
		{
			T* item = new T();
			dynamic_cast<ICodable*>(item)->Decode(packet);
			this->push_back(item);
		}
	}
};

template<typename T, typename = typename std::enable_if<std::is_base_of<ICodable, T>::value>::type>
class CPList : public std::list<T*>, public ICodable
{
	virtual void Code(sf::Packet& packet) override
	{
		sf::Uint64 count = this->size();
		packet << count;

		for (T* item : *this)
		{
			ICodable* codable = dynamic_cast<ICodable*>(item);
			codable->Code(packet);
		}
	}

	virtual void Decode(sf::Packet& packet) override
	{
		sf::Uint64 count;
		packet >> count;

		for (sf::Uint64 i = 0; i < count; i++)
		{
			T* item = new T();
			reinterpret_cast<ICodable*>(item)->Decode(packet);
			this->push_back(item);
		}
	}
};

///////////////////////////////////////////////////////////////////
//Example classes
class Job : public ICodable
{
public:
	std::string name;

	Job()
	{

	}

	~Job()
	{

	}

	virtual void Code(sf::Packet& packet) override
	{
		packet << name;
	}

	virtual void Decode(sf::Packet& packet) override
	{
		packet >> name;
	}
};

class Character : public ICodable
{
public:
	sf::Uint8 age;
	std::string name;
	float height;
	Job job;

	Character()
	{

	}

	~Character()
	{

	}

	virtual void Code(sf::Packet& packet) override
	{
		packet << age << name << height << job;
	}

	virtual void Decode(sf::Packet& packet) override
	{
		packet >> age >> name >> height >> job;
	}
};