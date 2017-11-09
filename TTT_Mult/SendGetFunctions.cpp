#include "stdafx.h"
#include "GameInclude/MultBase.h"



bool MultBase::SendBool(bool _Bool)
{
	int RetnCheck = send(Connection, (char*)&_Bool, sizeof(bool), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::GetBool(bool& _Bool)
{
	int RetnCheck = recv(Connection, (char*)&_Bool, sizeof(bool), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::SendInt(int _Int)
{
	int RetnCheck = send(Connection, (char*)&_Int, sizeof(int), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::GetInt(int &_Int)
{
	int RetnCheck = recv(Connection, (char*)&_Int, sizeof(int), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::SendPacket(PacketType _Packet)
{
	int RetnCheck = send(Connection, (char*)&_Packet, sizeof(PacketType), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::GetPacket(PacketType& _Packet)
{
	int RetnCheck = recv(Connection, (char*)&_Packet, sizeof(PacketType), NULL);
	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::SendString(string _String)
{
	if (!SendInt(_String.size()))
		return false;

	int RetnCheck = send(Connection, _String.c_str(), _String.size(), NULL);

	return RetnCheck != SOCKET_ERROR;
}

bool MultBase::GetString(string& _String)
{
	int BufferLength;
	if (!GetInt(BufferLength))
		return false;

	char* buffer = new char[BufferLength + 1];
	buffer[BufferLength] = '\0';

	int RetnCheck = recv(Connection, buffer, BufferLength, NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;

	_String = buffer;
	delete[] buffer;
	return true;
}