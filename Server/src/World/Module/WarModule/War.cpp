#include "stdafx.h"

War::War():
mEnt(NULL),
mTarget(NULL),
mCurrentSkill(NULL)
{
	Skill* skill = new Skill;
	skill->setSkillId(1);
	mSkills.push_back(skill);
}

War::~War()
{
	for (auto skill: mSkills)
		delete skill;
	mSkills.clear();
}

Skill* War::getSkill(int32 index)
{
	if (index < 0 || index > mSkills.size())
		return NULL;
	return mSkills[index];
}

bool War::useSkill(int32 index)
{
	Skill* skill = getSkill(index);
	if (skill == NULL)
		return false;
	setCurrentSkill(skill);
	mEnt->ChangeStatus(ES_ATTACT);
	return true;
}

bool War::cancelSkill()
{
	setCurrentSkill(NULL);
	return true;
}
