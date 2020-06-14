#include "Entity\Red\Red.h"
#define RANDOMRANGE 10


bool Red::init()
{
	bindSprite(Sprite::create("Red.png"), 1.0f, 1.0f);
	m_RedValue = 3;
	m_is_used = false;
	return true;
}

int Red::getRedValue()const
{
	return m_RedValue;
}

void Red::disappear()
{
	m_sprite->setVisible(false);
	m_is_used = true;
}

//��������һֱ������
void Red::setRandomPosition()
{
	auto curPosition = getPosition();

	auto ranF1 = CCRANDOM_0_1();
	auto ranF2 = CCRANDOM_0_1();
	auto xDif = ranF1 * RANDOMRANGE * 2 - RANDOMRANGE;
	auto yDif = ranF2 * RANDOMRANGE * 2 - RANDOMRANGE;
	auto ranPosition = ccp(curPosition.x + xDif, curPosition.y + yDif);
	//����һ��ƫ�Χ
	this->setPosition(ranPosition);
}

bool Red::isUsed()const
{
	return m_is_used;
}