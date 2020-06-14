#include "Monster\MonsterManager.h"
#include "Entity\Weapons\TrackWeapon.h"



void MonsterManager::bindMap(AdventureMapLayer* map)
{
	m_map = map;
}

void MonsterManager::bindPlayer(Entity* player)
{
	m_player = player;
	this->scheduleUpdate();
}

void MonsterManager::reviveAllMonsters()
{
	m_curCheckPoint = 1;
	m_fGameOver = 0;
	m_deathMonsNum = 0;
	for (auto monster : m_monsterList)
	{
		monster->resetPropoties();
		monster->setVisible(true);
		monster->getSprite()->setVisible(true);
	}
	createMonsterPos();
}

void MonsterManager::bindMapForWeapon()
{
	for (auto monster : m_monsterList)
		monster->getMonsterWeapon()->bindMap(m_map);
}

bool MonsterManager::init()
{
	m_curCheckPoint = 1;
	m_deathMonsNum = 0;
	m_fGameOver = 0;
	m_curRoom = ccp(-1, -1);
	return true;
}



void MonsterManager::createMonsterPos()
{//��������λ�ã�����ʾ����֮ǰ��ʾԤѡ��

	createRandomPos();
	showPreRec();
	auto callback2 = CallFunc::create(
		[this]() {
		hidePreRec();
	}
	);
	auto createAction = Sequence::create(DelayTime::create(2.0f), callback2, NULL);
	runAction(createAction);
}

void MonsterManager::createRandomPos() {
	int k = 0;
	//�������Ұ��
	for (int i = 0; i < m_monsterList.size(); i++)
	{
		auto randInt1 = rand() % 18 + 2;
		auto randInt2 = rand() % 18 + 2;

		auto monsterPos = 32 * ccp(randInt1, randInt2) + ccp(16.5, 0);

		auto worldTar = monsterPos + getPosition();
		Vec2 tarBlock = ccp(static_cast<int>(monsterPos.x) / 21, static_cast<int>(monsterPos.y) / 21);
		if (m_map->isBarrier(worldTar)
			|| m_monsPosMap[tarBlock])//�����ϰ�����ֱ��continue
		{
			i--;
			continue;
		}
		if (m_player)
		{
			if (m_player->getPosition() - worldTar < ccp(100, 100) &&
				m_player->getPosition() - worldTar > ccp(-100, -100))
			{//�Է�һ�����ͳ���
				i--;
				continue;
			}
		}

		m_monsPosMap[tarBlock] = 1;
		m_monsterList[k]->setPosition(monsterPos);

		k++;
	}

	bulkUpRandMons(this->m_bulkMonsterNum);//�������������
}

void MonsterManager::bulkUpRandMons(int totalNum)
{
	int totalBulkMonster = totalNum;
	std::set<int> bulkMap;
	int randMons;
	int monsNum = m_monsterList.size();
	while (1)
	{
		randMons = rand() % monsNum;
		if (bulkMap.count(randMons))
		{
			continue;
		}
		bulkMap.insert(randMons);

		m_monsterList[randMons]->bulkUp();

		totalNum--;
		if (totalNum <= 0)
		{
			break;
		}
	}
}

void MonsterManager::createMonstersWithGiantNum(int giantNum, int totalNum)
{
	//giantNumΪ0��û�о޴󻯵Ĺ���
	//giantNumΪ1���ϣ���giantNum���޴󻯵Ĺ���
	auto randVec = createRandomNums(4, totalNum - 4);
	this->m_bulkMonsterNum = giantNum;
	if (giantNum > totalNum - 4)
	{
		assert("too much giant monsters");
	}
	Pig* pig = NULL;
	Slime* slime = NULL;
	Sprite* sprite = NULL;
	ChiefOfTribe* chiefOfTribe = NULL;
	Duck* duck = NULL;
	//int k = 0;
	for (int i = 0; i < randVec[0] + 1; i++)
	{
		pig = Pig::create();
		pig->bindMap(m_map);
		pig->getMonsterWeapon()->bindMap(m_map);
		pig->bindMonsMgr(this);
		this->addChild(pig,1);
		m_monsterList.push_back(pig);
		m_shortMonsterList.push_back(pig);
	}

	for (int i = 0; i < randVec[1] + 1; i++)
	{
		duck = Duck::create();
		duck->bindMap(m_map);
		duck->getMonsterWeapon()->bindMap(m_map);
		duck->bindMonsMgr(this);
		this->addChild(duck,1);
		m_monsterList.push_back(duck);
		m_shortMonsterList.push_back(duck);
	}

	for (int i = 0; i < randVec[2] + 1; i++)
	{
		slime = Slime::create();
		this->addChild(slime,1);
		slime->bindMap(m_map);
		slime->getMonsterWeapon()->bindMap(m_map);
		slime->bindMonsMgr(this);
		m_monsterList.push_back(slime);
		m_longMonsterList.push_back(slime);
	}

	for (int i = 0; i < randVec[3] + 1; i++)
	{
		chiefOfTribe = ChiefOfTribe::create();
		this->addChild(chiefOfTribe,1);
		chiefOfTribe->bindMap(m_map);
		chiefOfTribe->getMonsterWeapon()->bindMap(m_map);
		chiefOfTribe->bindMonsMgr(this);
		m_monsterList.push_back(chiefOfTribe);
		m_longMonsterList.push_back(chiefOfTribe);
	}

}

