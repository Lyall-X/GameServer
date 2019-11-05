#include "Shared.hpp"

bool operator<<(BinaryStream& stream, const Vector2& value)
{
	CHECK_RETURN(stream << value.x, false);
	CHECK_RETURN(stream << value.y, false);
	return true;
}

bool operator>>(BinaryStream& stream, Vector2& value)
{
	CHECK_RETURN(stream >> value.x, false);
	CHECK_RETURN(stream >> value.y, false);
	return true;
}

bool operator<<(BinaryStream& stream, const Vector3& value)
{
	CHECK_RETURN(stream << value.x, false);
	CHECK_RETURN(stream << value.y, false);
	CHECK_RETURN(stream << value.z, false);
	return true;
}

bool operator>>(BinaryStream& stream, Vector3& value)
{
	CHECK_RETURN(stream >> value.x, false);
	CHECK_RETURN(stream >> value.y, false);
	CHECK_RETURN(stream >> value.z, false);
	return true;
}
