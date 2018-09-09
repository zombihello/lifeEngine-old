#include "..\Ray.h"

//-------------------------------------------------------------------------//

bool le::Ray::operator==( Ray& Ray )
{
	return 
		TraceType == Ray.TraceType &&
		TraceRadius == Ray.TraceRadius &&
		Start == Ray.Start &&
		End == Ray.End;
}

//-------------------------------------------------------------------------//