void MonsterManager::createWoodWalls(int woodWallsNum)
{
	//�������Ұ��
	for (int i = 0; i < woodWallsNum; i++)
	{

		auto randInt1 = rand() % 15 + 3 ;
		auto randInt2 = rand() % 15 + 3 ;


		auto monsterPos = 32 * ccp(randInt1, randInt2) + ccp(16.5, 0);

		auto worldTar = monsterPos + getPosition();
		Vec2 tarBlock = ccp(static_cast<int>(monsterPos.x) / 21, static_cast<int>(monsterPos.y) / 21);
		if (m_map->isBarrier(worldTar)
			|| m_monsPosMap[tarBlock])//�����ϰ�����ֱ��continue
		{
			i--;
			continue;
		}


		m_monsPosMap[tarBlock] = 1;
		auto woodWall = WoodWall::create();
		addChild(woodWall,-1);
		woodWall->bindMonsMgr(this);
		woodWall->bindMap(m_map);
		woodWall->setPosition(monsterPos);
		//m_map->getCollidable()->setTileGID(89, GameData::getCoord()[5 * m_curRoom.x + m_curRoom.y] - Vec2(10 - randInt1, -11 + randInt2));
		m_map->getCollidable()->setTileGID(89, Vec2(0, -1) +
			m_map->tileCoordFromPosition(m_map->convertToNodeSpace(this->convertToWorldSpace(woodWall->getPosition()))));
		woodWall->getChildByName("preRect")->setVisible(false);
		m_woodWallList.push_back(woodWall);
	}

}

void MonsterManager::showPreRec()
{
	//auto fadein = FadeIn::create(0.5f);
	for (auto monster : m_monsterList)
	{
		//monster->getChildByName("preRect")->runAction(fadein);
		monster->getChildByName("preRect")->setVisible(true);
		monster->getSprite()->setVisible(false);
		monster->hide();
	}
}

void MonsterManager::hidePreRec()
{
	//auto fadeout = FadeOut::create(0.5f);
	for (auto monster : m_monsterList)
	{
		monster->getChildByName("preRect")->setVisible(false);
		//monster->getChildByName("preRect")->runAction(fadeout);
		monster->getSprite()->setVisible(true);
		monster->show();
	}
}

std::vector<int> MonsterManager::createRandomNums(int numCnt, int sum)
{
	std::vector<int> randomVec;
	int randNum;
	numCnt -= 1;
	while (numCnt--)
	{
		randNum = rand() % sum;
		randomVec.push_back(randNum);
		sum -= randNum;
	}
	randomVec.push_back(sum);
	return randomVec;
}

bool MonsterManager::resetAllMons()
{
	m_deathMonsNum = 0;//����
	m_curCheckPoint++;
	if (m_curCheckPoint >= m_allCheckPoint)
	{
		m_fGameOver = 1;
		return false;
	}
	for (auto monster : m_monsterList)
	{
		monster->resetPropoties();
	}
	createMonsterPos();
	return true;
}

bool MonsterManager::isGameOver()
{
	if (!m_fIsInited)
		return true;
	return m_fGameOver;
}


void MonsterManager::setInited()
{
	m_fIsInited = 1;
}


bool MonsterManager::getInited()
{
	return m_fIsInited;
}


