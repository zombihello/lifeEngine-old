#include "..\Frustum.h"
#include <Graphics\BoundingBox.h>
#include <System\System.h>

//-------------------------------------------------------------------------//

void le::Frustum::UpdateFrustum( const glm::mat4& Projection, const glm::mat4& View )
{
	glm::mat4 ClippingPlane = Projection * View;
	glm::vec4 TempVector;

	TempVector.x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].x;
	TempVector.y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].x;
	TempVector.z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].x;
	TempVector.w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].x;
	PyramidFrustum[ Right ] = glm::normalize( TempVector );

	TempVector.x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].x;
	TempVector.y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].x;
	TempVector.z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].x;
	TempVector.w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].x;
	PyramidFrustum[ Left ] = glm::normalize( TempVector );

	TempVector.x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].y;
	TempVector.y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].y;
	TempVector.z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].y;
	TempVector.w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].y;
	PyramidFrustum[ Bottom ] = glm::normalize( TempVector );

	TempVector.x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].y;
	TempVector.y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].y;
	TempVector.z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].y;
	TempVector.w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].y;
	PyramidFrustum[ Top ] = glm::normalize( TempVector );

	TempVector.x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].z;
	TempVector.y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].z;
	TempVector.z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].z;
	TempVector.w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].z;
	PyramidFrustum[ Back ] = glm::normalize( TempVector );

	TempVector.x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].z;
	TempVector.y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].z;
	TempVector.z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].z;
	TempVector.w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].z;
	PyramidFrustum[ Front ] = glm::normalize( TempVector );
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
			if ( PyramidFrustum[ Side ].x * Vertexs[ IdVertex ].x + PyramidFrustum[ Side ].y * Vertexs[ IdVertex ].y +
				 PyramidFrustum[ Side ].z * Vertexs[ IdVertex ].z + PyramidFrustum[ Side ].w > 0 )
				break;
		}

		if ( IdVertex == 8 )
			return false;
	}

	return true;
}

//-------------------------------------------------------------------------//
