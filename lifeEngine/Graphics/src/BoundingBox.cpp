#include "..\BoundingBox.h"

//-------------------------------------------------------------------------//

void le::BoundingBox::InitBox( const glm::vec3& MinVertex, const glm::vec3& MaxVertex )
{
	Vertexs[ 0 ] = glm::vec3( MinVertex.x, MinVertex.z, 0 );
	Vertexs[ 1 ] = glm::vec3( MaxVertex.x, MinVertex.z, 0 );
	Vertexs[ 2 ] = glm::vec3( MinVertex.x, MaxVertex.z, 0 );
	Vertexs[ 3 ] = glm::vec3( MaxVertex.x, MaxVertex.z, 0 );
	Vertexs[ 4 ] = glm::vec3( MinVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 5 ] = glm::vec3( MaxVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 6 ] = glm::vec3( MinVertex.x, MaxVertex.z, MaxVertex.y );
	Vertexs[ 7 ] = glm::vec3( MaxVertex.x, MaxVertex.z, MaxVertex.y );
}

//-------------------------------------------------------------------------//

void le::BoundingBox::InitBox( const vector<glm::vec3>& Vertexs )
{
	for ( int i = 0; i < 8; i++ )
		this->Vertexs[ i ] = Vertexs[ i ];
}

//-------------------------------------------------------------------------//

glm::vec3* le::BoundingBox::GetVertexs()
{
	return &Vertexs[0];
}

//-------------------------------------------------------------------------//
