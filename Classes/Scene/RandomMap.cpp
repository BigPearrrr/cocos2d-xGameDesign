#include "Scene/AdventureMapScene.h"

#define EMPTY_TILE 0

#define WALL_TILE_UP 12
#define WALL_TILE_DOWN 34
#define WALL_TILE_LEFT 12
#define WALL_TILE_RIGHT 12

#define ROOM_TILE 56

#define ROAD_TILE 56

USING_NS_CC;



const int coord[25][2] = {
		{11,11},{52,11},{93,11},{134,11},{175,11},
		{11,52},{52,52},{93,52},{134,52},{175,52},
		{11,93},{52,93},{93,93},{134,93},{175,93},
		{11,134},{52,134},{93,134},{134,134},{175,134},
		{11,175},{52,175},{93,175},{134,175},{175,175} };//25���������������


bool onMap(int x, int y)
{
	return x >= 0 && x <= 4 && y >= 0 && y <= 4;
}


//����һ��������ɵ���Ϸ��ͼ
void AdventureMapLayer::createRandomMap()
{
	m_tileMap = TMXTiledMap::create("map/AdventureMap_random.tmx");

	m_collidable = m_tileMap->getLayer("barrier");//��ȡ�ж���ײ���ϰ���

	m_ground = m_tileMap->getLayer("ground");//��ȡ�����

	m_wall = m_tileMap->getLayer("wall");//��ȡǽ�ڲ�

	m_road = m_tileMap->getLayer("road");//��ȡ������

	std::vector<Vec2>occupiedRoom = {};//ռ�÷��������

	m_rooms = {};

	std::vector<std::pair<Vec2, Vec2>>roadPairs = {};//����������ĵ�·

	Vec2 startRoom = Vec2(2, 2);//��ʼ����

	m_rooms[startRoom] = BEGIN;
	occupiedRoom.push_back(startRoom);

	Vec2 dir[4] = { {0,1},{0,-1},{1,0},{-1,0} };//�ĸ�����

	srand(time(nullptr));

	int randDir = rand() % 4;

	Vec2 nextRoom = Vec2(startRoom.x + dir[randDir].x, startRoom.y + dir[randDir].y);//��ʼ�������һ����������
	m_rooms[nextRoom] = ENEMY;
	occupiedRoom.push_back(nextRoom);

	roadPairs.push_back({ startRoom,nextRoom });
	
	int roomCnt = 2;

	while (roomCnt < ROOM_NUM)//�������7������
	{
		int nearRoomCnt = 0;//���ڷ���
		std::vector<int>dirVec = {};//�������ߵķ���
		for (int i = 0; i < 4; i++)//�����ĸ�����
		{
			Vec2 offset = Vec2(dir[i].x, dir[i].y);
			if (onMap(nextRoom.x + dir[i].x, nextRoom.y + dir[i].y) &&
				m_rooms[nextRoom + offset] == EMPTY)
			{
				dirVec.push_back(i);//�����ߵķ������dirVec
			}
		}
		int randRoomNum = 0;
		if (roomCnt == 2)
		{
			randRoomNum = 2;
		}
		else if (dirVec.size() == 1)
		{
			randRoomNum = 1;
		}
		else
		{
			randRoomNum = rand() % (dirVec.size() - 1) + 1;
		}
		while (nearRoomCnt != randRoomNum)//�����1~3������
		{
			randDir = rand()% dirVec.size();//ѡȡ�������
			Vec2 offset = Vec2(dir[dirVec[randDir]].x, dir[dirVec[randDir]].y);
			if (m_rooms[nextRoom+offset] == EMPTY)
			{
				occupiedRoom.push_back(offset + nextRoom);

				roadPairs.push_back({ nextRoom,offset + nextRoom });

				nearRoomCnt++;
				roomCnt++;

				if (roomCnt == ROOM_NUM)
				{
					m_rooms[nextRoom + offset] = END;
					break;
				}
				else if (roomCnt == ROOM_NUM - 2)
				{
					m_rooms[nextRoom + offset] = BONUS;
					
				}
				else if (roomCnt == ROOM_NUM - 1)
				{
					m_rooms[nextRoom + offset] = SHOP;
				}
				else
				{
					m_rooms[nextRoom + offset] = ENEMY;
				}
			}
		}
		nextRoom += Vec2(dir[dirVec[randDir]].x, dir[dirVec[randDir]].y);//���·���
	}

	for (auto elem : occupiedRoom)
	{
		int x = elem.x;
		int y = elem.y;
		int z = 5 * x + y;
		buildRoom(Vec2(coord[z][0],coord[z][1]));
	}
	m_roadPairs = roadPairs;
	for (auto elem : roadPairs)
	{
		buildRoad(elem);
	}
	this->addChild(m_tileMap, 0, 100);//��Ϸ��ͼ tagΪ100

	buildBonus();
}

