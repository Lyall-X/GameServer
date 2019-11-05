#pragma once


class Session;
class Player : public Entity
{
public:
	uint32		playerId;
	Session*	ssn;

	void setSession(Session* s) { ssn = s; }
	Session* getSession() { return ssn; }
	void setPlayerId(uint32 plrId) { playerId = plrId; }
	uint32 getPlayerId() { return playerId; }
protected:
private:
};