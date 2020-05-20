#ifndef __Weapon_H__
#define __Weapon_H__

#include "cocos2d.h"
#include "Entity\Entity.h"
USING_NS_CC;

class Weapon :public Entity
{
public:
	Weapon();
	~Weapon();
	CREATE_FUNC(Weapon);
	virtual bool init();
public:
	virtual void attack(Point pos);
	int getPowerCost()const;
	int getRange()const;
protected:
	int m_power_cost;
	int m_range;
};
#endif