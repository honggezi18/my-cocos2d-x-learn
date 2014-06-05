#include "Teleport.h"

Teleport::Teleport(Value dict, int x, int y)
{
	Point position = Vec2(x, y);
	//传送点所在的TileMap位置
	tileCoord = sGlobal->gameMap->tileCoordForPosition(Vec2(x, y));
	//得出勇士在目标层的起始位置
	std::string key = "heroTileCoordX";
	int x1 = dict.asValueMap().at(key).asInt();
	key = "heroTileCoordY";
	int y1 = dict.asValueMap().at(key).asInt();
	heroTileCoord = Vec2(x1, y1);
	//取得目标地图的层数
	key = "targetMap";
	targetMap = dict.asValueMap().at(key).asInt();
	//获取image项
	key = "image";
	imagePath = dict.asValueMap().at(key).asString();
	//创建用于显示Teleport的精灵
	teleportSprite = Sprite::create(imagePath);
	teleportSprite->setAnchorPoint(CCPointZero);
	teleportSprite->setPosition(position);
	sGlobal->gameLayer->addChild(teleportSprite, kZTeleport);
}

Teleport::~Teleport(void)
{
}