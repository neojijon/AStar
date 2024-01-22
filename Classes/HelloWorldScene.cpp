#include "HelloWorldScene.h"
#include "Location_Node_Class.h"
#include "Node_Class.h"
#include <iostream>
#include "cocos2d.h"
#include <queue>
#include <math.h>
#include <string>
#include "queue"

int i=0;
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();

    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	label = Label::createWithTTF("Path not found", "fonts/Marker Felt.ttf", 24);
	this->addChild(label, 2);
	label->setVisible(false);


	this->_touchEnabled = true;
	this->_touchMode = Touch::DispatchMode::ONE_BY_ONE;	
	
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	//touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	background=Sprite::create("Grid.png");
	background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	this->addChild(background,0);

	checker = true;
	obstacle_selector=true;

	noder = Node_Class::create();
	noder->empty_square_array();


    return true;
}

//void HelloWorld::onTouchEnded(cocos2d::Touch* stouch, cocos2d::Event* sevent)
//{
//
//}


bool HelloWorld::onTouchBegan(cocos2d::Touch *stouch,cocos2d::Event *sevent)
{
	if(obstacle_selector==true)
	{
		location_obstacle = stouch->getLocation();
		i3 = (location_obstacle.x/ BLOCKSIZE) + 1;
		j3 = (location_obstacle.y/ BLOCKSIZE) + 1;
		if(noder->squares[i3][j3]==0)
		{
			createobstacleImage(i3,j3);
			noder->squares[i3][j3] = 1;
		}
		else
		{
			obstacle_selector=false;
		}
	}
	else
	{
		i++;

		if(i==1)
		{
			location_start = stouch->getLocation();
			i1= (location_start.x/ BLOCKSIZE) + 1;
			j1= (location_start.y/ BLOCKSIZE) + 1;
			createstartImage(i1,j1);
		}

		if(i==2)
		{
			location_end = stouch->getLocation();
			i2= (location_end.x/ BLOCKSIZE) + 1;
			j2= (location_end.y/ BLOCKSIZE) + 1;
			CCLOG("This is istrat = %d and jstart = %d and iend = %d and jend = %d ",i1,j1,i2,j2);
			if(checker == true)
			{
				if(label->isVisible())
				{
					CCLOG("It is visible");
					label->setVisible(false);
				}
				checker=workingallaone(i1,j1,i2,j2);
			}
		}

		if(i >= 3)
		{
			location_end = stouch->getLocation();
			i2= (location_end.x/ BLOCKSIZE) + 1;
			j2= (location_end.y/ BLOCKSIZE) + 1;
			if(checker == true)
			{

				if(label->isVisible())
				{
					CCLOG("It is visible");
					label->setVisible(false);
				}
				checker=workingallaone(i1,j1,i2,j2);
			}

		}
	}

	return true;
}

void HelloWorld::createstartImage(int istart,int jstart)
{
	box=Sprite::create("My_box.png");
	box->setPosition(Vec2((istart* BLOCKSIZE)-50,(jstart* BLOCKSIZE)-50));
	this->addChild(box,1);
}

void HelloWorld::createobstacleImage(int i3,int j3)
{
	obstacle1=Sprite::create("My_Obstacle.png");
	obstacle1->setPosition(Vec2((i3* BLOCKSIZE)-50,(j3* BLOCKSIZE)-50));
	this->addChild(obstacle1,1);
}



bool HelloWorld::workingallaone(int istart,int jstart,int iend,int jend)
{
	path1 = noder->pathFind(Location_Node_Class(istart, jstart), Location_Node_Class(iend, jend));

	char * path = new char [path1.length() + 1];
	strcpy(path,path1.c_str());

	int j = strtol(path1.c_str(),NULL,10);
	#define NUMOFACTIONS 20
	actions = new Vector<FiniteTimeAction*>(NUMOFACTIONS);

	auto action0 = MoveBy::create(0.5,Vec2(BLOCKSIZE,0));
	auto action1 = MoveBy::create(0.5,Vec2(0, BLOCKSIZE));
	auto action2 = MoveBy::create(0.5,Vec2(-BLOCKSIZE,0));
	auto action3 = MoveBy::create(0.5,Vec2(0,-BLOCKSIZE));
	auto callback = CallFuncN::create(CC_CALLBACK_0(HelloWorld::something, this));

	if(path1 == "")
	{
		label->setVisible(true);
		label->setColor(Color3B(255,255,0));
		label->setPosition(Vec2(400,300));

		i1=istart;
		j1=jstart;
		return true;
	}
	else
	{
		for(int i=0;i<=path1.length();i++)
		{
			if(path[i] == '0' )
			{
				actions->pushBack(action0);

			}
			else if(path[i] == '1' )
			{
				actions->pushBack(action1);

			}
			else if(path[i] == '2' )
			{
				actions->pushBack(action2);

			}
			else if(path[i] == '3' )
			{
				actions->pushBack(action3);

			}
		}
		actions->pushBack(callback);
		const Vector<FiniteTimeAction*> xyz = *actions;
		auto sequence = Sequence::create(xyz);
		box->runAction(sequence);
		i1=iend;
		j1=jend;

	}
	return false;
}

void HelloWorld::something(void)
{
	checker = true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
