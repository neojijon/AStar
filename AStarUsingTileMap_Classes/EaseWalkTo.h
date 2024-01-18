#ifndef __testAStar__EaseWalkTo__
#define __testAStar__EaseWalkTo__

#include "cocos2d.h"
#include "MapGeneral.h"
USING_NS_CC;

class EaseWalkTo : public ActionInterval
{
public:
    
    static EaseWalkTo* create(float duration, PointArray* pointArr);
    
    virtual EaseWalkTo* clone() const override;
	virtual EaseWalkTo* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
protected:
    EaseWalkTo();
    virtual ~EaseWalkTo();
    bool initWithDuration(float duration, PointArray* pointArr);
    
    void playActionByPos(Vec2 start, Vec2 end, Node *target);
    
    CC_SYNTHESIZE_RETAIN(PointArray*, _pointArr, PointArr);
    
    int _nIndex;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(EaseWalkTo);
};

#endif   