void AdventureMapLayer::buildRoom(Vec2 roomCoord)
{
	std::map<Vec2, bool> barrierMap;
	srand(time(nullptr));
	for (int i = 0; i < 10; i++)
	{
		int barrierCoord = rand() % 441;
		barrierMap[Vec2(barrierCoord / 21, barrierCoord % 21)] = true;
	}

	int roomX = roomCoord.x;
	int roomY = roomCoord.y;
	for (int i = roomX - 10; i <= roomX + 10; i++)
	{
		for (int j = roomY - 10; j <= roomY + 10; j++)
		{
			int relativeCoordX = roomX + 10 - i;
			int relativeCoordY = roomY + 10 - j;

			if (barrierMap.find(Vec2(relativeCoordX,relativeCoordY)) != barrierMap.end())
			{
				m_ground->setTileGID(89, Vec2(i, j));
			}
			else
			{
				m_ground->setTileGID(ROOM_TILE, Vec2(i, j));
				m_collidable->setTileGID(EMPTY_TILE, Vec2(i, j));
			}
		}
	}
	this->m_barrierMap = barrierMap;
}

void AdventureMapLayer::buildRoad(std::pair<cocos2d::Vec2, cocos2d::Vec2> roadPair)
{
	Vec2 room1 = roadPair.first;
	Vec2 room2 = roadPair.second;
	int roomCoord1 = 5 * room1.x + room1.y;//��������ķ�������
	int roomCoord2 = 5 * room2.x + room2.y;
	if (room1.x == room2.x)//������������
	{
		int midTileCoordX = (coord[roomCoord1][0] + coord[roomCoord2][0]) / 2;//��������ת��Ϊ��Ƭ����
		int tileCoordY = coord[roomCoord1][1];
		for (int i = midTileCoordX - 9; i <= midTileCoordX + 10; i++)//��· ��ǽ
		{
			m_wall->setTileGID(WALL_TILE_UP, Vec2(i, tileCoordY + 3));//����ǽ
			if (i != midTileCoordX - 9 && i != midTileCoordX + 10)
			{
				m_wall->setTileGID(WALL_TILE_DOWN, Vec2(i, tileCoordY + 4));
			}
			
			m_wall->setTileGID(WALL_TILE_UP, Vec2(i, tileCoordY - 3));//����ǽ
			m_wall->setTileGID(WALL_TILE_DOWN, Vec2(i, tileCoordY - 2));
			for (int j = tileCoordY - 2; j <= tileCoordY + 2; j++)
			{
				m_road->setTileGID(ROAD_TILE, Vec2(i, j));
				if (i != midTileCoordX - 9 && i != midTileCoordX + 10)
				{
					m_collidable->setTileGID(EMPTY_TILE, Vec2(i, j));
				}
				else
				{
					m_collidable->setTileGID(2, Vec2(i, j));
				}
				
			}
		}
		int leftDoorX = midTileCoordX - 9;
		int rightDoorX = midTileCoordX + 10;
		for (int i = tileCoordY - 1; i <= tileCoordY + 2; i++)//��ǽ
		{
			m_wall->setTileGID(ROAD_TILE, Vec2(leftDoorX, i));
			m_wall->setTileGID(ROAD_TILE, Vec2(rightDoorX, i));
		}
		m_wall->setTileGID(WALL_TILE_DOWN, Vec2(rightDoorX, tileCoordY - 2));
	}
	else//������������
	{
		int tileCoordX = coord[roomCoord1][0];//��������ת��Ϊ��Ƭ����
		int midTileCoordY = (coord[roomCoord1][1] + coord[roomCoord2][1]) / 2;
		for (int i = midTileCoordY - 9; i <= midTileCoordY + 10; i++)
		{
			m_wall->setTileGID(WALL_TILE_LEFT, Vec2(tileCoordX - 3,i));
			m_wall->setTileGID(WALL_TILE_RIGHT, Vec2(tileCoordX + 3,i));
			for (int j = tileCoordX - 2; j <= tileCoordX + 2; j++)
			{
				m_road->setTileGID(ROAD_TILE, Vec2(j, i));
				if (i != midTileCoordY - 9 && i != midTileCoordY + 10)
				{
					m_collidable->setTileGID(EMPTY_TILE, Vec2(j, i));
				}
				else
				{
					m_collidable->setTileGID(2, Vec2(j, i));
				}
				m_collidable->setTileGID(EMPTY_TILE, Vec2(j, i));
			}
		}
		int upDoorY = midTileCoordY + 10;
		int downDoorY = midTileCoordY - 9;
		for (int i = tileCoordX - 2; i <= tileCoordX + 2; i++)//��ǽ
		{
			m_wall->setTileGID(ROAD_TILE, Vec2(i, upDoorY));
			m_wall->setTileGID(ROAD_TILE, Vec2(i, upDoorY + 1));
			m_wall->setTileGID(ROAD_TILE, Vec2(i, downDoorY));
			m_wall->setTileGID(ROAD_TILE, Vec2(i, downDoorY + 1));
		}
	}
}

