#ifndef LAYER
#define LAYER

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	struct Layer
	{
		int                 iOpacity;
		vector<Sprite>      tiles;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LAYER

