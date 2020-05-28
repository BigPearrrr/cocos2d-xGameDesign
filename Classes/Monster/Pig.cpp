#include "Pig.h"

bool Pig::init()
{
	setMonsterSpeed(0.2);
	setHp(150);
	setResTrack("Pig.png");
	this->bindSprite(Sprite::create(this->getResTrack()),0.8f,0.8f);
	auto sword = MonsterSword::create();
	sword->setPosition(this->getPosition());
	//gun->setContentSize(Size(0.1, 0.1));
	setMonsterWeapon(sword);
	this->addChild(sword, 1);
	m_isAlive = true;
	return true;
}