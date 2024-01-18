#include "MapInfo.h"

MapInfo::MapInfo(void){
    _mapGridSize = Size(0, 0);
    _pointSize = Size(0, 0);
    
    m_nKeyOffset = 10000;
    
    srand((unsigned)time(NULL));
}

MapInfo::~MapInfo(void){
    
}

bool MapInfo::init(const std::string& filename){
    
    auto map = TMXTiledMap::create(filename);
    
    auto objectGrp = map->getObjectGroup("object");
    
    auto layer = map->getLayer("background");
    
    _mapGridSize = layer->getLayerSize();
    
    auto& objects = objectGrp->getObjects();
    
    int capacity = _mapGridSize.width * _mapGridSize.height;
    
    ValueMap& dict = objects.at(0).asValueMap();
    _pointSize = Size(dict["width"].asFloat(), dict["height"].asFloat());
    
    int *mapType =  new int[capacity];
    
    for (auto& obj : objects) {
        ValueMap& dict = obj.asValueMap();
        
        float x = dict["x"].asFloat() / _pointSize.width;
        float y = dict["y"].asFloat() / _pointSize.height;
        
        int objectId = dict["objectid"].asInt();
        
        int mapId = x + y * _mapGridSize.width;
        
        mapType[mapId] = objectId;
    }
    
    for (int i = 0; i < capacity; i++) {
        int num = mapType[i];
        
        CCASSERT(num != 0, "objectId not set");
        
        Value pNum = Value(num);
        _mapInfoTypeVec.push_back(pNum); 
    }
    return true;
}

#pragma -mark- ------Public Method------

MapInfo* MapInfo::create(const std::string& filename){
    MapInfo *pMapInfo = new MapInfo();
    if (pMapInfo && pMapInfo->init(filename))
    {
        pMapInfo->autorelease();
        return pMapInfo;
    }
    CC_SAFE_DELETE(pMapInfo);
    return nullptr;
}

#pragma -mark- Get Method

MapPath* MapInfo::getMapPath(int startId, int endId){
    m_nStartIndex = startId;
    m_nEndIndex = endId;
    
    MapPath* path = this->getPathFromCache();
    
    if (path == nullptr) {
        path = this->findPath();
    }
    return path;
}

PointArray* MapInfo::getPointPath(int startId, int endId){
    return getMapPath(startId, endId)->getPointArr();
}

ValueVector MapInfo::getMapInfoTypeVec(MapInfoType type){
    
    int typeNum = (int)type;

    ValueVector typeVec = _mapInfoTypeMap[typeNum].asValueVector();

    if (typeVec.empty()) {
        
        int i = 0;
        for (auto& value: _mapInfoTypeVec) {
            int testNum = value.asInt();
            if (testNum == typeNum) {
                typeVec.push_back(Value(i));
            }
            i++;
        }
        
        _mapInfoTypeMap[typeNum] = typeVec;
    }
    
    return typeVec;
}

int MapInfo::getRandomMapIdByType(MapInfoType type){
    int mapId = -1;
    
    auto roadVec = this->getMapInfoTypeVec(type);
    
    if (!roadVec.empty()) {
        int count = roadVec.size();
        
        float r = CCRANDOM_0_1();
        
        if (r == 1)         
        {
            r = 0;
        }
        
        int randomTarget = r * count;
        
        auto value = roadVec.at(randomTarget);
        
        mapId = value.asInt();
    }else{
        log("Warning: map id invalid, error type or type didnot exist");
    }
    
    return mapId;
}

Vec2 MapInfo::getRandomPointByType(MapInfoType type){
    int mapId = getRandomMapIdByType(type);
    return convertIdToPoint(mapId);
}

Vec2 MapInfo::getRandomPointMidByType(MapInfoType type){
    int mapId = getRandomMapIdByType(type);
    return convertIdToPointMid(mapId);
}

#pragma -mark- Convert Method

int MapInfo::convertPointToId(Vec2 point){
    int mapId = -1;
    if (Rect(0, 0, _mapGridSize.width * _pointSize.width, _mapGridSize.height * _pointSize.height).containsPoint(point)) {
        int x = point.x / _pointSize.width;
        int y = point.y / _pointSize.height;
        mapId = x + y * _mapGridSize.width;
    }
    return mapId;
}

