#ifndef __Weapon_H__
#define __Weapon_H__

#include <string>
#include "cocos2d.h"
#include "Entity\Entity.h"
#include "Scene\AdventureMapScene.h"
#include <Entity\Weapons\Bullets\Bullet.h>
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
	virtual std::vector<Bullet*> getBullet() const;
	int getPowerCost()const;
	int getRange()const;
	void bindMap(AdventureMapLayer* map);
	void setCritRate(float crit_rate);
	virtual float getAttackSpeed()const;
	void resetPosition();
	virtual void flipped(bool status/*true for flipped and false for not flipped*/);
	virtual void setRotationByPos(Point pos);
	virtual void upgrade();
	virtual bool isCloseWeapon()const;
	const std::string& getWeaponName() { return m_weaponName; }
protected:
	int m_power_cost;
	int m_range;
	float m_crit_rate;
	float m_attack_speed;
	std::string m_weaponName = "none";
	AdventureMapLayer* m_map;
};
#endif