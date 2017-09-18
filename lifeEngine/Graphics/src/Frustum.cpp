#include "..\Frustum.h"
#include <Graphics\BoundingBox.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

void le::Frustum::UpdateFrustum( const glm::mat4& Projection, const glm::mat4& View )
{
	glm::mat4 ClippingPlane = glm::transpose( Projection * View );

	PyramidFrustum[ Right ][ A ] = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].x;
	PyramidFrustum[ Right ][ B ] = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].x;
	PyramidFrustum[ Right ][ C ] = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].x;
	PyramidFrustum[ Right ][ D ] = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].x;
	NormalizePlane( Right );

	PyramidFrustum[ Left ][ A ] = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].x;
	PyramidFrustum[ Left ][ B ] = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].x;
	PyramidFrustum[ Left ][ C ] = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].x;
	PyramidFrustum[ Left ][ D ] = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].x;
	NormalizePlane( Left );

	PyramidFrustum[ Bottom ][ A ] = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].y;
	PyramidFrustum[ Bottom ][ B ] = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].y;
	PyramidFrustum[ Bottom ][ C ] = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].y;
	PyramidFrustum[ Bottom ][ D ] = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].y;
	NormalizePlane( Bottom );

	PyramidFrustum[ Top ][ A ] = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].y;
	PyramidFrustum[ Top ][ B ] = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].y;
	PyramidFrustum[ Top ][ C ] = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].y;
	PyramidFrustum[ Top ][ D ] = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].y;
	NormalizePlane( Top );

	PyramidFrustum[ Back ][ A ] = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].z;
	PyramidFrustum[ Back ][ B ] = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].z;
	PyramidFrustum[ Back ][ C ] = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].z;
	PyramidFrustum[ Back ][ D ] = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].z;
	NormalizePlane( Back );

	PyramidFrustum[ Front ][ A ] = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].z;
	PyramidFrustum[ Front ][ B ] = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].z;
	PyramidFrustum[ Front ][ C ] = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].z;
	PyramidFrustum[ Front ][ D ] = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].z;
	NormalizePlane( Front );
}

//-------------------------------------------------------------------------//

bool le::Frustum::IsVisible( BoundingBox& BoundingBox )
{
	glm::vec3* Vertexs = BoundingBox.GetVertexs();

	for ( int Side = 0; Side < 6; Side++ )
	{
		int IdVertex;

		for ( IdVertex = 0; IdVertex < 8; IdVertex++ )
		{
			if ( PyramidFrustum[ Side ][ A ] * Vertexs[ IdVertex ].x + PyramidFrustum[ Side ][ B ] * Vertexs[ IdVertex ].y +
				 PyramidFrustum[ Side ][ C ] * Vertexs[ IdVertex ].z + PyramidFrustum[ Side ][ D ] > 0 )
				break;
		}

		if ( IdVertex == 8 )
			return false;
	}

	return true;
}

//-------------------------------------------------------------------------//

void le::Frustum::NormalizePlane( FrustumSide FrustumSide )
{
	float Magnitude = NUMBER_TO_FLOAT( sqrt( PyramidFrustum[ FrustumSide ][ A ] * PyramidFrustum[ FrustumSide ][ A ] +
											 PyramidFrustum[ FrustumSide ][ B ] * PyramidFrustum[ FrustumSide ][ B ] +
											 PyramidFrustum[ FrustumSide ][ C ] * PyramidFrustum[ FrustumSide ][ C ] ) );

	PyramidFrustum[ FrustumSide ][ A ] /= Magnitude;
	PyramidFrustum[ FrustumSide ][ B ] /= Magnitude;
	PyramidFrustum[ FrustumSide ][ C ] /= Magnitude;
	PyramidFrustum[ FrustumSide ][ D ] /= Magnitude;
}

//-------------------------------------------------------------------------//
