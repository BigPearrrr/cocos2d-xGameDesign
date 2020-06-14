#include "Controller/Move/PlayerController.h"

bool PlayerController::init()
{
	m_iXSpeed = 0;
	m_iYSpeed = 0;
	this->scheduleUpdate();
	return true;
}

void PlayerController::onEnter()
{
	Node::onEnter();
	registeControllerEvent();
}

void PlayerController::update(float dt)
{
	if (m_controllerListener == NULL)
		return;
	Point curPos = m_controllerListener->getTagPosition();

	curPos.x += m_iXSpeed;
	curPos.y += m_iYSpeed;

	isEncounterBarriers(curPos.x, curPos.y);
	m_controllerListener->setTagPosition(curPos.x, curPos.y);

	playerOperate();
}


void PlayerController::setPlayer(Player* player)
{
	m_player = player;
}

void PlayerController::registeControllerEvent()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* keyListener = EventListenerKeyboard::create();
	//����һ���¼����������������¼�
	keyListener->onKeyPressed = CC_CALLBACK_2(PlayerController::onKeyPressed, this);
	//���̱�����ʱ��Ӧ
	keyListener->onKeyReleased = CC_CALLBACK_2(PlayerController::onKeyReleased, this);
	//���̰���������ʱ��Ӧ
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);

}

void PlayerController::animateOperate()
{
	if (getiXSpeed() != 0 || getiYSpeed() != 0)
	{
		if (m_standAnimate != NULL)
		{
			stopAction(m_standAnimate);
			m_standAnimate = NULL;
		}
		if (m_walkAnimate == NULL)
		{
			auto animate = m_player->walk();
			m_walkAnimate = animate;
		}
	}
	else
	{
		if (m_walkAnimate != NULL)
		{
			stopAction(m_walkAnimate);
			m_walkAnimate = NULL;
		}
		if (m_standAnimate == NULL)
		{
			auto animate = m_player->stand();
			m_standAnimate = animate;
		}
	}
}

void PlayerController::isEncounterBarriers(const int x, const int y)
{
	Size spriteSize = m_player->getSprite()->getContentSize();
	Point dstPos = Point(x + m_player->isPositiveOrNegative(m_player->getController()->getiXSpeed()) * 20 +
		m_player->getSkillDirectionX() * 5, y + m_player->isPositiveOrNegative(m_player->
			getController()->getiYSpeed()) * 20 + m_player->getSkillDirectionY() * 5);

	/* ��õ�ǰ����ǰ�������ڵ�ͼ�еĸ���λ�� */
	Point tiledPos = m_player->tileCoordForPosition(Point(dstPos.x, dstPos.y));

	// ��ȡ��ͼ���ӵ�Ψһ��ʶ 
	int tiledGid = (m_player->getMap()->getCollidable())->getTileGIDAt(tiledPos);

	// ��Ϊ0���������������� 
	if (tiledGid != 0)
	{
		/*
		��ȡ�õ�ͼ���ӵ��������ԣ�Ŀǰ����ֻ��һ��Collidable����
		����������meta��ģ���ͬʱҲ������������ͼ�ģ������ڻ�ȡ���ӵ���������
		ʱ��ͨ������Ψһ��ʶ�ڵ�ͼ��ȡ��
		*/
		Value properties = (m_player->getMap()->getMap())->getPropertiesForGID(tiledGid);

		ValueMap propertiesMap = properties.asValueMap();

		if (propertiesMap.find("Collidable") != propertiesMap.end())
		{
			/* ȡ�ø��ӵ�Collidable����ֵ */
			Value prop = propertiesMap.at("Collidable");
			/* �ж�Collidable�����Ƿ�Ϊtrue������ǣ���������ƶ� */
			if (prop.asString().compare("true") == 0)
			{
				auto moveBy = MoveBy::create(0.01f, Point(-(m_player->isPositiveOrNegative
				(m_player->getController()->getiXSpeed()) + m_player->getSkillDirectionX()) * 5,
					-(m_player->isPositiveOrNegative(m_player->getController()->
						getiYSpeed()) + m_player->getSkillDirectionY()) * 5));

				/* ִ�ж�������ײ���ϰ���ʱ�ķ���Ч�� */
				if (m_rangerSkill != NULL)
				{
					m_player->stopAction(m_rangerSkill);
					m_rangerSkill = NULL;
				}
				m_player->runAction(moveBy);
			}
		}
	}
}

void PlayerController::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_W)
	{
		//CCLOG("��W���Ϸ����");
		this->setiYSpeed(4);
		//m_player->resetWeaponPosition(m_player->getLeftSide());
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_A)
	{
		//CCLOG("��A�������");
		this->setiXSpeed(-4);
		m_player->setRightToward();
		//m_player->resetWeaponPosition(true);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_D)
	{
		//CCLOG("��D���ҷ����");
		this->setiXSpeed(4);
		m_player->setLeftToward();
		//m_player->resetWeaponPosition(false);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_S)
	{
		//CCLOG("��S���·����");
		this->setiYSpeed(-4);
		//m_player->resetWeaponPosition(m_player->getLeftSide());
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_SPACE)
	{
		//CCLOG("���ո񣺷ż���");
		m_player->skill();
		if (m_isRanger && (m_player->getiNowCD() == 0))
		{
			rangerOpearte();
		}
		return;
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_Q)
	{
		m_player->changeWeapon();
	}

	animateOperate();
}


void PlayerController::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_A || keycode == EventKeyboard::KeyCode::KEY_D)
	{
		this->setiXSpeed(0);
	}
	else if(keycode == EventKeyboard::KeyCode::KEY_W || keycode == EventKeyboard::KeyCode::KEY_S)
	{
		this->setiYSpeed(0);
	}
	if (m_walkAnimate != NULL && m_iXSpeed == 0 && m_iYSpeed == 0)
	{
		stopAction(m_walkAnimate);
		m_walkAnimate = NULL;
	}
	if (m_standAnimate == NULL)
	{
		auto animate = m_player->stand();
		m_standAnimate = animate;
	}
	m_isChangWeapon = false;
}

void PlayerController::playerOperate()const
{
	m_player->setiNowCD((m_player->getiNowCD() + 1));
    m_player->setArmorCd();
	if (m_player->getIsInSkill())
	{
		m_player->setiNowSkillDuration(m_player->getiNowSkillDuration() + 1);
		if (m_player->getiNowSkillDuration() == m_player->getiTotalSkillDuration())
			m_player->skillEnd();
	}
}

void PlayerController::rangerOpearte()
{
	auto moveBy = MoveBy::create(0.1, m_player->getSkillDirection());
	auto callFunc = CallFunc::create([&]()
	{
		m_rangerSkill = NULL;
		m_player->setSkillDirectionX(0);
		m_player->setSkillDirectionY(0);
	});
	auto rangerAction = Sequence::create(moveBy, callFunc, NULL);
	m_rangerSkill = rangerAction;
	m_player->runAction(rangerAction);
}

