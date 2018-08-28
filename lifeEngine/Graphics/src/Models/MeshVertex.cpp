#include <Graphics\Models\MeshVertex.h>

//-------------------------------------------------------------------------//

bool le::MeshVertex::operator==( MeshVertex& MeshVertex )
{
	return Position == MeshVertex.Position &&
		Normal == MeshVertex.Normal &&
		TextureCoord == MeshVertex.TextureCoord;
}

//-------------------------------------------------------------------------//