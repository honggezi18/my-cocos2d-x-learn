#include "GameScene.h"

GameScene::GameScene(void)
{
	sGlobal->gameScene = this;
}

GameScene::~GameScene(void)
{
}

bool GameScene::init()
{
	//新建一个GameLayer实例
	GameLayer *gamelayer = GameLayer::create();
	//将GameLayer实例添加到场景中
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//新建一个ControlLayer实例
	ControlLayer *controlLayer = ControlLayer::create();
	//将ControlLayer实例添加到场景中
	this->addChild(controlLayer, kControlLayer, kControlLayer);

	//auto layer = Layer::create();
	//Size visibleSize = Director::getInstance()->getVisibleSize();
 //   Vec2 origin = Director::getInstance()->getVisibleOrigin();

 //   // add layer as a child to scene
 //   this->addChild(layer);
	//auto label = LabelTTF::create("Hello World", "Arial", 24);
 // 
 //   // position the label on the center of the screen
 //   label->setPosition(Vec2(origin.x + visibleSize.width/2,
 //                           origin.y + visibleSize.height - label->getContentSize().height));

 //   // add the label as a child to this layer
 //   this->addChild(label, 1);

	return true;
}

Scene* GameScene::playNewGame()
{
	GameScene* scene = NULL;
	do 
    {
		//新游戏，当前地图层数为0
		sGlobal->currentLevel = 0;
		//勇士出生位置
		sGlobal->heroSpawnTileCoord = Vec2(1, 11);
		scene = GameScene::create();
		CC_BREAK_IF(! scene);
	} while (0);
	return scene;
}
//切换游戏地图之前
void GameScene::switchMap()
{
	//创建一个遮罩层，用于地图切换时的显示淡入淡出效果
	LayerColor* fadeLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	fadeLayer->setAnchorPoint(Point::ZERO);
	fadeLayer->setPosition(Point::ZERO);
	addChild(fadeLayer, kFadeLayer, kFadeLayer);
	//执行淡入动画，结束后调用resetGameLayer方法
	Action* action = Sequence::create(
		FadeIn::create(0.5f),
		CCCallFunc::create(this, callfunc_selector(GameScene::resetGameLayer)),
		NULL);
	fadeLayer->runAction(action);
}
//切换游戏地图
void GameScene::resetGameLayer()
{
	//删除老的GameLayer
	this->removeChildByTag(kGameLayer, true);
	//创建新的GameLayer
	GameLayer *gamelayer = GameLayer::create();
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//遮罩层执行淡出效果，结束后，调用removeFadeLayer方法删除遮罩层
	Action* action = Sequence::create(
		FadeOut::create(0.5f),
		CCCallFunc::create(this, callfunc_selector(GameScene::removeFadeLayer)),
		NULL);
	this->getChildByTag(kFadeLayer)->runAction(action);
}

void GameScene::removeFadeLayer()
{
	this->removeChildByTag(kFadeLayer, true);
}