void AdventureMapLayer::buildBonus()
{
	TMXObjectGroup* group = m_tileMap->getObjectGroup("objects");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (m_rooms[Vec2(i,j)] == BONUS)
			{
				m_chest = Sprite::create("chest.png");
				m_chest->setPosition(m_ground->getPositionAt(Vec2(coord[5 * i + j][0], coord[5 * i + j][1])));
				this->addChild(m_chest);
			}
			else if (m_rooms[Vec2(i, j)] == SHOP)
			{
				m_shop = Sprite::create("cat.png");
				m_shop->setPosition(m_ground->getPositionAt(Vec2(coord[5 * i + j][0], coord[5 * i + j][1])));
				this->addChild(m_shop);
			}
			else if (m_rooms[Vec2(i, j)] == END)
			{
				m_portal = Sprite::create("portal.png");
				m_portal->setPosition(m_ground->getPositionAt(Vec2(coord[5 * i + j][0], coord[5 * i + j][1])));
				this->addChild(m_portal);
			}
		}
	}
	
}

Vec2 AdventureMapLayer::roomCoordFromPosition(Vec2 position)
{
	Vec2 tileCoord = tileCoordFromPosition(position);
	int tileGid = m_ground->getTileGIDAt(tileCoord);
	if (tileGid == 0)
	{
		return Vec2(-1, -1);
	}
	int y = (tileCoord.x - 1) / 41;
	int x = (tileCoord.y - 1) / 41;
	return Vec2(x, y);
}


void AdventureMapLayer::switchGate(TMXLayer* wall, TMXLayer* barrier, int roomNum, int dir, bool isClosed)
{
	switch (dir)
	{
	case 0://��
		for (int i = coord[roomNum][1] - 2; i <= coord[roomNum][1] + 2; i++)
		{
			if (isClosed)
			{
				wall->setTileGID(89, Vec2(coord[roomNum][0] + 11, i));
				barrier->setTileGID(89, Vec2(coord[roomNum][0] + 11, i));
			}
			else
			{
				if (i == coord[roomNum][1] - 2)
				{
					wall->setTileGID(34, Vec2(coord[roomNum][0] + 11, i));
				}
				else
				{
					wall->setTileGID(56, Vec2(coord[roomNum][0] + 11, i));
				}
				barrier->setTileGID(2, Vec2(coord[roomNum][0] + 11, i));
			}
		}
		break;
	case 1://��
		for (int i = coord[roomNum][1] - 2; i <= coord[roomNum][1] + 2; i++)
		{
			if (isClosed)
			{
				wall->setTileGID(89, Vec2(coord[roomNum][0] - 11, i));
				barrier->setTileGID(89, Vec2(coord[roomNum][0] - 11, i));
			}
			else
			{
				if (i == coord[roomNum][1] - 2)
				{
					wall->setTileGID(34, Vec2(coord[roomNum][0] - 11, i));
				}
				else
				{
					wall->setTileGID(56, Vec2(coord[roomNum][0] - 11, i));
				}
				barrier->setTileGID(2, Vec2(coord[roomNum][0] - 11, i));
			}
		}
		break;
	case 2://��
		for (int i = coord[roomNum][0] - 2; i <= coord[roomNum][0] + 2; i++)
		{
			if (isClosed)
			{
				wall->setTileGID(89, Vec2(i, coord[roomNum][1] + 11));
				wall->setTileGID(111, Vec2(i, coord[roomNum][1] + 12));
				barrier->setTileGID(89, Vec2(i, coord[roomNum][1] + 11));
			}
			else
			{
				wall->setTileGID(56, Vec2(i, coord[roomNum][1] + 11));
				wall->setTileGID(56, Vec2(i, coord[roomNum][1] + 12));
				barrier->setTileGID(2, Vec2(i, coord[roomNum][1] + 11));
			}
		}
		break;
	default://��
		for (int i = coord[roomNum][0] - 2; i <= coord[roomNum][0] + 2; i++)
		{
			if (isClosed)
			{
				wall->setTileGID(89, Vec2(i, coord[roomNum][1] - 11));
				wall->setTileGID(111, Vec2(i, coord[roomNum][1] - 10));
				barrier->setTileGID(89, Vec2(i, coord[roomNum][1] - 11));
			}
			else
			{
				wall->setTileGID(56, Vec2(i, coord[roomNum][1] - 11));
				wall->setTileGID(56, Vec2(i, coord[roomNum][1] - 10));
				barrier->setTileGID(2, Vec2(i, coord[roomNum][1] - 11));
			}
		}
	}
}

void AdventureMapLayer::addCoin(Coin* coin)
{
	m_coinList.push_back(coin);
}

void AdventureMapLayer::addBlue(Blue* blue)
{
	m_blueList.push_back(blue);
}

void AdventureMapLayer::addRed(Red* red)
{
	m_redList.push_back(red);
}
