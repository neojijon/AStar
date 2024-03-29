#include "FindPath.h"
//#include "head.h"

//-------------------------------------------------
// 須採税 己聖 災君身
// 紫遂切原陥 陥牽奄凶庚拭 郊蚊爽室推
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
// 掩聖達壱 PathList研 軒渡敗
// std::list<Vec2>稽 閤焼醤 敗
std::list<Vec2> FindPath::GetPath(Vec2 start, Vec2 end)
{
	this-> Release();
	
	//---------------------------------
	// 薄仙 是帖人 魁 是帖亜 旭精走 溌昔
	// 赤陥檎 鋼発
	if (Overlap_Start_End(start, end))
	{
		log("Find Path!!");
		return PathList;
	}

	//---------------------------------
	// 森須 坦軒, end葵戚 錠琴廃員拭 級嬢穐聖凶
	if (!Exception(end))
		return PathList;

	//----------------------------------
	// 蒸陥檎 貼事 獣拙
	// 薄仙 葛球亜 採乞葛球亜 喫
	NodeInfo* _Parent = new NodeInfo(start, nullptr, 0, end);
	CloseList.push_back(_Parent);
	
	//----------------------------------
	// 掩達奄 獣拙
	// 仙瑛敗呪研 宜顕 -_-
	// 暗軒拭 魚虞辞 希郡惟 神掘杏軒暗蟹, 斗像
	NodeInfo* FindPath = Recursive_FindPath(_Parent, end);

	//----------------------------------
	// FindPath税 Parent研 蝕生稽 宜鍵陥
	while (FindPath->getParent() != NULL)
	{
		PathList.insert(PathList.begin(), FindPath->getPosition());
		FindPath = FindPath->getParent();
	}
	
	// 舛溌馬惟 Path亜 級嬢穐澗走 Test
	for (auto it : PathList)
	{
		log("[%.f, %.f]", it.x, it.y);
	}
	
	// PathList研 軒渡敗
	return PathList;
}

//-------------------------------------------------
// 因匂税 仙瑛敗呪研 戚遂廃
// 掩達奄 鋼差
NodeInfo* FindPath::Recursive_FindPath(NodeInfo* parent, Vec2 end)
{
	//焼爽 古酔 弦戚 掻推廃 採歳
	// 違団然澗亜?
	if (Overlap_Start_End(parent->getPosition(), end))
	{
		log("Find Path!!"); // 掩達製!!
		return parent;
	}

	// 爽痕税 葛球級聖 伊事敗
	for (int i = 0; i < 4; i++)
	{
		// 爽痕税 Position聖 閤焼身
		// Direction聖 希敗
		Vec2 _ChildPos = parent->getPosition() + Direction[i];

		// 森須 坦軒
		// 鉱生稽 蟹穐蟹?, 己拭 哀呪蒸澗掩拭 願紹蟹?
		if (Exception(_ChildPos))
		{
			//獣拙繊 採斗税 暗軒昔 G澗 雁尻備 1績, 郊稽 新拭赤生艦猿;;
			NodeInfo* _Child = new NodeInfo(_ChildPos, parent, parent->getCostG() + 1, end);
			//掻差 神蚤軒什闘, 適稽綜軒什闘 伊紫
			AppOpenList(_Child);
		}
	}
	// 亜舌刃混廃 NodeInfo研 達製 (F葵戚 拙精暗)
	NodeInfo* _Best = OpenList.front();
	for (auto it : OpenList)
	{
		// Best税 葵戚 OpenList拭 赤澗 橿汐左陥 滴檎
		// Best澗 薦析 拙精 叶生稽 郊会
		if (_Best->getCostF() >= it->getCostF())
		{
			_Best = it;
		}
	}
	// 戚薦 戚叶精 伊紫拝琶推蒸戚 杯維戚艦 OpenList拭辞 皐壱
	OpenList.remove(_Best);
	// CloseList拭 隔製
	CloseList.push_back(_Best);
	
	// 仙瑛敗呪 獣拙
	// Best研 奄層生稽 域紗 燦嬢蟹姶
	return Recursive_FindPath(_Best, end);
}

//-------------------------------------------------
// 乞窮 List研 段奄鉢
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
// 奄糎拭 OpenList人 Close軒什闘拭 NodeInfo亜 赤澗走 伊紫
bool FindPath::AppOpenList(NodeInfo* _node)
{
	// CloseList 伊紫
	for (auto it : CloseList)
	{
		if (Overlap_Start_End(it->getPosition(), _node->getPosition()))
		{
			return true;
		}
	}

	// OpenList 伊紫
	for (auto it : OpenList)
	{
		if (Overlap_Start_End(it->getPosition(),_node->getPosition()))
		{
			// 幻鉦 旭精切軒拭 赤陥檎 辞稽 F税 葵聖 搾嘘敗
			// 希 拙精 F税 葵聖 隔製
			if (it->getCostF() > _node->getCostF())
			{
				OpenList.remove(it);
				OpenList.push_back(_node);
				return true;
			}
		}
	}

	// 却陥 照違帖檎 OpenList拭 隔製
	OpenList.push_back(_node);
	return true;
}

//-------------------------------------------------
// 森須坦軒
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

	// 鎧亜 是拭辞採斗 隔嬢辞
	// 己税 疎妊亜 鋼企亜 喫 せせせせせせ
	// 26 - pos.y澗 巷獣馬獣掩 郊遇艦陥.
	int Xpos = pos.x;
	int Ypos = 20 - pos.y;

	// 己拭辞 哀呪 蒸澗 員
	if (MapData[Ypos][Xpos] == 1 
		|| MapData[Ypos][Xpos] == 9)
	{
		log("Blick");
		return false;
	}

	return true;
}