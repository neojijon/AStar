#include "FindPath.h"
//#include "head.h"

//-------------------------------------------------
// �ܺ��� ���� �ҷ���
// ����ڸ��� �ٸ��⶧���� �ٲ��ּ���
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
// ����ã�� PathList�� ������
// std::list<Vec2>�� �޾ƾ� ��
std::list<Vec2> FindPath::GetPath(Vec2 start, Vec2 end)
{
	this-> Release();
	
	//---------------------------------
	// ���� ��ġ�� �� ��ġ�� ������ Ȯ��
	// �ִٸ� ��ȯ
	if (Overlap_Start_End(start, end))
	{
		log("Find Path!!");
		return PathList;
	}

	//---------------------------------
	// ���� ó��, end���� �����Ѱ��� ������
	if (!Exception(end))
		return PathList;

	//----------------------------------
	// ���ٸ� Ž�� ����
	// ���� ��尡 �θ��尡 ��
	NodeInfo* _Parent = new NodeInfo(start, nullptr, 0, end);
	CloseList.push_back(_Parent);
	
	//----------------------------------
	// ��ã�� ����
	// ����Լ��� ���� -_-
	// �Ÿ��� ���� ������ �����ɸ��ų�, ����
	NodeInfo* FindPath = Recursive_FindPath(_Parent, end);

	//----------------------------------
	// FindPath�� Parent�� ������ ������
	while (FindPath->getParent() != NULL)
	{
		PathList.insert(PathList.begin(), FindPath->getPosition());
		FindPath = FindPath->getParent();
	}
	
	// ��Ȯ�ϰ� Path�� ������ Test
	for (auto it : PathList)
	{
		log("[%.f, %.f]", it.x, it.y);
	}
	
	// PathList�� ������
	return PathList;
}

//-------------------------------------------------
// ������ ����Լ��� �̿���
// ��ã�� �ݺ�
NodeInfo* FindPath::Recursive_FindPath(NodeInfo* parent, Vec2 end)
{
	//���� �ſ� ���� �߿��� �κ�
	// �������°�?
	if (Overlap_Start_End(parent->getPosition(), end))
	{
		log("Find Path!!"); // ��ã��!!
		return parent;
	}

	// �ֺ��� ������ �˻���
	for (int i = 0; i < 4; i++)
	{
		// �ֺ��� Position�� �޾ƿ�
		// Direction�� ����
		Vec2 _ChildPos = parent->getPosition() + Direction[i];

		// ���� ó��
		// ������ ������?, �ʿ� �������±濡 ��ҳ�?
		if (Exception(_ChildPos))
		{
			//������ ������ �Ÿ��� G�� �翬�� 1��, �ٷ� ���������ϱ�;;
			NodeInfo* _Child = new NodeInfo(_ChildPos, parent, parent->getCostG() + 1, end);
			//�ߺ� ���¸���Ʈ, Ŭ�����Ʈ �˻�
			AppOpenList(_Child);
		}
	}
	// ����Ϻ��� NodeInfo�� ã�� (F���� ������)
	NodeInfo* _Best = OpenList.front();
	for (auto it : OpenList)
	{
		// Best�� ���� OpenList�� �ִ� �༮���� ũ��
		// Best�� ���� ���� ������ �ٲ�
		if (_Best->getCostF() >= it->getCostF())
		{
			_Best = it;
		}
	}
	// ���� �̳��� �˻����ʿ���� �հ��̴� OpenList���� ����
	OpenList.remove(_Best);
	// CloseList�� ����
	CloseList.push_back(_Best);
	
	// ����Լ� ����
	// Best�� �������� ��� �����
	return Recursive_FindPath(_Best, end);
}

//-------------------------------------------------
// ��� List�� �ʱ�ȭ
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
// ������ OpenList�� Close����Ʈ�� NodeInfo�� �ִ��� �˻�
bool FindPath::AppOpenList(NodeInfo* _node)
{
	// CloseList �˻�
	for (auto it : CloseList)
	{
		if (Overlap_Start_End(it->getPosition(), _node->getPosition()))
		{
			return true;
		}
	}

	// OpenList �˻�
	for (auto it : OpenList)
	{
		if (Overlap_Start_End(it->getPosition(),_node->getPosition()))
		{
			// ���� �����ڸ��� �ִٸ� ���� F�� ���� ����
			// �� ���� F�� ���� ����
			if (it->getCostF() > _node->getCostF())
			{
				OpenList.remove(it);
				OpenList.push_back(_node);
				return true;
			}
		}
	}

	// �Ѵ� �Ȱ�ġ�� OpenList�� ����
	OpenList.push_back(_node);
	return true;
}

//-------------------------------------------------
// ����ó��
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

	// ���� ���������� �־
	// ���� ��ǥ�� �ݴ밡 �� ������������
	// 26 - pos.y�� �����Ͻñ� �ٶ��ϴ�.
	int Xpos = pos.x;
	int Ypos = 20 - pos.y;

	// �ʿ��� ���� ���� ��
	if (MapData[Ypos][Xpos] == 1 
		|| MapData[Ypos][Xpos] == 9)
	{
		log("Blick");
		return false;
	}

	return true;
}