#include "stdafx.h"
#include "SkillComponent.h"

SkillComponent::SkillComponent()
{
	this->skills.push_back(Skill(HEALTH));
	this->skills.push_back(Skill(ATTACK));
	this->skills.push_back(Skill(ACCURACY));
	this->skills.push_back(Skill(ENDURANCE));
}

SkillComponent::~SkillComponent()
{
}

const int SkillComponent::getSkill(const int skill) const
{
	if (skills.empty() || skill < 0 || skill >= this->skills.size())
		throw("ERROR:SKILLCOMPONENT::GETSKILL::SKILL DOES NOT EXIST: " + skill);
	else
		return this->skills[skill].getLevel();
}

const void SkillComponent::gainExp(const int skill, const int exp)
{
	if (skills.empty() || skill < 0 || skill >= this->skills.size())
		throw("ERROR:SKILLCOMPONENT::GAINEXP::SKILL DOES NOT EXIST: " + skill);
	else
	{
		this->skills[skill].gainExp(exp);
	}
}