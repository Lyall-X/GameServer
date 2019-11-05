#include "Shared.hpp"

SocketAngent::SocketAngent() :
mSocketId(0),
mSocket(NULL)
{
}

SocketAngent::~SocketAngent()
{
	SAFE_SOCKET(mSocketId);
}
