#include  "../../3D/Model.h"
#include <HaffmanCode.h>

//-------------------------------------------------------------------------//

le::Model::Model( sf::RenderWindow& RenderWindow )
{
	this->RenderWindow = &RenderWindow;
}

//-------------------------------------------------------------------------//

GLuint le::Model::LoadTexture( string route )
{
	Image image;
	image.loadFromFile( route );
	image.flipVertically();

	GLuint texture = 0;
	glGenTextures( 1 , &texture );
	glBindTexture( GL_TEXTURE_2D , texture );
	gluBuild2DMipmaps( GL_TEXTURE_2D , GL_RGBA , image.getSize().x , image.getSize().y , GL_RGBA , GL_UNSIGNED_BYTE , image.getPixelsPtr() );

	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );

	return texture;
}

//-------------------------------------------------------------------------//

bool le::Model::LoadModel( string route )
{
	vector<GLuint>		vTmpTextures;
	TiXmlDocument LMD;

#ifdef DEBUG
	string routeTmp = route;
	routeTmp.erase( routeTmp.find_last_of( '.' ) + 1 , routeTmp.size() );
	routeTmp += "xlmd";

	if ( !LMD.LoadFile( routeTmp.c_str() ) )
	{
		cout << "Error: Model [" << routeTmp << "] Not Found\n";
		return false;
	}
#else
	string routeTmp = route;
	routeTmp.erase( 0 , routeTmp.find_last_of( '.' ) + 1 );

	if ( routeTmp == "lmd" )
	{
		// Декодируем модель
		HaffmanCode haffmanCode;
		string sModel = haffmanCode.DecompressedFromFile( route );

		if ( sModel == "ERROR" )
		{
			cout << "Error: Model [" << route << "] Not Found\n";
			return false;
		}

		LMD.Parse( sModel.c_str() );
	}
	else if ( routeTmp == "xlmd" )
	{
		if ( !LMD.LoadFile( route.c_str() ) )
		{
			cout << "Error: Model [" << route << "] Not Found\n";
			return false;
		}
	}
	else
	{
		cout << "Error: Model [" << route << "] Not Correct Format\n";
		return false;
	}
#endif

	// Работаем с контейнером model
	TiXmlElement *model;
	model = LMD.FirstChildElement( "model" );

	// Работаем с контейнером textures
	TiXmlElement *textures;
	TiXmlElement *img;

	textures = model->FirstChildElement( "textures" );

	if ( textures != NULL )
	{
		img = textures->FirstChildElement( "image" );

		while ( img )
		{
			string route = img->Attribute( "src" );
			route.erase( 0 , route.find_last_of( '/' ) + 1 );
			vTmpTextures.push_back( LoadTexture( route ) );

			img = img->NextSiblingElement();
		}
	}

	// Работаем с контейнером geometries
	TiXmlElement *geometries;
	TiXmlElement *point;

	geometries = model->FirstChildElement( "geometries" );

	TiXmlElement *position_point;
	position_point = geometries->FirstChildElement( "position_point" );
	point = position_point->FirstChildElement( "point" );

	while ( point )
	{
		Vertex Vertex;

		Vertex.Position.x = atof( point->Attribute( "x" ) );
		Vertex.Position.y = atof( point->Attribute( "y" ) );
		Vertex.Position.z = atof( point->Attribute( "z" ) );

		Mesh.vVertexs.push_back( Vertex );
		point = point->NextSiblingElement();
	}

	TiXmlElement *Normals;
	Normals = geometries->FirstChildElement( "normals" );
	point = Normals->FirstChildElement( "point" );

	while ( point )
	{
		Vector3f Normal;

		Normal.x = atof( point->Attribute( "x" ) );
		Normal.y = atof( point->Attribute( "y" ) );
		Normal.z = atof( point->Attribute( "z" ) );

		Mesh.vNormals.push_back( Normal );
		point = point->NextSiblingElement();
	}

	TiXmlElement *UV;
	UV = geometries->FirstChildElement( "UV" );
	point = UV->FirstChildElement( "point" );

	while ( point )
	{
		TextureCoord TexCoord;

		TexCoord.Coords.x = atof( point->Attribute( "x" ) );
		TexCoord.Coords.y = atof( point->Attribute( "y" ) );

		Mesh.vTextureCoords.push_back( TexCoord );
		point = point->NextSiblingElement();

	}

	TiXmlElement *VertexColors;
	VertexColors = geometries->FirstChildElement( "VertexColors" );

	if ( VertexColors != NULL )
	{
		point = VertexColors->FirstChildElement( "point" );

		while ( point )
		{
			Color VertexColor;

			VertexColor.r = atof( point->Attribute( "r" ) );
			VertexColor.g = atof( point->Attribute( "g" ) );
			VertexColor.b = atof( point->Attribute( "b" ) );
			VertexColor.a = atof( point->Attribute( "a" ) );

			Mesh.vVertexColors.push_back( VertexColor );
			point = point->NextSiblingElement();
		}
	}

	TiXmlElement *polygons;
	TiXmlElement *texture;
	TiXmlElement *p;

	polygons = geometries->FirstChildElement( "polygons" );
	texture = polygons->FirstChildElement( "texture" );

	while ( texture )
	{
		p = texture->FirstChildElement( "p" );

		int idTexture = atoi( texture->Attribute( "id" ) );
		IDsVNTC IDs;

		while ( p )
		{
			TextureCoord* TexCoord;
			string sTmp = string( p->GetText() );
			istringstream iss( sTmp );

			for ( int id = 1; !iss.eof(); id++ )
			{
				string _tmp;
				iss >> _tmp;

				switch ( id )
				{
					case 1: // координаты	
						IDs.idVertex = atoi( _tmp.c_str() );
						break;

					case 2: // нормали
						IDs.idNormal = atoi( _tmp.c_str() );
						break;

					case 3:
						IDs.idTextureCoord = atoi( _tmp.c_str() );

						TexCoord = &Mesh.vTextureCoords[ atoi( _tmp.c_str() ) ];
						if ( TexCoord->TextureEmpty )
						{
							TexCoord->gl_Texture = vTmpTextures[ idTexture ];
							TexCoord->TextureEmpty = false;
						}

						if ( Mesh.vVertexColors.empty() )
						{
							Mesh.vIDs.push_back( IDs );
							id = 0;
							IDs = IDsVNTC();
						}

						break;

					case 4:
						IDs.idVertexColor = atoi( _tmp.c_str() );
						Mesh.vIDs.push_back( IDs );
						id = 0;
						IDs = IDsVNTC();
						break;
				}
			}

			p = p->NextSiblingElement();
		}

		texture = texture->NextSiblingElement();
	}

	// Работаем с контейнером skeleton
	TiXmlElement *skeleton;
	skeleton = model->FirstChildElement( "skeleton" );

	if ( skeleton != NULL )
	{
		Skeleton.LoadSkeleton( skeleton , Mesh );
		Skeleton.InitMesh( Mesh );
	}

	return true;
}