void MonsterManager::update(float dt)
{
	if (!m_fIsInited)
		return;
	Point playerPosition = m_player->getPosition() - getPosition();
	//��������ת��
	//playerPosition = convertToNodeSpace(playerPosition);
	if (m_deathMonsNum == m_monsterList.size())
	{

		resetAllMons();
	}

	for (auto woodWall : m_woodWallList)
	{
		auto curPos = woodWall->getPosition();
		Vec2 blockOccupied = ccp(static_cast<int>(curPos.x) / 21, static_cast<int>(curPos.y) / 21);
		if (!woodWall->isAlive())
		{
			continue;
		}
		if (woodWall->getHp() <= 0) //���»��ŵ�״̬
		{
			m_monsPosMap[blockOccupied] = 0;//���λ����Ϣ
			m_map->getCollidable()->setTileGID(2, Vec2(0, -1) +
				m_map->tileCoordFromPosition(m_map->convertToNodeSpace(this->convertToWorldSpace(woodWall->getPosition()))));
			woodWall->die();
		}
	}

	if (m_fGameOver)//��Ϸ������
	{
		return;
	}
	for (auto monster : m_monsterList)
	{
		auto curPos = monster->getPosition();
		Vec2 blockOccupied = ccp(static_cast<int>(curPos.x) / 21, static_cast<int>(curPos.y) / 21);
		auto dis = sqrtf(pow(playerPosition.x - curPos.x, 2) + pow(playerPosition.y - curPos.y, 2));
		if (monster->isAlive())
		{
			if (monster->getHp() <= 0) //���»��ŵ�״̬
			{
				m_monsPosMap[blockOccupied] = 0;//���λ����Ϣ
				monster->die();
				m_deathMonsNum++;
				continue;
			}



			if (dis < 200)//200�ǳ���Χ
			{
				monster->setMonsTaunted();
			}

			if (!monster->isTaunted())//��δ������
			{
				monster->wander();
				continue;
			}

			auto monsWeapon = monster->getMonsterWeapon();
			if (dis < 2 * monsWeapon->getRange())//�������������ٹ���
				//����Ҫ�õ���ͼ�е����ꡣ
			{
				if (typeid(*monsWeapon) == typeid(TrackWeapon))
				{
					dynamic_cast<TrackWeapon*>(monsWeapon)->bindPlayer(m_player);
				}
				if (typeid(*monsWeapon) == typeid(MonsterSword))
				{
					monsWeapon->getSprite()->setVisible(true);
				}
				monsWeapon->attack(m_map->convertToWorldSpace(m_player->getPosition()));
			}
			else
			{
				monsWeapon->getSprite()->setVisible(false);
			}

			m_monsPosMap[blockOccupied] = 0;
			//������λ�����Ϣ

			float xDirToMove = curPos.x > playerPosition.x ? -monster->getMonsterSpeed() : monster->getMonsterSpeed();
			float yDirToMove = curPos.y > playerPosition.y ? -monster->getMonsterSpeed() : monster->getMonsterSpeed();


			if (abs(curPos.x - playerPosition.x) < 3)	xDirToMove = 0;
			if (abs(curPos.y - playerPosition.y) < 3)	yDirToMove = 0;//����಻������λ��

			auto posToMove = ccp(curPos.x + xDirToMove, curPos.y + yDirToMove);

			//̽����Χ��û��ͬ��
			int i = 0;
			while (!monster->mySetPosition(posToMove)
				&& i++ < 4)
			{
				posToMove = ccp(curPos.x + m_dirs[i][0] * xDirToMove, curPos.y + m_dirs[i][1] * yDirToMove);
				//�Է����￨ǽ
			}
		}
	}


}


std::vector<Bullet*> MonsterManager::getMonsterBullets() const
{
	std::vector<Bullet*> monsterBullets;
	for (auto monster : m_monsterList)
	{
		auto tmpBlt = monster->getMonsterWeapon()->getBullet();//��ȡÿ�������������������ӵ�
		for (auto blt : tmpBlt)
			monsterBullets.push_back(blt);//���ӵ���������
	}
	return monsterBullets;
}

std::vector<Monster*> MonsterManager::getMonster()const
{
	return m_monsterList;
}

std::vector<Monster*> MonsterManager::getWoodWall() const
{
	return m_woodWallList;
}

void MonsterManager::setPosMap(Vec2 pos, bool flag)
{
	m_monsPosMap[pos] = flag;
}

bool MonsterManager::isPosOccupied(Vec2 pos)
{
	return m_monsPosMap[pos];
}

void MonsterManager::setCurRoom(Vec2 curRoom)
{
	m_curRoom = curRoom;
}

Vec2 MonsterManager::getCurRoom()
{
	return m_curRoom;
}

void MonsterManager::markRoomVisited(Vec2 room)
{
	m_visitedRoom[room] = true;
}

bool MonsterManager::isRoomVisited(Vec2 room)
{
	return m_visitedRoom[room];
}
void MonsterManager::setBulkMonsterNum(int giantNum)
{
	m_bulkMonsterNum = giantNum;
}