#ifndef __SAFE_MAP_SCENE_H__
#define __SAFE_MAP_SCENE_H__

#include "cocos2d.h"
#include "Scene/PlayerChoose.h"
#include "Scene/PauseScene.h"
#include "editor-support/cocostudio/CCSGUIReader.h"
#include "ui/CocosGUI.h"
using namespace cocos2d::ui;
using namespace cocostudio;

class SafeMapLayer :public cocos2d::Layer
{
	cocos2d::TMXTiledMap* m_tileMap;
	cocos2d::TMXLayer* m_collidable;
	cocos2d::Sprite* m_player;

	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;

	std::string m_heroName;
public:
	static cocos2d::Scene* createScene();

	virtual void update(float dt);

	virtual bool init();
	virtual void onEnter();
	virtual void onEnterTransitionDidFinish();
	virtual void onExit();
	virtual void onExitTransitionDidStart();
	virtual void cleanup();

	void setPlayer(int playerNum);

	void menuItemSettingCallback(cocos2d::Ref* pSender);
	void menuItemRangerCallback(cocos2d::Ref* pSender);
	void menuItemMageCallback(cocos2d::Ref* pSender);

	void setPlayerPosition(cocos2d::Vec2 position, int dx, int dy);
	cocos2d::Vec2 tileCoordFromPosition(cocos2d::Vec2 position);
	void setViewpointCenter(cocos2d::Vec2 position);

	CREATE_FUNC(SafeMapLayer);
};


#endif // !__SAFE_MAP_SCENE_H__
