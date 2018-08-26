#include "../BSPInfo.h"

//-------------------------------------------------------------------------//

bool le::BSPVertex::operator==( BSPVertex& Vertex )
{
	return
		Position == Vertex.Position &&
		TextureCoord == Vertex.TextureCoord &&
		LightmapCoord == Vertex.LightmapCoord &&
		Normal == Vertex.Normal &&
		Color[ 0 ] == Vertex.Color[ 0 ] &&
		Color[ 1 ] == Vertex.Color[ 1 ] &&
		Color[ 2 ] == Vertex.Color[ 2 ] &&
		Color[ 3 ] == Vertex.Color[ 3 ];
}

//-------------------------------------------------------------------------//

le::BSPVisData::BSPVisData() :
	NumOfClusters( 0 ),
	BytesPerCluster( 0 ),
	Bitsets( NULL )
{}

//-------------------------------------------------------------------------//

le::BSPVisData::~BSPVisData()
{
	if ( Bitsets )
		delete[] Bitsets;
}

//-------------------------------------------------------------------------//

le::BSPEntities::BSPEntities() :
	EntitiesData( NULL )
{}

//-------------------------------------------------------------------------//

le::BSPEntities::~BSPEntities()
{
	if ( EntitiesData )
		delete[] EntitiesData;
}

//-------------------------------------------------------------------------//