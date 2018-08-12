#include "..\Level.h"

//-------------------------------------------------------------------------//

le::Plane::Plane() :
	StartIndex( 0 ),
	NumberIndices( 0 ),
	LightmapID( 0 )
{}

//-------------------------------------------------------------------------//

void le::Plane::InitPlane( BSPFace& Face )
{
	StartIndex = Face.StartIndex;
	NumberIndices = Face.NumOfIndices;

	LightmapID = Face.LightmapID;
}

//-------------------------------------------------------------------------//