Vec2 MapInfo::convertIdToPoint(int mapId){
    Vec2 point = Vec2(0, 0);
    
    if (mapId >= 0 && mapId < _mapInfoTypeVec.size()) {
        int y = (mapId / (int)_mapGridSize.width); 
        int x = (mapId - y * _mapGridSize.width) ; 
        
        point = Vec2(x  * _pointSize.width, y * _pointSize.height);
    }
    return point;
}

Vec2 MapInfo::convertIdToPointMid(int mapId){
    Vec2 point = convertIdToPoint(mapId);
    return Vec2(point.x + _pointSize.width * 0.5f, point.y + _pointSize.height * 0.5f);
}

#pragma -mark- ------Private Method------

#pragma -mark- find Path Method

MapPath* MapInfo::findPath(){
    std::vector<PointNode*> vecClose;
    std::vector<PointNode*> vecOpen;
    
    if (m_nStartIndex == -1 || m_nEndIndex == -1)
    {
        return nullptr;
    }
    
    auto &pointTypeStart = _mapInfoTypeVec.at(m_nStartIndex); 
    if (pointTypeStart.asInt() == (int)MapInfoType::Block)     
    {
        return nullptr;
    }
    
    auto &pointTypeEnd = _mapInfoTypeVec.at(m_nEndIndex); 
    if (pointTypeEnd.asInt() == (int)MapInfoType::Block)     
    {
        return nullptr;
    }
    
    PointNode* pNode = new PointNode;
    pNode->nIndex = m_nStartIndex;
    vecClose.push_back(pNode);
    
    int nStep = 0;
    while(true)
    {
        if (nStep++ >= MAPINFO_MAX_STEP_NUM)
        {
            break;
        }
        PointNode* pNextNode = vecClose[vecClose.size() - 1];
        if (!pNextNode)
        {
            break;
        }
        if (pNextNode->nIndex == m_nEndIndex)
        {
            break;
        }
        
        for (int i = 0; i < MAPINFO_ASTAR_DIRECTION_NUM; i++)
        {
            int nIndex = GetIndexByDir(pNextNode->nIndex, i);
            if (-1 == nIndex)
            {
                continue;
            }
            
            auto &pointType = _mapInfoTypeVec.at(nIndex); 
            if (pointType.asInt() == (int)MapInfoType::Block)     
            {
                continue;
            }
            if (InTable(nIndex, vecClose) != nullptr)       
            {
                continue;
            }
            
            PointNode* pNode = InTable(nIndex, vecOpen);      
            if (pNode)
            {
                int nNewG = pNextNode->nG + GetGByIndex(pNextNode->nIndex, pNode->nIndex);
                if (pNode->nG > nNewG)
                {
                    pNode->nG = nNewG;
                    pNode->pParent = pNextNode;
                }
                continue;
            }
            
            pNode = new PointNode;
            pNode->nIndex = nIndex;
            pNode->nG = pNextNode->nG + GetGByIndex(pNextNode->nIndex, pNode->nIndex);
            pNode->pParent = pNextNode;
            vecOpen.push_back(pNode);
        }
        
        int nMinF = 0xFFFFFF;
        pNextNode = nullptr;
        int nNextNodeIndex = 0;
        for (int i = 0; i < (int)vecOpen.size(); i++)
        {
            PointNode* pNode = vecOpen[i];
            if (!pNode)
            {
                continue;
            }
            int nH = GetHByIndex(pNode->nIndex);
            int nF = nH + pNode->nG;
            if (nF < nMinF)
            {
                nMinF = nF;
                pNextNode = pNode;
                nNextNodeIndex = i;
            }
        }
        if (nNextNodeIndex >= 0 && nNextNodeIndex < (int)vecOpen.size())
        {
            vecClose.push_back(pNextNode);
            vecOpen.erase(vecOpen.begin() + nNextNodeIndex);
        }
    }
    
    pNode = vecClose[vecClose.size() - 1];
    
    PointArray *pathArr = PointArray::create(0);
 
    while (pNode)
    {
        
        int mapId = pNode->nIndex;
        Vec2 point = this->convertIdToPointMid(mapId);   
        
        pathArr->addControlPoint(point);
        
        pNode = pNode->pParent;
    }
    
    
    MapPath* pathRevert = MapPath::create(m_nEndIndex, m_nStartIndex , pathArr);
    MapPath* path = pathRevert->getRevertPath(); 
    
    int keyRevert = m_nEndIndex * m_nKeyOffset + m_nStartIndex;
    int key = m_nStartIndex * m_nKeyOffset + m_nEndIndex;
    
    int cacheSize = _mapPathCacheMap.size();
    
    if (cacheSize > MAPINFO_MAX_STORE_PATH_SIZE) {
        _mapPathCacheMap.clear(); 
    }
    
    _mapPathCacheMap.insert(key, path);
    _mapPathCacheMap.insert(keyRevert, pathRevert);

    do {
        for (std::vector<PointNode *>::iterator it = vecClose.begin(); it != vecClose.end(); it ++)
            if (NULL != *it)
            {
                delete *it;
                *it = NULL;
            }
        vecClose.clear();
    } while (0);
    
    do {
        for (std::vector<PointNode *>::iterator it = vecOpen.begin(); it != vecOpen.end(); it ++)
            if (NULL != *it)
            {
                delete *it;
                *it = NULL;
            }
        vecOpen.clear();
    } while (0);
    
    m_nStartIndex = -1;
    m_nEndIndex = -1;
    
    return path->getCopy();
}

