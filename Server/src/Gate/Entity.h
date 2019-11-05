#pragma once


class Entity
{
public:
	uint32	unitId;
	void setUnitId(uint32 uId) { unitId = uId; }
	uint32 getEntityId() { return unitId; }
protected:
private:
};