#ifndef GROUPCOLISION_H
#define GROUPCOLISION_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

typedef unsigned short uint16;

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API GroupColision
	{
		////////////////
		/// КОНСТРУКТОР
		////////////////
		GroupColision();
		GroupColision( uint16 ColisionGroup, uint16 IgnoreGroup );

		uint16			ColisionGroup;
		uint16			IgnoreGroup;
	};

	//-------------------------------------------------------------------------//
}

#endif // GROUPCOLISION_H
