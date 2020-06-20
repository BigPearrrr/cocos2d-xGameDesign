#include "SettingScene.h"
#include "HomeScene.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace ui;
using namespace CocosDenshion;


Scene* SettingLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = SettingLayer::create();

	scene->addChild(layer);

	return scene;
}

bool SettingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	//����
	auto bg = Sprite::create("menu/SettingBackground.png");
	bg->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(bg);
	//���뻬����
	auto slider = Slider::create();
	slider->loadBarTexture("menu/EmptyVolumeBar.png");
	slider->loadSlidBallTextures("menu/Slider.png", "menu/Slider.png", "menu/Slider.png");//����
	slider->loadProgressBarTexture("menu/FullVolumeBar.png");//������
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 100));
	slider->setPercent(50);

	//����¼�������
	slider->addEventListener([slider](Ref* pSender, Slider::EventType type)
	{
		float percentage = static_cast<float>(slider->getPercent());//��ȡ�ٷֱ�
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(percentage / 100.0f);
	});

	this->addChild(slider);
	//�������ư�ť
	auto musicOnMenuItem = MenuItemImage::create("menu/VolumeOn.png", "menu/VolumeOn.png");
	auto musicOffMenuItem = MenuItemImage::create("menu/VolumeOff.png", "menu/VolumeOff.png");

	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingLayer::menuMusicToggleCallback, this), musicOnMenuItem, musicOffMenuItem, nullptr);
	musicToggleMenuItem->setPosition(Vec2(visibleSize.width / 2 - 125, visibleSize.height / 2 + 100));
	//����˵�

	auto okMenuItem = MenuItemImage::create("menu/CheckMark.png", "menu/CheckMark.png", CC_CALLBACK_1(SettingLayer::menuOkCallback, this));
	okMenuItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));

	auto menu = Menu::create(musicToggleMenuItem, okMenuItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void SettingLayer::onEnter()
{
	Layer::onEnter();
}

void SettingLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/safeBgm.mp3", true);//��ʼ����
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5f);//��������
}

void SettingLayer::onExit()
{
	Layer::onExit();
}

void SettingLayer::onExitTransitionDidStart()
{
	Layer::onExitTransitionDidStart();
}

void SettingLayer::cleanup()
{
	Layer::cleanup();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm/safeBgm.mp3");//ֹͣ����
}

void SettingLayer::menuMusicToggleCallback(cocos2d::Ref* pSender)
{
	auto musicToggleMenuItem = dynamic_cast<MenuItemToggle*>(pSender);

	if (musicToggleMenuItem->getSelectedIndex() == 1)
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic("bgm/safeBgm.mp3");
	}
	else
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/safeBgm.mp3");
	}
}

void SettingLayer::menuOkCallback(cocos2d::Ref* pSender)
{
	//this->removeFromParentAndCleanup(true);
	Director::getInstance()->popScene();
}