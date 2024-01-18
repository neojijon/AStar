/**
Made by Samvid
**/

#ifndef __LOCATION_NODE_CLASS_H__
#define __LOCATION_NODE_CLASS_H__

#include "cocos2d.h"

class Location_Node_Class
{
public:
	int row,col;

	Location_Node_Class()
	{
		row=col=0;
	}
	Location_Node_Class(int r,int c)
	{
		row=r;
		col=c;
	}

};

#endif
