#pragma once
class AttributeComponent
{
public:
	//Leveling
	unsigned level;
	unsigned exp;
	unsigned expNext;
	unsigned attributePoints;


	//Attributes
	unsigned strength;
	unsigned vitality;
	unsigned dexterity;
	unsigned agility;
	unsigned intelligence;

	//Stats


	//Con/Des
	AttributeComponent();
	virtual ~AttributeComponent();

	//Functions
};

