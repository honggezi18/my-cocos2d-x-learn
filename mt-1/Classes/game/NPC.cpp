#include "NPC.h"

NPC::NPC(Value dict, int x, int y)
{
	//获取名称
	std::string key = "name";
	npcId = dict.asValueMap().at(key).asString();
	//获取类型
	key = "type";
	type = dict.asValueMap().at(key).asString();
	//获取image项
	key = "image";
	imagePath = dict.asValueMap().at(key).asString();
	//获取rectX和rectY
	key = "rectX";
	int x1 = dict.asValueMap().at(key).asInt();
	key = "rectY";
	int y1 = dict.asValueMap().at(key).asInt();
	rect = CCRectMake(x1, y1, 32, 32);
	//position为cocos2d-x坐标，tileCoord为TileMap坐标
	Point position = ccp(x, y);
	tileCoord = sGlobal->gameMap->tileCoordForPosition(position);
	//创建用于显示npc的精灵
	npcSprite = Sprite::create(imagePath, rect);
	npcSprite->setAnchorPoint(CCPointZero);
	npcSprite->setPosition(position);
	sGlobal->gameLayer->addChild(npcSprite, kZNPC);
	//从动画管理器中根据npcId获取动画，开始永久播放
	Animate* animation = sAnimationMgr->createAnimate(npcId);
	if (animation != NULL) {
		ActionInterval* action = RepeatForever::create(animation);
		npcSprite->runAction(action);
	}
}

NPC::~NPC(void)
{
	////释放CCString对象
	//CC_SAFE_RELEASE(npcId)
	//CC_SAFE_RELEASE(imagePath)
	//CC_SAFE_RELEASE(type)
}