MapPath* MapInfo::getPathFromCache(){
    auto path = _mapPathCacheMap.at(m_nStartIndex * m_nKeyOffset + m_nEndIndex);
    
    if (path == nullptr) {
        return nullptr;
    }else{
        return (MapPath*)path->getCopy();
    }
    
}

#pragma -mark- A Star Method

int MapInfo::GetIndexByDir(int nIndex, int nDir)
{
    if (nIndex < 0 || nIndex >= (int)_mapGridSize.width * _mapGridSize.height)
    {
        return -1;
    }
    
    int nRow = nIndex / _mapGridSize.width;
    int nCol = nIndex % (int)_mapGridSize.width;
    
    switch(nDir)
    {
        case 0:      
            nRow += 1;
            break;
        case 1:      
            nCol += 1;
            break;
        case 2:      
            nRow -= 1;
            break;
        case 3:      
            nCol -= 1;
            break;
        case 4:      
            nRow += 1;
            nCol +=1;
            break;
        case 5:      
            nRow -= 1;
            nCol += 1;
            break;
            
        case 6:      
            nRow -= 1;
            nCol -= 1;
            break;
            
        case 7:      
            nRow += 1;
            nCol -= 1;
            break;
        default:
            break;
    }
    if (nRow < 0 || nRow >= _mapGridSize.height
        || nCol < 0 || nCol >= _mapGridSize.width)
    {
        return -1;
    }
    return nRow * _mapGridSize.width + nCol;
}

int MapInfo::GetGByIndex(int nStartIndex, int nEndIndex){
    int nStartRow = nStartIndex / _mapGridSize.width;
    int nStartCol = nStartIndex %  (int)_mapGridSize.width;
    
    int nEndRow = nEndIndex /  _mapGridSize.width;
    int nEndCol = nEndIndex % (int)_mapGridSize.width;
    
    if (nStartRow == nEndRow || nStartCol == nEndCol)
    {
        return 10;
    }
    return 14;
}

int MapInfo::GetHByIndex(int nIndex)
{
    int nRow = nIndex / _mapGridSize.width;
    int nCol = nIndex % (int)_mapGridSize.width;
    
    int nEndRow = m_nEndIndex / _mapGridSize.width;
    int nEndCol = m_nEndIndex % (int)_mapGridSize.width;
    
    return (abs(nEndRow - nRow) + abs(nEndCol - nCol))*10;
}

MapInfo::PointNode *MapInfo::InTable(int nIndex, std::vector<PointNode*> &vecTbl)
{
    for (int i = 0; i < (int)vecTbl.size(); i++)
    {
        if (nIndex == vecTbl[i]->nIndex)
        {
            return vecTbl[i];
        }
    }
    return nullptr;
}

