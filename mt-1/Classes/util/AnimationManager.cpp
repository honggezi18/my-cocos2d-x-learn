#include "AnimationManager.h"

DECLARE_SINGLETON_MEMBER(AnimationManager);

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
	//CCDirector会自己清除AnimationCache
	//CCAnimationCache::purgeSharedAnimationCache();
}

bool AnimationManager::initAnimationMap()
{
	char temp[20];
	sprintf(temp, "%d", aDown);
	//加载勇士向下走的动画
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kDown), temp);
	sprintf(temp, "%d", aRight);
	//加载勇士向右走的动画
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kRight), temp);
	sprintf(temp, "%d", aLeft);
	//加载勇士向左走的动画
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kLeft), temp);
	sprintf(temp, "%d", aUp);
	//加载勇士向上走的动画
	AnimationCache::getInstance()->addAnimation(createHeroMovingAnimationByDirection(kUp), temp);
	//加载战斗动画
	sprintf(temp, "%d", aFight);
	AnimationCache::getInstance()->addAnimation(createFightAnimation(), temp);
	//加载NPC动画
	AnimationCache::getInstance()->addAnimation(createNPCAnimation(), "npc0");
	return true;
}

Animation* AnimationManager::createHeroMovingAnimationByDirection(HeroDirection direction)
{
	Texture2D *heroTexture = Director::getInstance()->getTextureCache()->addImage("hero.png");
	//Texture2D *heroTexture = TextureCache::getInstance()->addImage("hero.png");
	SpriteFrame *frame0, *frame1, *frame2, *frame3;
	//第二个参数表示显示区域的x, y, width, height，根据direction来确定显示的y坐标
	frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32*0, 32*direction, 32, 32));
	frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32*1, 32*direction, 32, 32));
	frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32*2, 32*direction, 32, 32));
	frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32*3, 32*direction, 32, 32));
	Vector<SpriteFrame*> animFrames;
	animFrames.pushBack(frame0);
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	//0.05f表示每帧动画间的间隔
	//animation->createWithSpriteFrames(animFrames, 0.05f);

	return animation;
}

//创建战斗动画模板
Animation* AnimationManager::createFightAnimation()
{
	//定义每帧的序号
	int fightAnim[] = 
	{
		4,6,8,10,13,15,17,19,20,22
	};
	Vector<SpriteFrame*> animFrames;
	Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("sword.png");
	//Texture2D *texture = TextureCache::getInstance()->addImage("sword.png");
	SpriteFrame *frame;
	int x, y;
	for (int i = 0; i < 10; i++) 
	{
		//计算每帧在整个纹理中的偏移量
		x = fightAnim[i] % 5 - 1;
		y = fightAnim[i] / 5;
		frame = SpriteFrame::createWithTexture(texture, Rect(192*x, 192*y, 192, 192));
		//第17和19帧在y方向上有-8的偏移
		if (fightAnim[i] == 17 || fightAnim[i] == 19)
		{
			frame->setOffsetInPixels( Vec2(0, -8) );
		}
		animFrames.pushBack(frame);
	}
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
	//animation->createWithSpriteFrames(animFrames, 0.1f);
	return animation;
}

Animation* AnimationManager::createNPCAnimation()
{
	Texture2D *heroTexture = Director::getInstance()->getTextureCache()->addImage("npc.png");
	//Texture2D *heroTexture = TextureCache::getInstance()->addImage("npc.png");
	SpriteFrame *frame0, *frame1, *frame2, *frame3;
	//第二个参数表示显示区域的x, y, width, height，根据direction来确定显示的y坐标
	frame0 = SpriteFrame::createWithTexture(heroTexture, Rect(32*0, 0, 32, 32));
	frame1 = SpriteFrame::createWithTexture(heroTexture, Rect(32*1, 0, 32, 32));
	frame2 = SpriteFrame::createWithTexture(heroTexture, Rect(32*2, 0, 32, 32));
	frame3 = SpriteFrame::createWithTexture(heroTexture, Rect(32*3, 0, 32, 32));
	Vector<SpriteFrame*> animFrames;
	animFrames.pushBack(frame0);
	animFrames.pushBack(frame1);
	animFrames.pushBack(frame2);
	animFrames.pushBack(frame3);
	Animation* animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
	//0.05f表示每帧动画间的间隔
	//animation->createWithSpriteFrames();

	return animation;
}

//获取指定动画模版
Animation* AnimationManager::getAnimation(int key)
{
	char temp[20];
	sprintf(temp, "%d", key);
	return AnimationCache::getInstance()->getAnimation(temp);
}

//获取一个指定动画模版的实例
Animate* AnimationManager::createAnimate(int key)
{
	//获取指定动画模版
	Animation* anim = getAnimation(key);
	if(anim)
	{
		//根据动画模版生成一个动画实例
		return Animate::create(anim);
	}
	return NULL;
}

//获取一个指定动画模版的实例
Animate* AnimationManager::createAnimate(const std::string key)
{
	//获取指定动画模版
	Animation* anim = AnimationCache::getInstance()->getAnimation(key);
	if(anim)
	{
		//根据动画模版生成一个动画实例
		return Animate::create(anim);
	}
	return NULL;
}