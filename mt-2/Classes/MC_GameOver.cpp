//---------------------------------------------------------------------------
#include "MC_GameOver.h"
#include "MC_MainScene.h"
//---------------------------------------------------------------------------
bool MC_GameOver::m_IsSeccess = false;
//---------------------------------------------------------------------------
MC_GameOver::MC_GameOver(void)
{	
}
//---------------------------------------------------------------------------
MC_GameOver::~MC_GameOver(void)
{
}
//---------------------------------------------------------------------------
CCScene* MC_GameOver::scene()
{
	CCScene* tScene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        tScene = CCScene::create();
        CC_BREAK_IF(!tScene);

        // 'layer' is an autorelease object
        MC_GameOver* tLayer = MC_GameOver::create();
        CC_BREAK_IF(!tLayer);

        // add layer as a child to scene
        tScene->addChild(tLayer);
    } while (0);

    return tScene;
}
//---------------------------------------------------------------------------
bool MC_GameOver::init()
{
	bool tRet = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

		//turn the color of backgrand white;
		CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(0, 0, 0, 125)));

		// Get window size and place the label upper. 
        CCSize tWinSize = CCDirector::sharedDirector()->getWinSize();

		CCMenuItemImage *tCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(MC_GameOver::menuCloseCallback));
		tCloseItem->setPosition(ccp(tWinSize.width - 20, 20));
		CCMenu* tMenu = CCMenu::create(tCloseItem, NULL);
		tMenu->setPosition(CCPointZero);
		this->addChild(tMenu, 0);

        tRet = true;
    } while (0);

    return tRet;
}
//---------------------------------------------------------------------------
void MC_GameOver::menuCloseCallback(CCObject* vSender)
{
	CCScene* tMainScene = MC_MainScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, tMainScene));
}
//---------------------------------------------------------------------------