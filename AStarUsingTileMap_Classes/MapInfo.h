#ifndef __testTileMap__MapInfo__
#define __testTileMap__MapInfo__

#include "cocos2d.h"
#include "MapPath.h"
#include "MapGeneral.h"
USING_NS_CC;

class MapInfo : public Node{
public:
    static MapInfo* create(const std::string& filename);
    
    MapPath* getMapPath(int startId, int endId);
    
    PointArray* getPointPath(int startId, int endId);
    
    ValueVector getMapInfoTypeVec(MapInfoType type);
    
    int getRandomMapIdByType(MapInfoType type);
    
    Vec2 getRandomPointByType(MapInfoType type);
    
    Vec2 getRandomPointMidByType(MapInfoType type);
    
    int convertPointToId(Vec2 point);
    
    Vec2 convertIdToPoint(int mapId);
    
    Vec2 convertIdToPointMid(int mapId);
    
    CC_SYNTHESIZE_READONLY(Size, _mapGridSize, MapGridSize);
    CC_SYNTHESIZE_READONLY(Size, _pointSize, PointSize);

private:
    struct PointNode
    {
        PointNode()  {nIndex = 0; nG = 0; pParent = NULL;}
        int nIndex;
        int nG;
        PointNode* pParent;
    };
    
private:
    MapInfo(void);
    
    virtual ~MapInfo(void);
    
    virtual bool init(const std::string& filename);
    
    int GetIndexByDir(int nIndex, int nDir);
    int GetGByIndex(int nStartIndex, int nEndIndex);
    int GetHByIndex(int nIndex);
    PointNode *InTable(int nIndex, std::vector<PointNode*> &vecTbl);
    
    MapPath *findPath();
    MapPath *getPathFromCache();
 
private:
    int m_nStartIndex;
    int m_nEndIndex;
    
    ValueVector _mapInfoTypeVec; 
    
    ValueMapIntKey _mapInfoTypeMap;
    
    Map<int, MapPath*> _mapPathCacheMap; 
    
    int m_nKeyOffset;
    
};

#endif   
