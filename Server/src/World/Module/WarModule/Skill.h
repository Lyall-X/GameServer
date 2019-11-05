#pragma once

enum SkillType
{
	ST_None,
	ST_Target,
	ST_Monomer,
	ST_Range,
};

class Skill
{
public:
	Skill();
	~Skill();
	void setSkillId(int32 skillId) { mSkillId = skillId; }
	int32 getSkillId() { return mSkillId; }
protected:
	int32 mSkillId;
};