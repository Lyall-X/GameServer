#pragma once

class BinaryStream;
class Vector2;
class Vector3;
class Vector4;

bool operator<<(BinaryStream& stream, const Vector2& value);
bool operator>>(BinaryStream& stream, Vector2& value);

bool operator<<(BinaryStream& stream, const Vector3& value);
bool operator>>(BinaryStream& stream, Vector3& value);
