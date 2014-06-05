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
	//�½�һ��GameLayerʵ��
	GameLayer *gamelayer = GameLayer::create();
	//��GameLayerʵ����ӵ�������
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//�½�һ��ControlLayerʵ��
	ControlLayer *controlLayer = ControlLayer::create();
	//��ControlLayerʵ����ӵ�������
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
		//����Ϸ����ǰ��ͼ����Ϊ0
		sGlobal->currentLevel = 0;
		//��ʿ����λ��
		sGlobal->heroSpawnTileCoord = Vec2(1, 11);
		scene = GameScene::create();
		CC_BREAK_IF(! scene);
	} while (0);
	return scene;
}
//�л���Ϸ��ͼ֮ǰ
void GameScene::switchMap()
{
	//����һ�����ֲ㣬���ڵ�ͼ�л�ʱ����ʾ���뵭��Ч��
	LayerColor* fadeLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	fadeLayer->setAnchorPoint(Point::ZERO);
	fadeLayer->setPosition(Point::ZERO);
	addChild(fadeLayer, kFadeLayer, kFadeLayer);
	//ִ�е��붯�������������resetGameLayer����
	Action* action = Sequence::create(
		FadeIn::create(0.5f),
		CCCallFunc::create(this, callfunc_selector(GameScene::resetGameLayer)),
		NULL);
	fadeLayer->runAction(action);
}
//�л���Ϸ��ͼ
void GameScene::resetGameLayer()
{
	//ɾ���ϵ�GameLayer
	this->removeChildByTag(kGameLayer, true);
	//�����µ�GameLayer
	GameLayer *gamelayer = GameLayer::create();
	this->addChild(gamelayer, kGameLayer, kGameLayer);
	//���ֲ�ִ�е���Ч���������󣬵���removeFadeLayer����ɾ�����ֲ�
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