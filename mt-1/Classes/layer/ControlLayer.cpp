#include "MTGame.h"

ControlLayer::ControlLayer(void)
{
	sGlobal->controlLayer = this;
}

ControlLayer::~ControlLayer(void)
{

}

bool ControlLayer::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}

	//创建关闭按钮
	MenuItemImage *pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		menu_selector(ControlLayer::menuCloseCallback));

	pCloseItem->setPosition(Vec2(Director::getInstance()->getWinSize().width - 20, 20));
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	//创建方向按钮
	MenuItem *down = MenuItemFont::create("down", this, menu_selector(ControlLayer::menuCallBackMove));
	MenuItem *left = MenuItemFont::create("left", this, menu_selector(ControlLayer::menuCallBackMove) );
	MenuItem *right = MenuItemFont::create("right", this, menu_selector(ControlLayer::menuCallBackMove) );
	MenuItem *up = MenuItemFont::create("up", this, menu_selector(ControlLayer::menuCallBackMove) );
	Menu* menu = Menu::create(down, left, right, up, NULL);
	//为了方便查找，给每个menuItem设置tag
	down->setTag(kDown);
	left->setTag(kLeft);
	right->setTag(kRight);
	up->setTag(kUp);
	//菜单项按间距50水平排列
	menu->alignItemsHorizontallyWithPadding(50);
	addChild(menu);
	return true;
}

void ControlLayer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();
}

void ControlLayer::menuCallBackMove(Ref* pSender)
{
    Node *node = (Node *) pSender;
	//按钮的tag就是需要行走的方向
	int targetDirection = node->getTag();
	sGlobal->hero->move((HeroDirection) targetDirection);
}