#include "NPC.h"

NPC::NPC(Value dict, int x, int y)
{
	//��ȡ����
	std::string key = "name";
	npcId = dict.asValueMap().at(key).asString();
	//��ȡ����
	key = "type";
	type = dict.asValueMap().at(key).asString();
	//��ȡimage��
	key = "image";
	imagePath = dict.asValueMap().at(key).asString();
	//��ȡrectX��rectY
	key = "rectX";
	int x1 = dict.asValueMap().at(key).asInt();
	key = "rectY";
	int y1 = dict.asValueMap().at(key).asInt();
	rect = CCRectMake(x1, y1, 32, 32);
	//positionΪcocos2d-x���꣬tileCoordΪTileMap����
	Point position = ccp(x, y);
	tileCoord = sGlobal->gameMap->tileCoordForPosition(position);
	//����������ʾnpc�ľ���
	npcSprite = Sprite::create(imagePath, rect);
	npcSprite->setAnchorPoint(CCPointZero);
	npcSprite->setPosition(position);
	sGlobal->gameLayer->addChild(npcSprite, kZNPC);
	//�Ӷ����������и���npcId��ȡ��������ʼ���ò���
	Animate* animation = sAnimationMgr->createAnimate(npcId);
	if (animation != NULL) {
		ActionInterval* action = RepeatForever::create(animation);
		npcSprite->runAction(action);
	}
}

NPC::~NPC(void)
{
	////�ͷ�CCString����
	//CC_SAFE_RELEASE(npcId)
	//CC_SAFE_RELEASE(imagePath)
	//CC_SAFE_RELEASE(type)
}
