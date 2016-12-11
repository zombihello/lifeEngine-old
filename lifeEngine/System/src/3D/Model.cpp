#include  "../../3D/Model.h"
#include <HaffmanCode.h>

//-------------------------------------------------------------------------//

le::Model::Model( sf::RenderWindow& RenderWindow )
{
	this->RenderWindow = &RenderWindow;
}

//-------------------------------------------------------------------------//

le::Model::~Model()
{
	vMainPoligons.clear();
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
	// Временые вектора для работы
	vector<Vector3f>	vTmpPoints;
	vector<Vector3f>	vTmpNormals;
	vector<Vector2f>	vTmpTextureCoords;
	vector<Color>		vTmpVertexColors;
	vector<GLuint>		vTmpTextures;

	// Декодируем модель
	HaffmanCode haffmanCode;
	string sModel = haffmanCode.DecompressedFromFile( route );

	if ( sModel == "ERROR" ) return false;

	TiXmlDocument MDL;
	MDL.Parse( sModel.c_str() );

	// Работаем с контейнером model
	TiXmlElement *model;
	model = MDL.FirstChildElement( "model" );

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
		Vector3f positionPoint;

		positionPoint.x = atof( point->Attribute( "x" ) );
		positionPoint.y = atof( point->Attribute( "y" ) );
		positionPoint.z = atof( point->Attribute( "z" ) );

		vTmpPoints.push_back( positionPoint );

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

		vTmpNormals.push_back( Normal );

		point = point->NextSiblingElement();
	}

	TiXmlElement *UV;
	UV = geometries->FirstChildElement( "UV" );
	point = UV->FirstChildElement( "point" );

	while ( point )
	{
		Vector2f UV;

		UV.x = atof( point->Attribute( "x" ) );
		UV.y = atof( point->Attribute( "y" ) );

		vTmpTextureCoords.push_back( UV );

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

			vTmpVertexColors.push_back( VertexColor );

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
		MainPoligon _mainPoligon;

		while ( p )
		{
			string sTmp = string( p->GetText() );
			istringstream iss( sTmp );
			Poligon poligon;

			for ( int id = 1; !iss.eof(); id++ )
			{
				string _tmp;
				iss >> _tmp;

				switch ( id )
				{
				case 1: // координаты			
					poligon.vPoints.push_back( vTmpPoints[ atoi( _tmp.c_str() ) ] );
					break;

				case 2: // нормали
					poligon.vNormals.push_back( vTmpNormals[ atoi( _tmp.c_str() ) ] );
					break;

				case 3:
					poligon.vTextureCoords.push_back( vTmpTextureCoords[ atoi( _tmp.c_str() ) ] );

					if ( vTmpVertexColors.empty() )
					{
						if ( !vTmpTextures.empty() )
							_mainPoligon.gl_Texture = vTmpTextures[ idTexture ];

						_mainPoligon.vPoligons.push_back( poligon );
						poligon = Poligon();
						id = 0;
					}

					break;

				case 4:
					poligon.vVertexColors.push_back( vTmpVertexColors[ atoi( _tmp.c_str() ) ] );

					if ( !vTmpTextures.empty() )
						_mainPoligon.gl_Texture = vTmpTextures[ idTexture ];

					_mainPoligon.vPoligons.push_back( poligon );
					poligon = Poligon();
					id = 0;
					break;
				}
			}

			p = p->NextSiblingElement();
		}

		vMainPoligons.push_back( _mainPoligon );
		texture = texture->NextSiblingElement();
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

	for ( int j = 0; j < vMainPoligons.size(); j++ )
	{
		glBindTexture( GL_TEXTURE_2D , vMainPoligons[ j ].gl_Texture );
		glBegin( GL_TRIANGLES );

		for ( int i = 0; i < vMainPoligons[ j ].vPoligons.size(); i++ )
			for ( int id = 0; id < vMainPoligons[ j ].vPoligons[ i ].vPoints.size(); id++ )
			{
				glNormal3f( vMainPoligons[ j ].vPoligons[ i ].vNormals[ id ].x , vMainPoligons[ j ].vPoligons[ i ].vNormals[ id ].z , vMainPoligons[ j ].vPoligons[ i ].vNormals[ id ].z );
				glTexCoord2f( vMainPoligons[ j ].vPoligons[ i ].vTextureCoords[ id ].x , vMainPoligons[ j ].vPoligons[ i ].vTextureCoords[ id ].y );
				glVertex3f( vMainPoligons[ j ].vPoligons[ i ].vPoints[ id ].x , vMainPoligons[ j ].vPoligons[ i ].vPoints[ id ].y , vMainPoligons[ j ].vPoligons[ i ].vPoints[ id ].z );
			}

		glEnd();
	}

	RenderWindow->pushGLStates();
}

//-------------------------------------------------------------------------//