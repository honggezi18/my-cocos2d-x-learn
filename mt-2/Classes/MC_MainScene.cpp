//---------------------------------------------------------------------------
#include "MC_MainScene.h"
#include "MC_GameScene.h"
//---------------------------------------------------------------------------
Scene* MC_MainScene::scene(void)
{
    Scene* tScene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        tScene = Scene::create();
        CC_BREAK_IF(!tScene);

        // 'layer' is an autorelease object
        MC_MainScene* tLayer = MC_MainScene::create();
        CC_BREAK_IF(!tLayer);

        // add layer as a child to scene
        tScene->addChild(tLayer);
    } while (0);

    // return the scene
    return tScene;
}
//---------------------------------------------------------------------------
bool MC_MainScene::init(void)
{
    bool tRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(!Layer::init());

		//////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		//turn the color of backgrand white;
		CC_BREAK_IF(!LayerColor::initWithColor(Color4B(0, 0, 0, 125)));

		// Get window size and place the label upper. 
        Size tWinSize = Director::sharedDirector()->getWinSize();

        //Add a menu item with "X" image, which is clicked to quit the program.
        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *tCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(MC_MainScene::menuCloseCallback));
        CC_BREAK_IF(!tCloseItem);

        // Place the menu item bottom-right conner.
        tCloseItem->setPosition(ccp(tWinSize.width - 20, 20));

		CCMenuItemFont* tStartGameItem = CCMenuItemFont::create("Start Game", this, menu_selector(MC_MainScene::menuStartGameCallback) );;
		tStartGameItem->setFontSizeObj(50);
		tStartGameItem->setPosition(ccp(tWinSize.width/2, 50));

		CCMenuItemFont* tCheckInItem = CCMenuItemFont::create("Login", this, menu_selector(MC_MainScene::menuLogin) );;
		tCheckInItem->setFontSizeObj(30);
		tCheckInItem->setPosition(ccp(tWinSize.width/2, tWinSize.height/2+15));


		CCMenuItemFont* tCheckOutItem = CCMenuItemFont::create("Register", this, menu_selector(MC_MainScene::menuRegister) );;
		tCheckOutItem->setFontSizeObj(30);
		tCheckOutItem->setPosition(ccp(tWinSize.width/2, tWinSize.height/2-15));

        // Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* tMenu = CCMenu::create(tCloseItem, tStartGameItem, tCheckInItem, tCheckOutItem, NULL);
        tMenu->setPosition(CCPointZero);
        CC_BREAK_IF(!tMenu);

        // Add the menu to MainScene layer as a child layer.
        this->addChild(tMenu, 1);

        tRet = true;
    } while (0);

    return tRet;
}
//---------------------------------------------------------------------------
void MC_MainScene::menuCloseCallback(CCObject* vSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
//---------------------------------------------------------------------------
void MC_MainScene::menuStartGameCallback(CCObject* vSender)
{
	CCScene* tGameScene = MC_GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(tGameScene);
}
//---------------------------------------------------------------------------
void MC_MainScene::menuLogin(CCObject* vSender)
{
}
//---------------------------------------------------------------------------
void MC_MainScene::menuRegister(CCObject* vSender)
{
}
//---------------------------------------------------------------------------

