#include "Node.h"

//-------------------------------------------------------------------------//

Node::Node()
{
	left = 0;
	right = 0;
}

//-------------------------------------------------------------------------//

Node::Node( Node *Left , Node *Right  )
{
	left = Left;
	right = Right;
	iCount = Left->iCount + Right->iCount;
}

//-------------------------------------------------------------------------//
