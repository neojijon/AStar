/**
Made by Samvid
**/
#ifndef __NODE_CLASS_H__
#define __NODE_CLASS_H__

#include "cocos2d.h"
#include <queue>
#include <math.h>
#include <string>
#include "Location_Node_Class.h"
#include "queue"

#define IDIM 9
#define JDIM 7
#define NDIR 4

#define BLOCKSIZE 100

USING_NS_CC;
using namespace std;

class  Node_Class
{
public:

	static Node_Class* create();
	virtual bool init();

	/*
	static int iDir[NDIR];
	static int jDir[NDIR];
	*/
	const static int iDir[NDIR];
	const static int jDir[NDIR];



	int squares[IDIM][JDIM];
	int squares1[IDIM][JDIM];
	void empty_square_array(void);

	int closedNodes[IDIM][JDIM];

	int openNodes[IDIM][JDIM];

	int dirMap[IDIM][JDIM];

	int rPos;
    int cPos;

    int GValue;

    int FValue;

	Node_Class(const Location_Node_Class &loc, int g, int f)
            {rPos = loc.row; cPos = loc.col; GValue = g; FValue = f;}

	Node_Class() {rPos = 0; cPos = 0; GValue = 0; FValue = 0; }

		//Location_Node_Class getLocation();
		Location_Node_Class getLocation() const ;

		//int getGValue();
        int getGValue() const ;

		//int getFValue();
		int getFValue() const;

        //void calculateFValue(Location_Node_Class &locDest);
		void calculateFValue(const Location_Node_Class& locDest);

		//void updateGValue(int & i);
		void updateGValue(const int & i);

		//int & getHValue(Location_Node_Class &locDest);
		const int & getHValue(const Location_Node_Class& locDest) const;
		friend bool operator<(const Node_Class & a,const Node_Class & b)
		{
			return a.getFValue() > b.getFValue();
		}

		std::string pathFind(const Location_Node_Class &locStart ,const Location_Node_Class &locFinish );

};
#endif
