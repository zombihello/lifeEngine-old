#include "..\AI_Route.h"

//-------------------------------------------------------------------------//

le::AI_Route::AI_Route()
{

}

//-------------------------------------------------------------------------//

le::AI_Route::AI_Route( FloatRect Rect , bool NotUseY )
{
	RectRoute = Rect;
	bNotUseY = NotUseY;
}

//-------------------------------------------------------------------------//

le::AI_Route::AI_Route( FloatRect Rect , string NextRoute , bool NotUseY )
{
	RectRoute = Rect;
	sNextRoute = NextRoute;
	bNotUseY = NotUseY;
}

//-------------------------------------------------------------------------//

string le::AI_Route::GetNextRoute()
{
	return sNextRoute;
}

//-------------------------------------------------------------------------//

bool le::AI_Route::IsUsedY()
{
	return bNotUseY;
}

//-------------------------------------------------------------------------//

FloatRect le::AI_Route::GetRect()
{
	return RectRoute;
}

//-------------------------------------------------------------------------//