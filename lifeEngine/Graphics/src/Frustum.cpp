#include <System\System.h>
#include <Graphics\Bounding Volume\BoundingBox.h>
#include <Graphics\Bounding Volume\BoundingSphere.h>
#include <Graphics\Frustum.h>

//-------------------------------------------------------------------------//

void le::Frustum::UpdateFrustum( const glm::mat4& Projection, const glm::mat4& View )
{
	glm::mat4 ClippingPlane = Projection * View;

	PyramidFrustum[ Right ].x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].x;
	PyramidFrustum[ Right ].y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].x;
	PyramidFrustum[ Right ].z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].x;
	PyramidFrustum[ Right ].w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].x;

	PyramidFrustum[ Left ].x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].x;
	PyramidFrustum[ Left ].y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].x;
	PyramidFrustum[ Left ].z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].x;
	PyramidFrustum[ Left ].w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].x;

	PyramidFrustum[ Bottom ].x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].y;
	PyramidFrustum[ Bottom ].y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].y;
	PyramidFrustum[ Bottom ].z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].y;
	PyramidFrustum[ Bottom ].w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].y;

	PyramidFrustum[ Top ].x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].y;
	PyramidFrustum[ Top ].y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].y;
	PyramidFrustum[ Top ].z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].y;
	PyramidFrustum[ Top ].w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].y;

	PyramidFrustum[ Back ].x = ClippingPlane[ 0 ].w + ClippingPlane[ 0 ].z;
	PyramidFrustum[ Back ].y = ClippingPlane[ 1 ].w + ClippingPlane[ 1 ].z;
	PyramidFrustum[ Back ].z = ClippingPlane[ 2 ].w + ClippingPlane[ 2 ].z;
	PyramidFrustum[ Back ].w = ClippingPlane[ 3 ].w + ClippingPlane[ 3 ].z;

	PyramidFrustum[ Front ].x = ClippingPlane[ 0 ].w - ClippingPlane[ 0 ].z;
	PyramidFrustum[ Front ].y = ClippingPlane[ 1 ].w - ClippingPlane[ 1 ].z;
	PyramidFrustum[ Front ].z = ClippingPlane[ 2 ].w - ClippingPlane[ 2 ].z;
	PyramidFrustum[ Front ].w = ClippingPlane[ 3 ].w - ClippingPlane[ 3 ].z;

	NormalizePlanes();
}

//-------------------------------------------------------------------------//

bool le::Frustum::IsVisible( BoundingBox& BoundingBox )
{
	glm::vec3* Vertexs = BoundingBox.GetVertexs();
	glm::vec3* Position = &BoundingBox.GetPosition();

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

bool le::Frustum::IsVisible( glm::vec3& MinPosition, glm::vec3& MaxPosition )
{
	for ( int i = 0; i < 6; i++ )
	{
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;

		return false;
	}

	return true;
}

//-------------------------------------------------------------------------//

bool le::Frustum::IsVisible( glm::ivec3& MinPosition, glm::ivec3& MaxPosition )
{
	for ( int i = 0; i < 6; i++ )
	{
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MinPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MinPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MinPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;
		if ( PyramidFrustum[ i ].x * MaxPosition.x + PyramidFrustum[ i ].y * MaxPosition.y + PyramidFrustum[ i ].z * MaxPosition.z + PyramidFrustum[ i ].w > 0 )  continue;

		return false;
	}

	return true;
}

//-------------------------------------------------------------------------//

bool le::Frustum::IsVisible( BoundingSphere& BoundingSphere )
{
	glm::vec3* Position = &BoundingSphere.GetPosition();
	float Radius = BoundingSphere.GetRadius();

	for ( int Side = 0; Side < 6; Side++ )
		if ( PyramidFrustum[ Side ].x * Position->x + PyramidFrustum[ Side ].y * Position->y +
			 PyramidFrustum[ Side ].z * Position->z + PyramidFrustum[ Side ].w <= -Radius )
			return false;

	return true;
}

//-------------------------------------------------------------------------//

bool le::Frustum::IsVisible( const glm::vec3 & Position, float Radius )
{
	for ( int Side = 0; Side < 6; Side++ )
		if ( PyramidFrustum[ Side ].x * Position.x + PyramidFrustum[ Side ].y * Position.y +
			 PyramidFrustum[ Side ].z * Position.z + PyramidFrustum[ Side ].w <= -Radius )
			return false;

	return true;
}

//-------------------------------------------------------------------------//

void le::Frustum::NormalizePlanes()
{
	for ( int Side = 0; Side < 6; Side++ )
	{
		float Magnitude = sqrt( PyramidFrustum[ Side ].x * PyramidFrustum[ Side ].x +
								PyramidFrustum[ Side ].y * PyramidFrustum[ Side ].y +
								PyramidFrustum[ Side ].z * PyramidFrustum[ Side ].z );

		PyramidFrustum[ Side ].x /= Magnitude;
		PyramidFrustum[ Side ].y /= Magnitude;
		PyramidFrustum[ Side ].z /= Magnitude;
		PyramidFrustum[ Side ].w /= Magnitude;
	}
}

//-------------------------------------------------------------------------//
