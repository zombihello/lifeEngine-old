﻿#include <System\ResourcesManager.h>
#include <System\System.h>
#include "..\Mesh.h"

//-------------------------------------------------------------------------//

bool le::Mesh::LoadMesh( const string & Route )
{
	Logger::Log( Logger::Info, "Loading Mesh [" + Route + "]" );

	TiXmlDocument LMD;

	if ( !LMD.LoadFile( Route.c_str() ) )
	{
		Logger::Log( Logger::Error, "Mesh [" + Route + "] Not Found" );
		return false;
	}

	TiXmlElement* Model;
	Model = LMD.FirstChildElement( "model" );

	if ( Model == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"model\"" );
		return false;
	}

	//====== ЗАГРУЖАЕМ ТЕКСТУРЫ ======//

	TiXmlElement* Textues;
	Textues = Model->FirstChildElement( "textures" );

	if ( Textues )
	{
		TiXmlElement* Img;
		string IdTexture, Route, NameTexture;

		Img = Textues->FirstChildElement( "image" );

		while ( Img )
		{
			IdTexture = Img->Attribute( "id" );
			Route = Img->Attribute( "src" );

			Route.erase( 0, Route.find_last_of( '/' ) + 1 );
			NameTexture = Route;
			NameTexture.erase( 0, NameTexture.find_last_of( '\\' ) + 1 );

			ResourcesManager::LoadGlTexture( NameTexture, Route );
			Mesh::Textures.push_back( ResourcesManager::GetGlTexture( NameTexture ) );

			Img = Img->NextSiblingElement();
		}
	}

	//====== ЗАГРУЖАЕМ ГЕОМЕТРИЮ ======//

	TiXmlElement* Geometries;
	Geometries = Model->FirstChildElement( "geometries" );

	if ( Geometries == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"geometries\"" );
		return false;
	}

	// Загружаем позицию вершин

	TiXmlElement* PositionPoint;
	PositionPoint = Geometries->FirstChildElement( "position_point" );

	if ( PositionPoint == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"position_point\"" );
		return false;
	}

	vector<glm::vec3> VertexPosition;
	glm::vec3 TempVector3;
	glm::vec2 TempVector2;

	TiXmlElement* Point;
	Point = PositionPoint->FirstChildElement( "point" );

	while ( Point )
	{
		TempVector3.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "x" ) ) );
		TempVector3.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "y" ) ) );
		TempVector3.z = NUMBER_TO_FLOAT( atof( Point->Attribute( "z" ) ) );

		VertexPosition.push_back( TempVector3 );
		Point = Point->NextSiblingElement();
	}

	// Загружаем нормали

	TiXmlElement* Normals;
	Normals = Geometries->FirstChildElement( "normals" );

	if ( Normals == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"normals\"" );
		return false;
	}

	vector<glm::vec3> VertexNormal;
	Point = Normals->FirstChildElement( "point" );

	while ( Point )
	{
		TempVector3.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "x" ) ) );
		TempVector3.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "y" ) ) );
		TempVector3.z = NUMBER_TO_FLOAT( atof( Point->Attribute( "z" ) ) );

		VertexNormal.push_back( TempVector3 );
		Point = Point->NextSiblingElement();
	}

	// Загружаем текстурные координаты

	TiXmlElement* UV;
	UV = Geometries->FirstChildElement( "UV" );

	if ( UV == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"UV\"" );
		return false;
	}

	vector<glm::vec2> TextureCoords;
	Point = UV->FirstChildElement( "point" );

	while ( Point )
	{
		TempVector2.x = NUMBER_TO_FLOAT( atof( Point->Attribute( "x" ) ) );
		TempVector2.y = NUMBER_TO_FLOAT( atof( Point->Attribute( "y" ) ) );

		TextureCoords.push_back( TempVector2 );
		Point = Point->NextSiblingElement();
	}

	// Считываем id вершин, нормалей и uv, и записуем 
	// в окончательный массив

	TiXmlElement* Polygons;
	Polygons = Geometries->FirstChildElement( "polygons" );

	if ( Polygons == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Mesh [" + Route + "]. Not Found Tag \"polygons\"" );
		return false;
	}

	TiXmlElement* Material;
	TiXmlElement* p;
	string TmpString;
	istringstream StrStream;
	int IdTexture;
	Material = Polygons->FirstChildElement( "material" );

	while ( Material )
	{
		int IdVertex = 0;
		MeshVertex MeshVertex;
		vector<unsigned int> TmpIdVertexs;

		p = Material->FirstChildElement( "p" );
		IdTexture = atoi( Material->Attribute( "id" ) );

		while ( p )
		{
			StrStream.clear();
			StrStream.str( p->GetText() );

			for ( int id = 1; !StrStream.eof(); id++ )
			{
				StrStream >> TmpString;

				switch ( id )
				{
				case 1: // Позиция
					IdVertex = atoi( TmpString.c_str() );
					MeshVertex.Position = VertexPosition[ IdVertex ];
					break;

				case 2: // Нормаль
					MeshVertex.Normal = VertexNormal[ atoi( TmpString.c_str() ) ];
					break;

				case 3: // Текстурная координата
					MeshVertex.TextureCoord = TextureCoords[ atoi( TmpString.c_str() ) ];

					bool IsFind = false;

					for ( size_t i = 0; i < VBO_Vertexs.size(); i++ )
						if ( MeshVertex == VBO_Vertexs[ i ] )
						{
							TmpIdVertexs.push_back( i );
							IsFind = true;
							break;
						}

					if ( !IsFind )
					{
						size_t VBOsize = VBO_Vertexs.size();

						Vertexs[ IdVertex ].push_back( VBOsize );
						TmpIdVertexs.push_back( VBOsize );
						VBO_Vertexs.push_back( MeshVertex );
					}

					id = 0;
					break;
				}

				TmpString.clear();
			}

			this->IdVertexs[ Mesh::Textures[ IdTexture ] ] = TmpIdVertexs;
			p = p->NextSiblingElement();
		}

		Material = Material->NextSiblingElement();
	}

	// TODO: Написать загрузку скелета и анимаций

	Logger::Log( Logger::Info, "Mesh [" + Route + "] Loaded" );
	return true;
}

//-------------------------------------------------------------------------//

void le::Mesh::ClearMesh()
{
	VBO_Vertexs.clear();
	Mesh::Vertexs.clear();
	IdVertexs.clear();
	Textures.clear();
}

//-------------------------------------------------------------------------//

const vector<le::Mesh::MeshVertex>& le::Mesh::GetVBO_Vertexs()
{
	return VBO_Vertexs;
}

//-------------------------------------------------------------------------//

const vector<GLuint>& le::Mesh::GetTextures()
{
	return Textures;
}

//-------------------------------------------------------------------------//

const map<int, vector<int>>& le::Mesh::GetVertexs()
{
	return Vertexs;
}

//-------------------------------------------------------------------------//

const map<GLuint, vector<unsigned int>>& le::Mesh::GetIdVertexs()
{
	return IdVertexs;
}

//-------------------------------------------------------------------------//

bool le::Mesh::MeshVertex::operator==( MeshVertex& MeshVertex )
{
	return Position == MeshVertex.Position &&
		Normal == MeshVertex.Normal &&
		TextureCoord == MeshVertex.TextureCoord;
}

//-------------------------------------------------------------------------//