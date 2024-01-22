//#pragma once
#ifndef __FINDPATH_H_
#define __FINDPATH_H_
/*��ã�⿡ ���� Nodeinfo
cocos�� vec2��
G�� H�� ���� ������, F�� �Լ��� ����
�θ� NodeInfo�� ����*/
//#include "head.h"
#include "cocos2d.h"
#include "NodeInfo.h"
#include <vector>
#include <list>

USING_NS_CC;
//using namespace std;
/*�ζ������� ����
overlap ���״�� �������°�?
�������� ������ �������� ���� ã�Ҵٴ°�*/
class FindPath
{
private:
	//-------------------------------------------------
	// ���� ����Ʈ
	// �ֺ��� �����ִ� ����� ����ϴ�
	std::list<NodeInfo*> OpenList;
	//-------------------------------------------------
	// Ŭ���� ����Ʈ
	// ���¸���Ʈ���� F�� ���� ���� ���� NodeInfo�� ����ϴ�
	// (�����ִ� ��)
	std::list<NodeInfo*> CloseList;
	//-------------------------------------------------
	// �����ִ±��� NodeInfo�� parent�� ������ ������
	// ó������ ������� ���� �����մϴ�
	// EX> CloseList = 5,4,3,2,1
	// EX> PathList  = 1,2,3,4,5
	std::list<Vec2> PathList;
	//-------------------------------------------------
	// 4������ ������ �������, 8���⵵ ����, ���� 6���⵵ ����
	// UP			0,1
	// RIGHT		1,0
	// DOWN			0,-1
	// LEFT			-1,0
	Vec2 Direction[4];
	//-------------------------------------------------
	// ���� ������ �������
	// ����ڸ��� �޸� �������ֽñ� �ٶ��ϴ�.
	bool MapData[20][20];
public:
	//-------------------------------------------------
	// Direction ����, ������ �ʱ�ȭ
	FindPath()
	{
		Direction[0] = Vec2(0, 1);  // UP
		Direction[1] = Vec2(1, 0);  // RIGHT
		Direction[2] = Vec2(0, -1); // DOWN
		Direction[3] = Vec2(-1, 0); // LEFT
	};
	//-------------------------------------------------
	// �Ҹ���
	~FindPath() {};

	//-------------------------------------------------
	// �ܺ��� ���� �޾ƿ�
	void GetMap(TMXTiledMap* map);

	//-------------------------------------------------
	// ���� �޾ƿ��� ���� �Լ�
	std::list<Vec2> GetPath(Vec2 start, Vec2 end);

private:
	// ���� OpenList�� ���� �������� ��尡 �ִ��� �˻�
	bool AppOpenList(NodeInfo* _node);
	// �ֺ��� ���� Ž���� �ݺ��ϴ� �Լ�
	// (����Լ�) Recursive
	NodeInfo* Recursive_FindPath(NodeInfo* parent, Vec2 end);
	// OpenList,CloseList,PathList�� �ʱ�ȭ��
	void Release();
	// ���� ó��
	// ȭ����� �����ٰų�, �ʿ� ����ȵǴ±��� Ž�������� false
	bool Exception(Vec2 pos);


};

#endif
