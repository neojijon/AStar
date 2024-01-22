#include "FindPath.h"
//#include "head.h"

//-------------------------------------------------
// 외부의 맵을 불러옴
// 사용자마다 다르기때문에 바꿔주세요
void FindPath::GetMap(TMXTiledMap* map)
{
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			TMXLayer* metainfo = map->getLayer("meta");
			int tileGid = metainfo->getTileGIDAt(Vec2(x, y));
			if (tileGid)
			{
				Value& properties = map->getPropertiesForGID(tileGid);
				if (!properties.isNull())
				{
					MapData[y][x] = properties.asValueMap()["wall"].asBool();
				}
			}
		}
	}
}

//-------------------------------------------------
// 길을찾고 PathList를 리턴함
// std::list<Vec2>로 받아야 함
std::list<Vec2> FindPath::GetPath(Vec2 start, Vec2 end)
{
	this-> Release();
	
	//---------------------------------
	// 현재 위치와 끝 위치가 같은지 확인
	// 있다면 반환
	if (Overlap_Start_End(start, end))
	{
		log("Find Path!!");
		return PathList;
	}

	//---------------------------------
	// 예외 처리, end값이 엉뚱한곳에 들어갔을때
	if (!Exception(end))
		return PathList;

	//----------------------------------
	// 없다면 탐색 시작
	// 현재 노드가 부모노드가 됨
	NodeInfo* _Parent = new NodeInfo(start, nullptr, 0, end);
	CloseList.push_back(_Parent);
	
	//----------------------------------
	// 길찾기 시작
	// 재귀함수를 돌림 -_-
	// 거리에 따라서 더럽게 오래걸리거나, 터짐
	NodeInfo* FindPath = Recursive_FindPath(_Parent, end);

	//----------------------------------
	// FindPath의 Parent를 역으로 돌린다
	while (FindPath->getParent() != NULL)
	{
		PathList.insert(PathList.begin(), FindPath->getPosition());
		FindPath = FindPath->getParent();
	}
	
	// 정확하게 Path가 들어갔는지 Test
	for (auto it : PathList)
	{
		log("[%.f, %.f]", it.x, it.y);
	}
	
	// PathList를 리턴함
	return PathList;
}

//-------------------------------------------------
// 공포의 재귀함수를 이용한
// 길찾기 반복
NodeInfo* FindPath::Recursive_FindPath(NodeInfo* parent, Vec2 end)
{
	//아주 매우 많이 중요한 부분
	// 겹쳐졌는가?
	if (Overlap_Start_End(parent->getPosition(), end))
	{
		log("Find Path!!"); // 길찾음!!
		return parent;
	}

	// 주변의 노드들을 검색함
	for (int i = 0; i < 4; i++)
	{
		// 주변의 Position을 받아옴
		// Direction을 더함
		Vec2 _ChildPos = parent->getPosition() + Direction[i];

		// 예외 처리
		// 밖으로 나갔나?, 맵에 갈수없는길에 닿았나?
		if (Exception(_ChildPos))
		{
			//시작점 부터의 거리인 G는 당연히 1임, 바로 옆에있으니까;;
			NodeInfo* _Child = new NodeInfo(_ChildPos, parent, parent->getCostG() + 1, end);
			//중복 오픈리스트, 클로즈리스트 검사
			AppOpenList(_Child);
		}
	}
	// 가장완벽한 NodeInfo를 찾음 (F값이 작은거)
	NodeInfo* _Best = OpenList.front();
	for (auto it : OpenList)
	{
		// Best의 값이 OpenList에 있는 녀석보다 크면
		// Best는 제일 작은 놈으로 바뀜
		if (_Best->getCostF() >= it->getCostF())
		{
			_Best = it;
		}
	}
	// 이제 이놈은 검사할필요없이 합격이니 OpenList에서 빼고
	OpenList.remove(_Best);
	// CloseList에 넣음
	CloseList.push_back(_Best);
	
	// 재귀함수 시작
	// Best를 기준으로 계속 뻗어나감
	return Recursive_FindPath(_Best, end);
}

//-------------------------------------------------
// 모든 List를 초기화
void FindPath::Release()
{
	if (OpenList.size())
		OpenList.clear();

	if (CloseList.size())
		CloseList.clear();

	if (PathList.size())
		PathList.clear();
}

//-------------------------------------------------
// 기존에 OpenList와 Close리스트에 NodeInfo가 있는지 검사
bool FindPath::AppOpenList(NodeInfo* _node)
{
	// CloseList 검사
	for (auto it : CloseList)
	{
		if (Overlap_Start_End(it->getPosition(), _node->getPosition()))
		{
			return true;
		}
	}

	// OpenList 검사
	for (auto it : OpenList)
	{
		if (Overlap_Start_End(it->getPosition(),_node->getPosition()))
		{
			// 만약 같은자리에 있다면 서로 F의 값을 비교함
			// 더 작은 F의 값을 넣음
			if (it->getCostF() > _node->getCostF())
			{
				OpenList.remove(it);
				OpenList.push_back(_node);
				return true;
			}
		}
	}

	// 둘다 안겹치면 OpenList에 넣음
	OpenList.push_back(_node);
	return true;
}

//-------------------------------------------------
// 예외처리
bool FindPath::Exception(Vec2 pos)
{
	if (pos.x < 0 || pos.x > 320/16)
	{
		//log("X Exeption");
		return false;
	}

	if (pos.y < 0 || pos.y > 320 / 16)
	{
		//log("Y Exeption");
		return false;
	}

	// 내가 위에서부터 넣어서
	// 맵의 좌표가 반대가 됨 ㅋㅋㅋㅋㅋㅋ
	// 26 - pos.y는 무시하시길 바랍니다.
	int Xpos = pos.x;
	int Ypos = 20 - pos.y;

	// 맵에서 갈수 없는 곳
	if (MapData[Ypos][Xpos] == 1 
		|| MapData[Ypos][Xpos] == 9)
	{
		log("Blick");
		return false;
	}

	return true;
}