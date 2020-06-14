#include "Bonus/Shop.h"
#include "Entity/Weapons/CandyGun.h"
#include "Entity/Weapons/RPG.h"
#include "Entity/Weapons/Shotgun.h"
Shop::Shop()
{
	m_interaction = 1;
	m_flowWord = FlowWord::create();
	this->addChild(m_flowWord);
	this->bindSprite(Sprite::create("cat.png"));
}

Shop::~Shop()
{
}

void Shop::bindSprite(Sprite* shopSprite)
{
	m_shop = shopSprite;
	this->addChild(m_shop);
}

void Shop::setInteractionNum(int num)
{
	m_interaction = num;
}

void Shop::setWeapon(int num)
{
	if (num == 1)
	{
		m_weapon = RPG::create();
	}
	else if (num == 2)
	{
		m_weapon = Shotgun::create();
	}
	else
	{
		m_weapon = CandyGun::create();
	}
}

void Shop::showFlowWordFirstMeet()
{
	m_flowWord->showShopWord("��λ��ʿ,��Ҫ����20��ҹ���һ������������?");
}

void Shop::showFlowWordLackMoney()
{
	m_flowWord->showShopWord("���Ľ�Ҳ���,�´�����!");
}

void Shop::showFlowWordEnoughMoney()
{
	m_flowWord->showShopWord("лл�ݹ�,�������ǿ��!");
}
