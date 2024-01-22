//#pragma once
#ifndef __FINDPATH_H_
#define __FINDPATH_H_
/*길찾기에 사용됳 Nodeinfo
cocos의 vec2와
G와 H의 값을 가진다, F는 함수에 있음
부모 NodeInfo를 가짐*/
//#include "head.h"
#include "cocos2d.h"
#include "NodeInfo.h"
#include <vector>
#include <list>

USING_NS_CC;
//using namespace std;
/*인라인으로 선언
overlap 말그대로 겹쳐졌는가?
시작점과 끝점이 겹쳤으면 길을 찾았다는것*/
class FindPath
{
private:
	//-------------------------------------------------
	// 오픈 리스트
	// 주변에 갈수있는 길들을 담습니다
	std::list<NodeInfo*> OpenList;
	//-------------------------------------------------
	// 클로즈 리스트
	// 오픈리스트에서 F의 값이 가장 작은 NodeInfo를 담습니다
	// (갈수있는 길)
	std::list<NodeInfo*> CloseList;
	//-------------------------------------------------
	// 갈수있는길의 NodeInfo의 parent를 역으로 돌려서
	// 처음부터 사용자의 길을 나열합니다
	// EX> CloseList = 5,4,3,2,1
	// EX> PathList  = 1,2,3,4,5
	std::list<Vec2> PathList;
	//-------------------------------------------------
	// 4방향의 정보를 담고있음, 8방향도 가능, 물론 6방향도 ㅇㅇ
	// UP			0,1
	// RIGHT		1,0
	// DOWN			0,-1
	// LEFT			-1,0
	Vec2 Direction[4];
	//-------------------------------------------------
	// 맵의 정보를 담고있음
	// 사용자마다 달리 설정해주시길 바랍니다.
	bool MapData[20][20];
public:
	//-------------------------------------------------
	// Direction 정의, 생성자 초기화
	FindPath()
	{
		Direction[0] = Vec2(0, 1);  // UP
		Direction[1] = Vec2(1, 0);  // RIGHT
		Direction[2] = Vec2(0, -1); // DOWN
		Direction[3] = Vec2(-1, 0); // LEFT
	};
	//-------------------------------------------------
	// 소멸자
	~FindPath() {};

	//-------------------------------------------------
	// 외부의 맵을 받아옴
	void GetMap(TMXTiledMap* map);

	//-------------------------------------------------
	// 길을 받아오는 메인 함수
	std::list<Vec2> GetPath(Vec2 start, Vec2 end);

private:
	// 기존 OpenList에 현재 넣으려는 노드가 있는지 검사
	bool AppOpenList(NodeInfo* _node);
	// 주변의 길의 탐색을 반복하는 함수
	// (재귀함수) Recursive
	NodeInfo* Recursive_FindPath(NodeInfo* parent, Vec2 end);
	// OpenList,CloseList,PathList를 초기화함
	void Release();
	// 예외 처리
	// 화면밖을 나간다거나, 맵에 가면안되는길을 탐색했을때 false
	bool Exception(Vec2 pos);


};

#endif
