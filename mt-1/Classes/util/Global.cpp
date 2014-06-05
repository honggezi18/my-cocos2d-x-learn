#include "Global.h"

DECLARE_SINGLETON_MEMBER(Global);

Global::Global(void)
{
	num = 0;
}

Global::~Global(void)
{
	num = 0;
	gameScene = NULL;
	gameLayer = NULL;
	gameMap = NULL;
	hero = NULL;
	controlLayer = NULL;
}