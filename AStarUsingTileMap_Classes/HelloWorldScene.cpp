#include "HelloWorldScene.h"

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

    _mapLayer = Layer::create();
    this->addChild(_mapLayer);
    

    auto tmxFileName = "map1.tmx";
    
    TMXTiledMap* pMap = TMXTiledMap::create(tmxFileName);
    _mapLayer->addChild(pMap);
 
    auto pMapInfo = MapInfo::create(tmxFileName);
    _mapInfo = pMapInfo;
    pMapInfo->retain();
    

    auto *frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("player1.plist");
    frameCache->addSpriteFramesWithFile("player2.plist");
    frameCache->addSpriteFramesWithFile("player3.plist");
    frameCache->addSpriteFramesWithFile("player4.plist");
    
    do {
        for (int i = 0; i < 3; i++) {
        
            auto *testSprite1 = this->createTestSpriteWithFormat("player1_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 3; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player2_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 3; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player3_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
        
        for (int i = 0; i < 3; i++) {
            
            auto *testSprite1 = this->createTestSpriteWithFormat("player4_%i_%i.png");
            
            _mapLayer->addChild(testSprite1, 200);
            
            this->actionDone(testSprite1);
        }
    } while (0);

    this->setTouchEvent();

    return true;
}

void HelloWorld::setTouchEvent(){    
    auto *testSprite = this->createTestSpriteWithFormat("player1_%i_%i.png");
    
    testSprite->runAction(RepeatForever::create(Blink::create(0.5, 3)));
    
    Size winSize = Director::getInstance()->getWinSize();
    
    _mapLayer->addChild(testSprite, 200);
    
    Vec2 mapPosition = Vec2(winSize.width * 0.5f - testSprite->getPosition().x , winSize.height * 0.5f - testSprite->getPosition().y);
    
    _mapLayer->setPosition(mapPosition);
    this->adjustMapLayer(false);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto myListener = EventListenerTouchOneByOne::create();
    
    myListener->setSwallowTouches(true);
    
    myListener->onTouchBegan = [=](Touch* touch,Event* event)
    {        
        _bIsMove = false;
        return true;
    };
    
    myListener->onTouchMoved = [=](Touch* touch,Event* event)
    {
        auto touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
        auto firstTouchPoint = _mapLayer->convertToNodeSpace(touch->getStartLocation());
        
        if (touchPoint.getDistance(firstTouchPoint) > 5){
            _bIsMove = true;
            
        }else{
            
        }
        
        if (_bIsMove) {
            auto preTouchPoint = _mapLayer->convertToNodeSpace(touch->getPreviousLocation());
            auto position  = _mapLayer->getPosition();
            _mapLayer->setPosition(Vec2(position.x + touchPoint.x - preTouchPoint.x, position.y + touchPoint.y - preTouchPoint.y));
        }
    };
    
    myListener->onTouchEnded = [=](Touch* touch,Event* event)
    {
        
        auto touchPoint = _mapLayer->convertToNodeSpace(touch->getLocation()); ;
        
        if (!_bIsMove) {
            auto mapId = this->_mapInfo->convertPointToId(touchPoint);
            
            auto position = testSprite->getPosition();
            auto originId = _mapInfo->convertPointToId(position);
            
            auto* pMapPath = _mapInfo->getMapPath(originId, mapId);
            
            if (pMapPath != nullptr)
            {
                auto *pointArr1 = pMapPath->getPointArr();
                
                auto duration = 0.2 * pointArr1->count();
                
                auto *easeWalkTo1 = EaseWalkTo::create(duration, pointArr1);
                easeWalkTo1->setTag(99);
                testSprite->stopActionByTag(99);
                testSprite->runAction(easeWalkTo1);
                
            }
        }else{
            
        }
        
        this->adjustMapLayer(true);
        
    };
    
    dispatcher->addEventListenerWithSceneGraphPriority(myListener, this);
 
}

TestSprite* HelloWorld::createTestSpriteWithFormat(std::string fileName){
    auto startPoint = _mapInfo->getRandomPointMidByType(MapInfoType::Road);
    
    auto *testSprite = TestSprite::create(fileName);
    
    testSprite->setPosition(startPoint);
    
    testSprite->setAnchorPoint(Vec2(0.5, 0));
    
    return testSprite;
}

void HelloWorld::adjustMapLayer(bool isAnim){
    
    do {
        
        Size mapSize = Size( _mapInfo->getMapGridSize().width * _mapInfo->getPointSize().width, _mapInfo->getMapGridSize().height * _mapInfo->getPointSize().height) ;
        Size winSize = Director::getInstance()->getWinSize();
        
        Vec2 mapPoint = _mapLayer->getPosition();
        float x = mapPoint.x;
        float y = mapPoint.y;
        
        Vec2 adjustPoint = Vec2::ZERO;
        
        if (x > 0) {
            adjustPoint += Vec2(-x, 0);
        }else if (x < winSize.width - mapSize.width){
            adjustPoint += Vec2(winSize.width - mapSize.width - x, 0);
        }
        
        if (y > 0) {
            adjustPoint += Vec2(0, -y);
        }else if (y < winSize.height - mapSize.height){
            adjustPoint += Vec2(0, winSize.height - mapSize.height - y);
        }
        
        _mapLayer->stopAllActions();
        if (isAnim) {
            _mapLayer->runAction(EaseBackOut::create(MoveBy::create(0.5f, adjustPoint)));
        }else{
            _mapLayer->setPosition(_mapLayer->getPosition() + adjustPoint);
        }
        
        
    
    } while (0);
}

void HelloWorld::actionDone(Node *pSender){

    if (!_mapInfo) {
        CCLOG("Error: MapInfo is nullptr!");
        return;
    }

    int newTarget = _mapInfo->getRandomMapIdByType(MapInfoType::Road);
    
    int oldTarget = _mapInfo->convertPointToId(pSender->getPosition());
    
    if (oldTarget == newTarget) return;

    MapPath* pMathPath = _mapInfo->getMapPath(oldTarget, newTarget);

    if (!pMathPath) {
        CCLOG("Error: MathPath is nullptr!");
        return;
    }
    
    PointArray *pointArr = pMathPath->getPointArr();
    
    float duration = 0.2 * pointArr->count();
    
    EaseWalkTo *easeWalkTo1 = EaseWalkTo::create(duration, pointArr);
    
    
    Sequence *seq = Sequence::create(
                                     easeWalkTo1,
                                     CallFuncN::create(CC_CALLBACK_1(HelloWorld::actionDone,this)),
                                     NULL
                                     );
    
    pSender->runAction(seq);
}

