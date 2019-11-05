#pragma once

class War
{
public:
	War();
	~War();
	void setEntity(Entity* ent) { mEnt = ent; }
	Entity* getEntity() { return mEnt; }
	void setTarget(Entity* tar) { mTarget = tar; }
	Entity* getTarget() { return mTarget; }
	void setCurrentSkill(Skill* skill) { mCurrentSkill = skill; }
	Skill* getCurrentSkill() { return mCurrentSkill; }
	Skill* getSkill(int32 index);

	bool useSkill(int32 index);
	bool cancelSkill();
protected:
	Entity* mEnt;
	Entity* mTarget;
	std::vector<Skill*> mSkills;
	Skill* mCurrentSkill;
};