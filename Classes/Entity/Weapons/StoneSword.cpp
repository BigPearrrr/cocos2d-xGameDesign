#include "Entity/Weapons/StoneSword.h"

bool StoneSword::init()
{
	m_damage = 8;
	m_range = 25;
	m_attack_speed = 0.18f;
	m_crit_rate = 0.05f;
	m_weaponName = "StoneSword";
	bindSprite(Sprite::create("StoneSword.png"), 0.16f, 0.16f);
	m_sprite->setAnchorPoint(Vec2(0, 0));
	return true;
}

void StoneSword::upgrade()
{
	isUpgrate = true;
	m_damage = 15;
	m_range = 30;
	m_attack_speed = 0.15f;
	m_crit_rate = 0.1f;
	//bindSprite(Sprite::create("StoneSword.png"), 0.18f, 0.18f);
	auto upgrade_effect = ParticleFlower::create();
	upgrade_effect->setEmitterMode(ParticleSystem::Mode::RADIUS);
	upgrade_effect->setPositionType(ParticleSystem::PositionType::RELATIVE);
	upgrade_effect->setAutoRemoveOnFinish(true);
	upgrade_effect->setTotalParticles(100);
	upgrade_effect->setPosition(getSprite()->getContentSize().width / 2, getSprite()->getContentSize().height / 2);
	upgrade_effect->setDuration(-1);
	upgrade_effect->setStartRadius(50.0f);
	upgrade_effect->setStartRadiusVar(25.0f);
	upgrade_effect->setEndRadius(50.0f);
	getSprite()->addChild(upgrade_effect);
}