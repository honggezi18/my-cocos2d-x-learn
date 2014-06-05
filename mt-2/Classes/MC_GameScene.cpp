//---------------------------------------------------------------------------
#include "SimpleAudioEngine.h"
#include "MC_GameScene.h"
#include "MC_MainScene.h"
#include "MC_GameOver.h"
#include "cmath"
//---------------------------------------------------------------------------
using namespace cocos2d;
//---------------------------------------------------------------------------
//计算两点间距离
#define DIST(Ax, Ay, Bx, By) sqrt((Ax-Bx)*(Ax-Bx)+(Ay-By)*(Ay-By))
//---------------------------------------------------------------------------
MC_GameScene::MC_GameScene(void)
{
	m_SpritePlayer		= 0;
	m_SpriteMonster_A	= 0;
	m_SpriteMonster_B	= 0;

	m_TargetX			= 0.0;
	m_TargetY			= 0.0;
	m_LifeNumOfPlayer	= 100;

	m_IsFindRoadAuto	= true;

	m_Dx[0] = 0;	m_Dy[0] = -1;	//dowm 
	m_Dx[1] = -1;	m_Dy[1] = 0;	//left
	m_Dx[2] = 1;	m_Dy[2] = 0;	//right 
	m_Dx[3] = 0;	m_Dy[3] = 1;	//up 
}
//---------------------------------------------------------------------------
MC_GameScene::~MC_GameScene(void)
{
}
//---------------------------------------------------------------------------
CCScene* MC_GameScene::scene(void)
{
	CCScene* tScene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        tScene = CCScene::create();
        CC_BREAK_IF(!tScene);

        // 'layer' is an autorelease object
        MC_GameScene* tLayer = MC_GameScene::create();
        CC_BREAK_IF(!tLayer);

        // add layer as a child to scene
        tScene->addChild(tLayer);
    } while (0);
    return tScene;
}
//---------------------------------------------------------------------------
bool MC_GameScene::init()
{
	bool tRet = false;
    do 
    {
		CC_BREAK_IF(!CCLayer::init());

		//从Map文件读取地图数据
		FILE* tFd = fopen("Map", "r");
		CC_BREAK_IF(!tFd);
		for(int i=0; i<10*15; i++)
			fscanf(tFd, "%d,", &m_MapTable[i/15][i%15]);
		fclose(tFd);

		//获取窗口大小 
		CCSize tWinSize = CCDirector::sharedDirector()->getWinSize();

		//添加关闭按钮，以便从游戏场景中退出
		CCMenuItemImage *tCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(MC_GameScene::menuCloseCallback));
		CC_BREAK_IF(!tCloseItem);
		tCloseItem->setPosition(ccp(tWinSize.width - 20, 20));

		//添加设置自动寻路的按钮，默认为自动寻路
		m_ButtonAutoRun = CCMenuItemFont::create("Auto Run", this, menu_selector(MC_GameScene::AutoRunCallback));
		CC_BREAK_IF(!m_ButtonAutoRun);
		m_ButtonAutoRun->setFontSizeObj(17);
		m_ButtonAutoRun->setPosition(ccp(120, tWinSize.height-15));

		//添加按钮菜单
		CCMenu* tMenu = CCMenu::create(tCloseItem, m_ButtonAutoRun, NULL);
		CC_BREAK_IF(!tMenu);
		tMenu->setPosition(CCPointZero);
		this->addChild(tMenu, 1);

		//添加地图作为背景
		CCSprite* tSprite = CCSprite::create("Background.png");
		CC_BREAK_IF(!tSprite);
		tSprite->setPosition(ccp(tWinSize.width/2, tWinSize.height/2));
		this->addChild(tSprite, 0);

		//添加玩家精灵
		float			tPositionX	= 0.0;
		float			tPositionY	= 0.0;
		CCTexture2D*	tTexture	= CCTextureCache::sharedTextureCache()->addImage("Player.png"); 
		CC_BREAK_IF(!tTexture);
		CCSpriteFrame*	tFrame		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*0, 32, 32)); 
		CC_BREAK_IF(!tFrame);
						tSprite		= CCSprite::createWithSpriteFrame(tFrame);
		CC_BREAK_IF(!tSprite);
		GetRealPosition(1, 1, &tPositionX, &tPositionY);
		tSprite->setPosition(ccp(tPositionX, tPositionY) ); 
		this->addChild(tSprite, 2);
		m_SpritePlayer = tSprite; 

		//添加怪物精灵A
		tTexture= CCTextureCache::sharedTextureCache()->addImage("Monster_A.png"); 
		CC_BREAK_IF(!tTexture);
		tFrame	= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*0, 32, 32));	
		CC_BREAK_IF(!tFrame);
		tSprite	= CCSprite::createWithSpriteFrame(tFrame);
		CC_BREAK_IF(!tSprite);
		GetRealPosition(7, 4, &tPositionX, &tPositionY);
		tSprite->setPosition(ccp(tPositionX, tPositionY) ); 
		tSprite->setTag(1);
		this->addChild(tSprite, 2); 
		m_SpriteMonster_A = tSprite;
		SpriteRandMove(tSprite);

		//添加怪物精灵B
		tTexture= CCTextureCache::sharedTextureCache()->addImage("Monster_B.png"); 
		CC_BREAK_IF(!tTexture);
		tFrame	= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*0, 32, 32));	
		CC_BREAK_IF(!tFrame);
		tSprite	= CCSprite::createWithSpriteFrame(tFrame);
		CC_BREAK_IF(!tSprite);
		GetRealPosition(11, 6, &tPositionX, &tPositionY);
		tSprite->setPosition(ccp(tPositionX, tPositionY) ); 
		tSprite->setTag(2);
		this->addChild(tSprite, 2); 
		m_SpriteMonster_B = tSprite;
		SpriteRandMove(tSprite);

		//左上角显示玩家生命值
		m_LifeNumLabel = CCLabelTTF::create("100", "Arial", 24);
		m_LifeNumLabel->setPosition(ccp(20, tWinSize.height-20));
		this->addChild(m_LifeNumLabel, 1);

		this->setTouchEnabled(true);
		this->schedule(schedule_selector(MC_GameScene::Update));  
		this->schedule(schedule_selector(MC_GameScene::AttackA)); 
		this->schedule(schedule_selector(MC_GameScene::AttackB), 0.3); 

		tRet = true;
    } while (0);

	return true;
}
//---------------------------------------------------------------------------
void MC_GameScene::menuCloseCallback(CCObject* pSender)
{
	CCScene* tMainScene = MC_MainScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, tMainScene));	
}
//---------------------------------------------------------------------------
//精灵随机走动函数
void MC_GameScene::SpriteRandMove(CCNode* vSender)
{
	CCSprite* tSprite = (CCSprite *)vSender;

	//获取精灵的地图下标值
	int	tIndexX = 0;
	int	tIndexY = 0;
	GetMapTableIndex(tSprite->getPosition().x, tSprite->getPosition().y, &tIndexX, &tIndexY);
	
	//随机走动时间，可以调制走动速度
	int	tActualDuration = rand() % 2 + 1;
	do
	{	
		//随机走动方向，知道找到一个可以走的方向为止
		int tRandDirection	= rand() % 4; 
		if(0 == m_MapTable[ tIndexY+m_Dy[tRandDirection] ][ tIndexX+m_Dx[tRandDirection] ])
			continue;

		char* tMonster = 0;
		switch(tSprite->getTag())
			{
			case 1: tMonster = "Monster_A.png"; break;
			case 2: tMonster = "Monster_B.png"; break;
			}

		//绑定动画动作
		CCAnimation*	tAnimation	= GetAnimation(tMonster, tRandDirection); 
		CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
		tSprite->runAction(CCRepeatForever::create(tAnimate));

		//通过地图下标值获取实际屏幕坐标
		float tTargetX = 0.0;
		float tTargetY = 0.0;
		GetRealPosition(tIndexX+m_Dx[tRandDirection], tIndexY+m_Dy[tRandDirection], &tTargetX, &tTargetY);

		//绑定移动动作
		CCFiniteTimeAction* actionMove = CCMoveTo::create((float)tActualDuration, ccp(tTargetX, tTargetY) );
		CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(MC_GameScene::SpriteRandMove));
		tSprite->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
		return;
	} while(1);
}
//---------------------------------------------------------------------------
void MC_GameScene::onTouchesEnded(const std::vector<Touch*>& vTouches, Event* vEvent)
{
	//先停止玩家精灵当前左右动作，重新绑定动作
	m_SpritePlayer->stopAllActions();

	//获取触屏坐标
	CCTouch*	tTouch = (CCTouch*)( vTouches.front() );
	CCPoint		tLocation = tTouch->getLocationInView();
	tLocation	= CCDirector::sharedDirector()->convertToGL(tLocation);
	m_TargetX	= tLocation.x;
	m_TargetY	= tLocation.y;

	if(true == m_IsFindRoadAuto)	//自动寻路方式
		{	
		//获取精灵当前地图下标
		int tOriginX = 0;
		int tOriginY = 0;
		GetMapTableIndex(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y, &tOriginX, &tOriginY);

		//获取目的地地图下标
		int tDestinationX = 0;
		int tDestinationY = 0;
		GetMapTableIndex(m_TargetX, m_TargetY, &tDestinationX, &tDestinationY);

		//通过迷宫算法获取路径
		m_Step		= 0;
		m_CurStep	= 1;
		if(false == AutoFindLoad(tOriginX, tOriginY, tDestinationX, tDestinationY, m_Path, &m_Step))
			return;
		//执行动画
		if(tOriginX!=tDestinationX || tOriginY!=tDestinationY)
			AutoRun(m_SpritePlayer);
		}
	else	//非自动寻路方式
		{	
		//绑定走动动画
		int				tDirection	= GetDirection(m_TargetX-m_SpritePlayer->getPositionX(), m_TargetY-m_SpritePlayer->getPositionY());;
		CCAnimation*	tAnimation	= GetAnimation("Player.png", tDirection);
		CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
		m_SpritePlayer->runAction(CCRepeatForever::create(tAnimate));

		//获取地图下标
		int tIndexX = 0;
		int tIndexY = 0;
		GetMapTableIndex(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y, &tIndexX, &tIndexY);
		//目的地为障碍时直接返回
		if(0 == m_MapTable[ tIndexY+m_Dy[tDirection] ][ tIndexX+m_Dx[tDirection] ])
			return;

		//绑定移动动作
		float tTargetX = 0.0;
		float tTargetY = 0.0;
		GetRealPosition(tIndexX+m_Dx[tDirection], tIndexY+m_Dy[tDirection], &tTargetX, &tTargetY);
		CCFiniteTimeAction* actionMove = CCMoveTo::create(0.3, ccp(tTargetX, tTargetY) );
		CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::FinishedMove));
		m_SpritePlayer->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
		}
}
void MC_GameScene::TouchesEnded(CCSet* vTouches, CCEvent* vEvent)
{
	//先停止玩家精灵当前左右动作，重新绑定动作
	m_SpritePlayer->stopAllActions();

	//获取触屏坐标
	CCTouch*	tTouch = (CCTouch*)( vTouches->anyObject() );
	CCPoint		tLocation = tTouch->getLocationInView();
	tLocation	= CCDirector::sharedDirector()->convertToGL(tLocation);
	m_TargetX	= tLocation.x;
	m_TargetY	= tLocation.y;

	if(true == m_IsFindRoadAuto)	//自动寻路方式
		{	
		//获取精灵当前地图下标
		int tOriginX = 0;
		int tOriginY = 0;
		GetMapTableIndex(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y, &tOriginX, &tOriginY);

		//获取目的地地图下标
		int tDestinationX = 0;
		int tDestinationY = 0;
		GetMapTableIndex(m_TargetX, m_TargetY, &tDestinationX, &tDestinationY);

		//通过迷宫算法获取路径
		m_Step		= 0;
		m_CurStep	= 1;
		if(false == AutoFindLoad(tOriginX, tOriginY, tDestinationX, tDestinationY, m_Path, &m_Step))
			return;
		//执行动画
		AutoRun(m_SpritePlayer);
		}
	else	//非自动寻路方式
		{	
		//绑定走动动画
		int				tDirection	= GetDirection(m_TargetX-m_SpritePlayer->getPositionX(), m_TargetY-m_SpritePlayer->getPositionY());;
		CCAnimation*	tAnimation	= GetAnimation("Player.png", tDirection);
		CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
		m_SpritePlayer->runAction(CCRepeatForever::create(tAnimate));

		//获取地图下标
		int tIndexX = 0;
		int tIndexY = 0;
		GetMapTableIndex(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y, &tIndexX, &tIndexY);
		//目的地为障碍时直接返回
		if(0 == m_MapTable[ tIndexY+m_Dy[tDirection] ][ tIndexX+m_Dx[tDirection] ])
			return;

		//绑定移动动作
		float tTargetX = 0.0;
		float tTargetY = 0.0;
		GetRealPosition(tIndexX+m_Dx[tDirection], tIndexY+m_Dy[tDirection], &tTargetX, &tTargetY);
		CCFiniteTimeAction* actionMove = CCMoveTo::create(0.3, ccp(tTargetX, tTargetY) );
		CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::FinishedMove));
		m_SpritePlayer->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
		}
}
//---------------------------------------------------------------------------
//通过走动方向取得动画帧。vDirection=> 0:down  1:left  2:right  3:up
CCAnimation* MC_GameScene::GetAnimation(char* vImagName, int vDirection)
{	 
	CCTexture2D*	tTexture	= CCTextureCache::sharedTextureCache()->addImage(vImagName); 
	CCSpriteFrame*	tFrame0		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*(float)vDirection, 32, 32)); 
	CCSpriteFrame*	tFrame1		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*1, 32*(float)vDirection, 32, 32)); 
	CCSpriteFrame*	tFrame2		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*2, 32*(float)vDirection, 32, 32)); 
	CCSpriteFrame*	tFrame3		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*3, 32*(float)vDirection, 32, 32)); 
 
	Vector<CCSpriteFrame*> tAnimFrames; 
	tAnimFrames.pushBack(tFrame0); 
	tAnimFrames.pushBack(tFrame1); 
	tAnimFrames.pushBack(tFrame2); 
	tAnimFrames.pushBack(tFrame3); 
 
	CCAnimation *tAnimation = CCAnimation::createWithSpriteFrames(tAnimFrames, 0.2f); 

	return tAnimation;
}
//---------------------------------------------------------------------------
int	MC_GameScene::GetDirection(float vX, float vY)
{
	if((vX>vY) && (vX<-vY))			return 0;
	else if((vX<vY) && (vX<-vY))	return 1;
	else if((vX>vY) && (vX>-vY))	return 2;
	else if((vX<vY) && (vX>-vY))	return 3;

	return 0;
}
//---------------------------------------------------------------------------
void MC_GameScene::FinishedMove(CCNode* vSender)
{
	if(m_TargetX == m_SpritePlayer->getPositionX())
		if(m_TargetY == m_SpritePlayer->getPositionY())
			m_SpritePlayer->stopAllActions();
}
//---------------------------------------------------------------------------
//判断游戏状态
void MC_GameScene::Update(float vDt)
{
	CCSize		tWinSize	= CCDirector::sharedDirector()->getWinSize();
	CCLabelTTF* tInfo		= CCLabelTTF::create("You Win!!!", "Arial", 35); 
	tInfo->setPosition(ccp(tWinSize.width / 2, tWinSize.height/2));

	//游戏失败，切换场景并显示"You Loss~~"
	if(0 >= m_LifeNumOfPlayer)
		{
		this->unschedule(schedule_selector(MC_GameScene::Update));
		m_LifeNumOfPlayer = 0;		
		CCScene* tResult = MC_GameOver::scene();
		tInfo->setString("You Loss~~");
		tResult->addChild(tInfo);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, tResult));
		}

	int tX = 0;
	int tY = 0;
	GetMapTableIndex(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y, &tX, &tY);
	//到达地图下标（13，8）处表示游戏胜利，切换场景并显示"You Win!!!"
	if((13==tX)&&(8==tY))
		{
		this->unschedule(schedule_selector(MC_GameScene::Update));
		CCScene* tResult = MC_GameOver::scene();	
		tResult->addChild(tInfo);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, tResult));
		}

	//更新玩家血量
	char tStr[4] = {0};
	sprintf(tStr, "%d", m_LifeNumOfPlayer);
	m_LifeNumLabel->setString(tStr);
}
//---------------------------------------------------------------------------
//自动怪B的攻击检测
void MC_GameScene::AttackB(float vDt)
{
	//获取玩家坐标及自动怪的坐标
	float tAx		= m_SpritePlayer->getPosition().x;
	float tAy		= m_SpritePlayer->getPosition().y;
	float tBx		= m_SpriteMonster_B->getPosition().x;
	float tBy		= m_SpriteMonster_B->getPosition().y;
	float tDistance = DIST(tAx, tAy, tBx, tBy);
	//距离小鱼64时自动怪主动攻击玩家
	if(64.0 > tDistance)
		{
		CCTexture2D*	tTexture= CCTextureCache::sharedTextureCache()->addImage("Fire.png"); 
		CCSpriteFrame*	tFrame	= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*3, 32, 32));	
		CCSprite*		tSprite	= CCSprite::createWithSpriteFrame(tFrame);		
		tSprite->setPosition(ccp(tBx, tBy) ); 
		this->addChild(tSprite, 3); 

		CCAnimation*	tAnimation	= GetAnimation("Fire.png", 3);
		CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
		tSprite->runAction(CCRepeatForever::create(tAnimate));

		CCFiniteTimeAction* actionMove		= CCMoveTo::create(0.3, ccp(tAx, tAy) );
		CCFiniteTimeAction* actionMoveDone	= CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::FinishedAttack));
		tSprite->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
		
		//每次攻击，玩家扣血量为5
		m_LifeNumOfPlayer -= 5;
		}
}
//---------------------------------------------------------------------------
//被动怪A的碰撞检测
void MC_GameScene::AttackA(float vDt)
{
	CCRect tPlayerRect = CCRectMake(
		m_SpritePlayer->getPositionX() - (m_SpritePlayer->getContentSize().width/2),
		m_SpritePlayer->getPositionY() - (m_SpritePlayer->getContentSize().height/2),
		m_SpritePlayer->getContentSize().width,
		m_SpritePlayer->getContentSize().height);

	CCRect tComputerRect = CCRectMake(
		m_SpriteMonster_A->getPositionX() - (m_SpriteMonster_A->getContentSize().width/2),
		m_SpriteMonster_A->getPositionY() - (m_SpriteMonster_A->getContentSize().height/2),
		m_SpriteMonster_A->getContentSize().width,
		m_SpriteMonster_A->getContentSize().height);

	//发生碰撞，则播放战斗动画
	if (tPlayerRect.intersectsRect(tComputerRect))
		{
		this->unschedule(schedule_selector(MC_GameScene::Update));
		this->unschedule(schedule_selector(MC_GameScene::AttackA));
		this->unschedule(schedule_selector(MC_GameScene::AttackB));
		m_SpritePlayer->stopAllActions();
		m_SpriteMonster_A->stopAllActions();
		m_SpriteMonster_B->stopAllActions();
	
		//播放战斗动画
		FightImage();
		
		//玩家扣血量为20
		m_LifeNumOfPlayer -= 20;
		}
}
//---------------------------------------------------------------------------
void MC_GameScene::FightImage(void)
{
	//插入战斗画面背景
	CCSprite* tBackground = CCSprite::create("FightScene.png");
	tBackground->setPosition(ccp(m_SpritePlayer->getPosition().x, m_SpritePlayer->getPosition().y));
	tBackground->setTag(10);
	this->addChild(tBackground, 5);

	//添加战斗精灵
	CCTexture2D*	tTexture	= CCTextureCache::sharedTextureCache()->addImage("Monster_A.png"); 
	CCSpriteFrame*	tFrame		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*1, 32, 32)); 
	CCSprite*		tSprite		= CCSprite::createWithSpriteFrame(tFrame);
	tSprite->setPosition(ccp(m_SpritePlayer->getPosition().x+60, m_SpritePlayer->getPosition().y) ); 
	tSprite->setTag(11);
	this->addChild(tSprite, 6);
	tTexture	= CCTextureCache::sharedTextureCache()->addImage("Player.png"); 
	tFrame		= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*2, 32, 32)); 
	tSprite		= CCSprite::createWithSpriteFrame(tFrame);
	tSprite->setPosition(ccp(m_SpritePlayer->getPosition().x-60, m_SpritePlayer->getPosition().y) );
	tSprite->setTag(12);
	this->addChild(tSprite, 6);

	MonsterAttack(tSprite);
}
//---------------------------------------------------------------------------
void MC_GameScene::FinishedAttack(CCNode* vSender)
{
	//攻击动画结束，移除怪物A
	CCSprite* tSprite = (CCSprite *)vSender;
	this->removeChild(tSprite, true);
}
//---------------------------------------------------------------------------
//根据迷宫算法获取的路径进行自动寻路
void MC_GameScene::AutoRun(CCNode* vSender)
{
	int tDirectionX = m_Path[m_CurStep].x - m_Path[m_CurStep-1].x;
	int tDirectionY = m_Path[m_CurStep].y - m_Path[m_CurStep-1].y;
	//绑定走动动画
	int				tDirection	= GetDirection((float)tDirectionX, (float)tDirectionY);;
	CCAnimation*	tAnimation	= GetAnimation("Player.png", tDirection);
	CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
	m_SpritePlayer->runAction(CCRepeatForever::create(tAnimate));

	//绑定移动动画
	float tTargetX = 0.0;
	float tTargetY = 0.0;
	GetRealPosition(m_Path[m_CurStep].x, m_Path[m_CurStep].y, &tTargetX, &tTargetY);
	CCFiniteTimeAction* actionMove = CCMoveTo::create(0.3, ccp(tTargetX, tTargetY) );

	//判断是否为最后一步，最后一步时精灵停止动画，否则继续回调本函数
	CCFiniteTimeAction* tActionMoveDone;
	if(m_CurStep == m_Step)
		tActionMoveDone= CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::FinishedMove));
	else
		tActionMoveDone = CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::AutoRun));
	m_SpritePlayer->runAction( CCSequence::create(actionMove, tActionMoveDone, NULL) );
	m_CurStep++;
}
//---------------------------------------------------------------------------
void MC_GameScene::AutoRunCallback(CCObject* vSender)
{
	if(false == m_IsFindRoadAuto)
		{
		m_IsFindRoadAuto = true;
		m_ButtonAutoRun->setString("Auto Run");
		}
	else
		{
		m_IsFindRoadAuto = false;
		m_ButtonAutoRun->setString("Run by yourself");
		}
}
//---------------------------------------------------------------------------
void MC_GameScene::GetMapTableIndex(float vX, float vY, int* vIndexX, int* vIndexY)
{
	*vIndexX = (int)vX / 32;
	*vIndexY = (int)vY / 32;
}
//---------------------------------------------------------------------------
void MC_GameScene::GetRealPosition(int vIndexX, int vIndexY, float* vX, float* vY)
{
	*vX = (float)vIndexX*32 + 16;
	*vY = (float)vIndexY*32 + 16;
}
//---------------------------------------------------------------------------
//本游戏的重点，迷宫算法，解决自动寻路
bool MC_GameScene::AutoFindLoad(int vOriginX, int vOriginY, int vDestinationX, int vDestinationY, PATH* vPath, int* vStep)
{
	if(0 == m_MapTable[vOriginY][vOriginX])
		return false;
	if(0 == m_MapTable[vDestinationY][vDestinationX])
		return false;

	if(vOriginX==vDestinationX && vOriginY==vDestinationY)
		{
		*vStep = 0;
		return true;
		}

	int tMapTable[10][15] = {0};
	for(int i=0; i<10*15; i++)
		tMapTable[i/15][i%15] = m_MapTable[i/15][i%15];

	int tDx[4];
	int tDy[4];
	tDx[0] = 1;   tDy[0] = 0;	//right
	tDx[1] = 0;   tDy[1] = 1;	//up
	tDx[2] = -1;  tDy[2] = 0;	//left
	tDx[3] = 0;   tDy[3] = -1;	//down

	int t=0;  
	int x=vOriginX; 
	int y=vOriginY;   
	int k=-1;
	vPath[0].x=vOriginX;    
	vPath[0].y=vOriginY;  
	vPath[0].k = -1;
	tMapTable[y][x] = -1;
	do 
	{
		do
		{ 
			k++;
			x+=tDx[k];  
			y+=tDy[k];   
			if((tMapTable[y][x]==1) )
				{  
				t++;
				vPath[t].x=x;  
				vPath[t].y=y;  
				vPath[t].k=k;  
				tMapTable[y][x]=-1;    
				if ((x==vDestinationX)&&(y==vDestinationY))
					{
					*vStep = t;
					return true;
					}
				else
					k=-1;   
				}
			else    
				{  
				x-=tDx[k];  
				y-=tDy[k];  
				}

		}while (k<3);
		k=-1; 
		t--; 
		x=vPath[t].x;  
		y=vPath[t].y; 
	} while (t>=0);

	return true;
}
//---------------------------------------------------------------------------
//怪物攻击动画
void MC_GameScene::MonsterAttack(CCNode* vSender)
{
	//添加怪物的攻击动画
	float			tFirePositionX = m_SpritePlayer->getPosition().x+50;
	float			tFirePositionY = m_SpritePlayer->getPosition().y;
	CCTexture2D*	tTexture= CCTextureCache::sharedTextureCache()->addImage("Fire.png"); 
	CCSpriteFrame*	tFrame	= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*3, 32, 32));	
	CCSprite*		tSprite	= CCSprite::createWithSpriteFrame(tFrame);		
	tSprite->setPosition(ccp(tFirePositionX, tFirePositionY) );
	tSprite->setTag(13);
	this->addChild(tSprite, 7); 

	//绑定动画动作
	CCAnimation*	tAnimation	= GetAnimation("Fire.png", 3);
	CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
	tSprite->runAction(CCRepeatForever::create(tAnimate));

	//绑定移动动作，移动动作完成时，轮到玩家攻击动画
	float tTargetX = m_SpritePlayer->getPosition().x-40;
	float tTargetY = m_SpritePlayer->getPosition().y;
	CCFiniteTimeAction* actionMove		= CCMoveTo::create(1, ccp(tTargetX, tTargetY) );
	CCFiniteTimeAction* actionMoveDone	= CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::PlayerAttack));
	tSprite->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
}
//---------------------------------------------------------------------------
//玩家攻击动画
void MC_GameScene::PlayerAttack(CCNode* vSender)
{
	//先移除怪物的攻击动画
	this->removeChildByTag(13, true);

	//添加玩家的攻击动画
	float			tFirePositionX = m_SpritePlayer->getPosition().x-50;
	float			tFirePositionY = m_SpritePlayer->getPosition().y;
	CCTexture2D*	tTexture= CCTextureCache::sharedTextureCache()->addImage("Fire.png"); 
	CCSpriteFrame*	tFrame	= CCSpriteFrame::createWithTexture(tTexture,CCRectMake(32*0, 32*2, 32, 32));	
	CCSprite*		tSprite	= CCSprite::createWithSpriteFrame(tFrame);		
	tSprite->setPosition(ccp(tFirePositionX, tFirePositionY) ); 
	tSprite->setTag(14);
	this->addChild(tSprite, 7); 

	//绑定动画动作
	CCAnimation*	tAnimation	= GetAnimation("Fire.png", 1);
	CCAnimate*		tAnimate	= CCAnimate::create(tAnimation); 
	tSprite->runAction(CCRepeatForever::create(tAnimate));

	//绑定攻击动画的移动动作，完成后清除战斗动画并回复游戏运行
	float tTargetX = m_SpritePlayer->getPosition().x+40;
	float tTargetY = m_SpritePlayer->getPosition().y;
	CCFiniteTimeAction* actionMove		= CCMoveTo::create(1, ccp(tTargetX, tTargetY) );
	CCFiniteTimeAction* actionMoveDone	= CCCallFuncN::create(this, callfuncN_selector(MC_GameScene::AttackFinished));
	tSprite->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
}
//---------------------------------------------------------------------------
//清除战斗动画，并恢复游戏运行
void MC_GameScene::AttackFinished(CCNode* vSender)
{
	this->removeChildByTag(10, true);
	this->removeChildByTag(11, true);
	this->removeChildByTag(12, true);
	this->removeChildByTag(14, true);

	this->schedule(schedule_selector(MC_GameScene::Update));
	this->schedule(schedule_selector(MC_GameScene::AttackB), 0.3);
	this->removeChild(m_SpriteMonster_A, true);
	SpriteRandMove(m_SpriteMonster_B);
}
//---------------------------------------------------------------------------
