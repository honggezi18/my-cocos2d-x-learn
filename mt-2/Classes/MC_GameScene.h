//---------------------------------------------------------------------------
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
//---------------------------------------------------------------------------
#include "cocos2d.h"
//---------------------------------------------------------------------------
using namespace cocos2d;
//---------------------------------------------------------------------------
typedef struct
{ 
	int x;
    int y;
    int k; 
} PATH;  
//---------------------------------------------------------------------------
class MC_GameScene : public cocos2d::CCLayer
{
private:
	CCSprite*		m_SpritePlayer;
	CCSprite*		m_SpriteMonster_A;
	CCSprite*		m_SpriteMonster_B;
	CCLabelTTF*		m_LifeNumLabel;
	CCMenuItemFont* m_ButtonAutoRun;

	float	m_TargetX;
	float	m_TargetY;
	int		m_LifeNumOfPlayer;
	int		m_MapTable[10][15];
	bool	m_IsFindRoadAuto;

	int	m_Dx[4];
	int m_Dy[4];

	PATH	m_Path[150];
	int		m_Step;
	int		m_CurStep;

private:
	void			SpriteRandMove	(CCNode* vSender);
	int				GetDirection	(float vX, float vY);
	void			FinishedMove	(CCNode* vSender);
	CCAnimation*	GetAnimation	(char* vImagName, int vDirection);	//vDirection=> 0:down  1:left  2:right  3:up
	void			Update			(float vDt);
	void			AttackA			(float vDt);
	void			AttackB			(float vDt);
	void			FightImage		(void);
	void			FinishedAttack	(CCNode* vSender);
	void			AutoRun			(CCNode* vSender);
	void			AutoRunCallback	(CCObject* vSender);

	void			AttackFinished	(CCNode* vSender);
	void			PlayerAttack	(CCNode* vSender);
	void			MonsterAttack	(CCNode* vSender);

	void			GetMapTableIndex(float vX, float vY, int* vIndexX, int* vIndexY);
	void			GetRealPosition	(int vIndexX, int vIndexY, float* vX, float* vY);
	bool			AutoFindLoad	(int vOriginX, int vOriginY, int vDestinationX, int vDestinationY, PATH* vPath, int* vStep);

public:
	MC_GameScene(void);
	virtual ~MC_GameScene(void);
	virtual bool	init	(void);  
	static CCScene* scene	(void);
	
	virtual void menuCloseCallback	(CCObject* vSender);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
	virtual void TouchesEnded		(CCSet* vTouches, CCEvent* vEvent);

	CREATE_FUNC(MC_GameScene);
};
//---------------------------------------------------------------------------
#endif 
