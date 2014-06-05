#include "Teleport.h"

Teleport::Teleport(Value dict, int x, int y)
{
	Point position = Vec2(x, y);
	//���͵����ڵ�TileMapλ��
	tileCoord = sGlobal->gameMap->tileCoordForPosition(Vec2(x, y));
	//�ó���ʿ��Ŀ������ʼλ��
	std::string key = "heroTileCoordX";
	int x1 = dict.asValueMap().at(key).asInt();
	key = "heroTileCoordY";
	int y1 = dict.asValueMap().at(key).asInt();
	heroTileCoord = Vec2(x1, y1);
	//ȡ��Ŀ���ͼ�Ĳ���
	key = "targetMap";
	targetMap = dict.asValueMap().at(key).asInt();
	//��ȡimage��
	key = "image";
	imagePath = dict.asValueMap().at(key).asString();
	//����������ʾTeleport�ľ���
	teleportSprite = Sprite::create(imagePath);
	teleportSprite->setAnchorPoint(CCPointZero);
	teleportSprite->setPosition(position);
	sGlobal->gameLayer->addChild(teleportSprite, kZTeleport);
}

Teleport::~Teleport(void)
{
}