//-------------------------------------------------------------------------//

void le::Model::RenderModel()
{
	RenderWindow->popGLStates();

	glClear( GL_DEPTH_BUFFER_BIT );

	glRotatef( 0.5 , 10.f , 0.f , 0.f );
	glRotatef( 0.5 , 0.f , 10.f , 0.f );
	glRotatef( 0.5 , 0.f , 0.f , 10.f );

	Skeleton.DrawSkeleton( Skeleton.GetAllBones() );

	if ( !Keyboard::isKeyPressed( Keyboard::Q ) )
	{
		GLuint lastTexture = Mesh.vTextureCoords[ Mesh.vIDs[ 0 ].idTextureCoord ].gl_Texture;
		glBindTexture( GL_TEXTURE_2D , lastTexture );
		glBegin( GL_TRIANGLES );

		for ( int i = 0; i < Mesh.vIDs.size(); i++ )
		{
			IDsVNTC IDs = Mesh.vIDs[ i ];
			TextureCoord TexCoord = Mesh.vTextureCoords[ IDs.idTextureCoord ];
			Vector3f Vertex = Mesh.vVertexs[ IDs.idVertex ].Position;
			Vector3f Normal = Mesh.vNormals[ IDs.idNormal ];

			if ( TexCoord.gl_Texture != lastTexture )
			{
				lastTexture = TexCoord.gl_Texture;

				glEnd();
				glBindTexture( GL_TEXTURE_2D , TexCoord.gl_Texture );
				glBegin( GL_TRIANGLES );
			}

			glNormal3f( Normal.x , Normal.y , Normal.z );
			glTexCoord2f( TexCoord.Coords.x , TexCoord.Coords.y );
			glVertex3f( Vertex.x , Vertex.y , Vertex.z );
		}

		glEnd();
	}

	RenderWindow->pushGLStates();
}

//-------------------------------------------------------------------------//

le::TextureCoord::TextureCoord()
{
	TextureEmpty = true;
}

//-------------------------------------------------------------------------//

le::IDsVNTC::IDsVNTC()
{
	idVertex = idNormal = idTextureCoord = idVertexColor = 0;
}

//-------------------------------------------------------------------------//