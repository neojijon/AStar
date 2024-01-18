#include "TestSprite.h"


TestSprite::TestSprite(){
    _lastActionType = PlayerActionType::Invalid;
}

TestSprite::~TestSprite(){
    
}

TestSprite* TestSprite::create(const std::string& fileNameFormat){
    TestSprite *ret = new TestSprite();
    if(ret && ret->init(fileNameFormat))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TestSprite::init(const std::string& fileNameFormat){
    if (Sprite::init())
    {
        SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
        
        for (int j = 0; j < 4; j++) {
            Vector<SpriteFrame*> animFrames;
            
            for(int i = 0; i < 4; i++)
            {
                std::string str = StringUtils::format(fileNameFormat.c_str(), j, i);
                SpriteFrame *frame = frameCache->getSpriteFrameByName(str);
                
                animFrames.pushBack(frame);
            }
            
            Animation *animation = Animation::createWithSpriteFrames(animFrames, 0.2f);
            
            int actionTag = j + 1;
            
            _playerAnimMap.insert(actionTag, animation);

        }
        
        std::string fileStr = StringUtils::format(fileNameFormat.c_str(), 0, 0);
        Sprite *animSprite = Sprite::createWithSpriteFrameName(fileStr);
        
        animSprite->setAnchorPoint(Vec2(0.5, 0));
        
        this->addChild(animSprite);
        
        _animSprite = animSprite;

        return true;
    }
    else
    {
        return false;
    }
}

void TestSprite::playAction(PlayerActionType actionType){
    if (actionType == _lastActionType) {
        return;
    }
    
    if (_lastActionType != PlayerActionType::Invalid) {
        _animSprite->stopActionByTag((int)_lastActionType);
    }
    
    _lastActionType = actionType;
    Animation *animation = (Animation*)_playerAnimMap.at((int)actionType);

    if (animation != nullptr) {
        Animate *anim = Animate::create(animation);
        RepeatForever *repeat = RepeatForever::create(anim);
        
        repeat->setTag((int)actionType);
        _animSprite->runAction(repeat);
    }
}

