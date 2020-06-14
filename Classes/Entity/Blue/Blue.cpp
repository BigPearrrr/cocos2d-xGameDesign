#include "Entity\Blue\Blue.h"
#define RANDOMRANGE 10


bool Blue::init()
{
	bindSprite(Sprite::create("Blue.png"), 1.0f, 1.0f);
	m_blueValue = 8;
	m_is_used = false;
	return true;
}

int Blue::getBlueValue()const
{
	return m_blueValue;
}

void Blue::disappear()
{
	m_sprite->setVisible(false);
	m_is_used = true;
}

//��������һֱ������
void Blue::setRandomPosition()
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

bool Blue::isUsed()const
{
	return m_is_used;
}
