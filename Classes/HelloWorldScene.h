/**
Made by Samvid
**/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Location_Node_Class.h"
#include "Node_Class.h"

using namespace cocos2d;
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

	cocos2d::Sprite *background;
    cocos2d::Sprite *box;
	cocos2d::Sprite *obstacle1;
	//cocos2d::Sprite *obstacle;
	//cocos2d::Vector<Sprite>(3) *vecObstacle;
	//cocos2d::Sprite *obstacles[3];
	Vector<Sprite*> *obstacle;
	Vector<FiniteTimeAction*> *actions;

	Label *label;

	int i1,j1,i2,j2,i3,j3;
	Node_Class *noder;
	Node_Class *noder1;
	string path1;


	Vec2 location_start;
	Vec2 location_end;
	Vec2 location_obstacle;
		
	bool onTouchBegan(cocos2d::Touch *stouch,cocos2d::Event *sevent);
	//void onTouchEnded(cocos2d::Touch *stouch,cocos2d::Event *sevent);
	void something(void);

	bool obstacle_selector;

	bool workingallaone(int istart,int jstart,int iend,int jend);
	bool checker;
	void createstartImage(int istart,int jstart);
    void createobstacleImage(int i,int j);
	// implement the "static create()" method manually
    CREATE_FUNC(HelloWorld)
};

#endif // __HELLOWORLD_SCENE_H__
