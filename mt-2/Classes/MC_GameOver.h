//---------------------------------------------------------------------------
#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__
//---------------------------------------------------------------------------
#include "cocos2d.h"
#include "MC_GameScene.h"
//---------------------------------------------------------------------------
using namespace cocos2d;
//---------------------------------------------------------------------------
class MC_GameOver : public CCLayerColor
{
public:
	static bool m_IsSeccess;

public:
	MC_GameOver(void);
	virtual ~MC_GameOver(void);

	virtual bool init(void);  
	static CCScene* scene();

	virtual void menuCloseCallback(CCObject* vSender);

	CREATE_FUNC(MC_GameOver);
};
//---------------------------------------------------------------------------
#endif 
