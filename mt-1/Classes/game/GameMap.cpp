#include "GameMap.h"

GameMap::GameMap(void)
{
	sGlobal->gameMap = this;
}

GameMap::~GameMap(void)
{
	this->unscheduleAllSelectors();
	/*CC_SAFE_RELEASE(enemyArray)
	CC_SAFE_RELEASE(npcDict)
	CC_SAFE_RELEASE(teleportDict)*/
}

//��̬��������������GameMapʵ��
GameMap* GameMap::gameMapWithTMXFile(const char *tmxFile)
{
	//newһ������
	GameMap *pRet = new GameMap();
	//����init����
	if (pRet->initWithTMXFile(tmxFile))
	{
		//���ö����init����
		pRet->extraInit();
		//��ʵ������autorelease�أ�ͳһ��������ƶ������������
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
//TliedMap����ĳ�ʼ������
void GameMap::extraInit()
{
	//��������ͼ����������
	enableAnitiAliasForEachLayer();
	//��ʼ���������
	floorLayer = this->layerNamed("floor");
	wallLayer = this->layerNamed("wall");
	itemLayer = this->layerNamed("item");
	doorLayer = this->layerNamed("door");

	initEnemy();
	initObject();
}

void GameMap::initEnemy()
{
	//��ȡ�����
	enemyLayer = this->layerNamed("enemy");
	Size s = enemyLayer->getLayerSize();
	int index;
	//����enemy�㣬�����ڵĹ��ﱣ�浽������
	for (int x = 0; x < s.width; x++) {
		for (int y = 0; y < s.height; y++) {
			int gid = enemyLayer->tileGIDAt(Vec2(x, y));
			if (gid != 0) {
				Enemy* enemy = new Enemy();
				//�����������
				enemy->position = Vec2(x, y);
				//���������ʼ��ͼ��ID
				enemy->startGID = gid;
				//��ӹ����������
				enemyArray.pushBack(enemy);
			}
		}
	}
	//���ڸ��µ��˶���
	schedule(schedule_selector(GameMap::updateEnemyAnimation), 0.2f);
}

//���¹����ͼ��
void GameMap::updateEnemyAnimation(float dt)
{	
	Vector<Enemy*>::iterator  iter;
	Enemy *enemy, *needRemove = NULL;
	//�����������й�����������
	for (iter = enemyArray.begin(); iter != enemyArray.end(); ++iter) {
		enemy = *iter;
		if (enemy != NULL) {
			//��ȡ���ﵱǰ��ͼ��ID
			int gid = enemyLayer->tileGIDAt(enemy->position);
			//������ﱻɾ���ˣ���Ҫ������enemyArray��Ҳɾ��
			if (gid == 0)
			{
				needRemove = enemy;
				continue;
			}
			gid++;
			//�����һ��ͼ��ID ��ʼͼ��ID
			if (gid - enemy->startGID > 3) {
				gid = enemy->startGID;
			}
			//�����������µ�ͼ��
			enemyLayer->setTileGID(gid, enemy->position);
		}
	}
	//ɾ��������Ĺ���
	if (needRemove != NULL) {
		enemyArray.eraseObject(needRemove, true);
	}
}
//��ʼ�������
void GameMap::initObject()
{
	////��ʼ�������ź�npc����
	//teleportDict = new CCMutableDictionary<int, Teleport*>();
	//npcDict = new CCMutableDictionary<int, NPC*>();
	//��ȡ�����
	TMXObjectGroup* group = this->objectGroupNamed("object");
	//��ȡ������ڵ����ж���
	ValueVector objects = group->getObjects();
	Value dict;
	ValueVector::iterator it;
	//�������ж���
	for( it = objects.begin(); it != objects.end(); it++) 
	{
		dict = (*it);
		if(dict.isNull())
			break;
		std::string key = "x";
		//��ȡx����
		int x = dict.asValueMap().at(key).asInt(); //asInt();
		key = "y";
		//��ȡy����
		int y = dict.asValueMap().at(key).asInt();
		CCPoint tileCoord = tileCoordForPosition(ccp(x, y));
		//����ΨһID
		int index = tileCoord.x + tileCoord.y * this->getMapSize().width;
		key = "type";
		//��ȡ�������
		std::string type = dict.asValueMap().at(key).asString();
		//��������Ǵ�����
		if (type == "teleport"){
			Teleport *teleport = new Teleport(dict, x, y);
			teleportDict.insert(index, teleport);
		}
		//���������NPC����
		else if (type == "npc"){
			NPC *npc = new NPC(dict, x, y);
			npcDict.insert(index, npc);
		}
	}
}

void GameMap::enableAnitiAliasForEachLayer()
{
	Vector<Node*> pChildrenArray = this->getChildren();
	SpriteBatchNode* child = NULL;
	Ref* pObject = NULL;
	//����Tilemap������ͼ��
	Vector<Node*>::iterator iter;  
    for(iter=pChildrenArray.begin();iter!=pChildrenArray.end();iter++)  
	{
		child = (SpriteBatchNode*)*iter;
		if(!child)
			break;
		//��ͼ��������������
		child->getTexture()->setAntiAliasTexParameters();
	}
}

//��cocos2d-x����ת��ΪTilemap����
Point GameMap::tileCoordForPosition(Point position)
{
	int x = position.x / this->getTileSize().width;
	int y = (((this->getMapSize().height - 1) * this->getTileSize().height) - position.y) / this->getTileSize().height;
	return Vec2(x, y);
}

//��Tilemap����ת��Ϊcocos2d-x����
CCPoint GameMap::positionForTileCoord(CCPoint tileCoord)
{
	CCPoint pos =  ccp((tileCoord.x * this->getTileSize().width),
		((this->getMapSize().height - tileCoord.y - 1) * this->getTileSize().height));
	return pos;
}

//���صذ��
TMXLayer* GameMap::getFloorLayer() const
{
	return floorLayer;
}
//����ǽ�ڲ�
TMXLayer* GameMap::getWallLayer() const
{
	return wallLayer;
}
//���ع����
TMXLayer* GameMap::getEnemyLayer() const
{
	return enemyLayer;
}
//������Ʒ��
TMXLayer* GameMap::getItemLayer() const
{
	return itemLayer;
}
//�����Ų�
TMXLayer* GameMap::getDoorLayer() const
{
	return doorLayer;
}

//���¹���ս��ʱ����ɫ״̬
void GameMap::updateEnemyHitEffect(float dt)
{
	//���´�����һ
	fightCount++;
	if (fightCount % 2 == 1) {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(ccWHITE);
	} else {
		//���ù���sprite��ɫΪ��ɫ
		fightingEnemy->setColor(ccRED);
	}

	//�л�5�κ�ȡ����ʱ��
	if (fightCount == 5)
	{
		unschedule(schedule_selector(GameMap::updateEnemyHitEffect));
	}
}
//��ʾ����������
void GameMap::showEnemyHitEffect(Point tileCoord)
{
	//���´���
	fightCount = 0;
	//��ȡ�����Ӧ��CCSprite����
	fightingEnemy = enemyLayer->tileAt(tileCoord);
	if (fightingEnemy == NULL)
		return;
	//���ù���sprite��ɫΪ��ɫ
	fightingEnemy->setColor(ccRED);
	//������ʱ�����´��״̬
	this->schedule(schedule_selector(GameMap::updateEnemyHitEffect), 0.18f);
}