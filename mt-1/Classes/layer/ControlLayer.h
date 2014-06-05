#ifndef __CONTROLLAYER_H__
#define __CONTROLLAYER_H__

#include "MTGame.h"

using namespace cocos2d;

class ControlLayer: public Layer
{
public:
	ControlLayer(void);
	~ControlLayer(void);
	//node方法会调用此函数
	virtual bool init();
	//方向按钮点击事件的回调
	void menuCallBackMove(Ref* pSender);
	//关闭按钮点击事件的回调
	void menuCloseCallback(Ref* pSender);
	//使用CCLayer标准的创建实例的方式，声明node方法
	CREATE_FUNC(ControlLayer);
};

#endif
