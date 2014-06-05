//---------------------------------------------------------------------------
#ifndef __MainScene_SCENE_H__
#define __MainScene_SCENE_H__
//---------------------------------------------------------------------------
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
//---------------------------------------------------------------------------
using namespace cocos2d;
//---------------------------------------------------------------------------
class MC_MainScene : public LayerColor
{
public:
    virtual bool	init(void);  
    static  Scene*	scene(void);
    
    virtual void menuCloseCallback		(Ref* vSender);
	virtual void menuStartGameCallback	(Ref* vSender);
	virtual void menuLogin				(Ref* vSender);
	virtual void menuRegister			(Ref* vSender);

    CREATE_FUNC(MC_MainScene);
};
//---------------------------------------------------------------------------
#endif  // __MainScene_SCENE_H__