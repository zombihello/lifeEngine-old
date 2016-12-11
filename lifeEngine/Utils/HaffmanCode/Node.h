#ifndef NODE_H
#define NODE_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//-------------------------------------------------------------------------//

struct DLL_API Node
{
	Node();
	Node( Node *Left , Node *Right );

	int			iCount;
	char		_char;
	Node*		left;
	Node*		right;
};

//-------------------------------------------------------------------------//

#endif // NODE_H