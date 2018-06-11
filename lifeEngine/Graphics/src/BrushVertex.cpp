#include "../BrushVertex.h"

//-------------------------------------------------------------------------//

bool le::BrushVertex::operator==( BrushVertex& BrushVertex )
{
	return 
		Position == BrushVertex.Position && 
		Normal == BrushVertex.Normal && 
		TextureCoord_DiffuseMap == BrushVertex.TextureCoord_DiffuseMap &&
		TextureCoord_LightMap == BrushVertex.TextureCoord_LightMap;
}

//-------------------------------------------------------------------------//