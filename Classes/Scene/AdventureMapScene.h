#pragma once

#include "cocos2d.h"


#define EMPTY 0
#define BEGIN 1
#define BONUS 2
#define SHOP 3
#define ENEMY 4
#define BOSS 5
#define END 6

#define ROOM_NUM 7


#include "Entity/Coin/Coin.h"
#include "Entity/Blue/Blue.h"
#include "Entity/Red/Red.h"

class AdventureMapLayer : public cocos2d::Layer
{
private:
	cocos2d::TMXTiledMap* m_tileMap;
	cocos2d::TMXTiledMap* m_miniMap;
	cocos2d::TMXLayer* m_collidable;
	cocos2d::TMXLayer* m_ground;
	cocos2d::TMXLayer* m_wall;
	cocos2d::TMXLayer* m_road;
	cocos2d::Sprite* m_player;
	cocos2d::Sprite* m_chest;
	cocos2d::Sprite* m_shop;
	cocos2d::Sprite* m_portal;
	
	std::map<cocos2d::Vec2, int>m_rooms;

	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;
	std::map<cocos2d::Vec2, bool> m_barrierMap;
	std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>>m_roadPairs;
	std::vector<Coin*> m_coinList;
	std::vector<Blue*> m_blueList;
	std::vector<Red*> m_redList;
public:
	inline cocos2d::TMXTiledMap* getMap() { return m_tileMap; }
	inline cocos2d::TMXTiledMap* getMiniMap() { return m_miniMap; }
	inline cocos2d::TMXLayer* getCollidable() { return m_collidable; }
	inline  cocos2d::TMXLayer* getGround() { return m_ground; }
	inline cocos2d::TMXLayer* getWall() { return m_wall; }
	inline cocos2d::TMXLayer* getRoad() { return m_road; }
	inline cocos2d::Sprite* getPlayer() { return m_player; }
	inline cocos2d::Sprite* getChest() { return m_chest; }
	inline cocos2d::Sprite* getShop() { return m_shop; }
	inline cocos2d::Sprite* getPortal() { return m_portal; }
	inline std::vector<Coin*> getCoinList() { return m_coinList; }
	inline std::vector<Blue*> getBlueList() { return m_blueList; }
	static cocos2d::Scene* createScene();
	virtual bool init();

	virtual void createRandomMap();
	virtual void buildRoom(cocos2d::Vec2 roomCoord);
	virtual void buildRoad(std::pair<cocos2d::Vec2, cocos2d::Vec2> roadPair);
	virtual void buildBonus();
	static void switchGate(cocos2d::TMXLayer* wall, cocos2d::TMXLayer* barrier, int roomNum, int dir, bool isClosed);
	void addCoin(Coin* coin);
	void addBlue(Blue* blue);
	void addRed(Red* red);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	cocos2d::Vec2 roomCoordFromPosition(cocos2d::Vec2 position);

	cocos2d::Point convertToMapSpace(const cocos2d::Point& point);
	std::map<cocos2d::Vec2, bool> getBarrierMap();
	std::vector<std::pair<cocos2d::Vec2, cocos2d::Vec2>> getRoadPairs() { return m_roadPairs; }
	bool isBarrier(cocos2d::Vec2 position);
	bool isMonsterRoom(cocos2d::Vec2 roomCoord);

	// implement the "static create()" method manually
	CREATE_FUNC(AdventureMapLayer);
};

