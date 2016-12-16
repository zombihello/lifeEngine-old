#include  "../../3D/Model.h"
#include <HaffmanCode.h>
//#include <Matrix.h>
//#include "../../Utils/Math/Quaternion.h"

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
/*
Matrixf IMB
{
-0.000000, - 0.026077, - 0.999660, 0.261000,
0.000000, 0.999660, - 0.026077, 1.107341,
1.000000, - 0.000000, - 0.000000, 0.000000,
0.000000, 0.000000, 0.000000, 1.000000
};

Matrixf IMB2
{
-0.000000, - 0.019414, - 0.999812, 34.607894,
- 0.000000, 0.999812, - 0.019414, 0.984820,
1.000000, - 0.000000, - 0.000000, 0.000000,
0.000000, 0.000000, 0.000000, 1.000000
};

Matrixf BM =
{
-0.000000 , -0.000000 , 1.000000 , 0.000000 ,
-0.019414 , 0.999812 , -0.000000 , -0.312757 ,
-0.999812 , -0.019414 , -0.000000 , 34.620491 ,
0.000000 , 0.000000 , 0.000000 , 1.000000
};

Matrixf BM2 =
{
0.999978 , 0.006665 , -0.000000 , 16.224157 ,
-0.006665 , 0.999978 , -0.000000 , -0.000018 ,
0.000000 , 0.000000 , 1.000000 , 0.000000 ,
0.000000 , 0.000000 , 0.000000 , 1.000000
};

Matrixf BM3 =
{
1.000000 , 0.000000 , -0.000000 , 18.115768 ,
0.000000 , 1.000000 , -0.000000 , -0.000003 ,
0.000000 , 0.000000 , 1.000000 , 0.000000 ,
0.000000 , 0.000000 , 0.000000 , 1.000000
};

MatrixMultiply( BM2 , BM , boun2 );
MatrixMultiply( BM3 , boun2 , boun3 );

for ( int i = 0 , count = 1; i < 16; i++ , count++ )
{
cout << boun3[ i ] << " ";
if ( count == 4 )
{
cout << endl;
count = 0;
}
}

Vector3f pos = MatrixToXYZ( boun2 );
cout << endl << pos.x << " " << pos.y << " " << pos.z << endl;

pos = MatrixToXYZ( BM );
cout << endl << pos.x << " " << pos.y << " " << pos.z << endl;


Vector3f _bm1 = MatrixToXYZ( BM );
Vector3f _bm2 = MatrixToXYZ( boun2 );

glBegin( GL_LINES );
glColor3f( 1 , 0 , 0 );
glVertex3f( _bm1.x , _bm1.y , _bm1.z );
glVertex3f( _bm2.x , _bm2.y , _bm2.z );
glColor3f( 0 , 1 ,0 );

_bm1 = MatrixToXYZ( boun2 );
_bm2 = MatrixToXYZ( boun3 );

glVertex3f( _bm1.x , _bm1.y , _bm1.z );
glVertex3f( _bm2.x , _bm2.y , _bm2.z );
glColor3f( 1 , 1 , 1);
glEnd();


Matrixf boun2;
Matrixf boun3;
Matrixf tmp;
*/
//-------------------------------------------------------------------------//

void ReadingMatrix( TiXmlElement* node , le::Bone* bone )
{
		// Работаем с контейнером startMatrix
		TiXmlElement *startMatrix;
		startMatrix = node->FirstChildElement( "startMatrix" );

		if ( startMatrix != NULL ){

			Matrixf tmpBSM;
			string sTmpMatrix = startMatrix->GetText();
			istringstream strStream( sTmpMatrix );

			for ( int i = 0; i < 16 && !strStream.eof(); i++ )
			{
				string sTmp;
				strStream >> sTmp;
				tmpBSM[ i ] = atof( sTmp.c_str() );
			}

			for ( int i = 0; i < 16; i++ )
				bone->StartMatrix[ i ] = tmpBSM[ i ];

			// Работаем с контейнером invertMatrix
			TiXmlElement *invertMatrix;
			invertMatrix = node->FirstChildElement( "invertMatrix" );

			sTmpMatrix = string( invertMatrix->GetText() );
			istringstream _strStream( sTmpMatrix );

			/*for ( int i = 0; i < 16 && !_strStream.eof(); i++ )
			{
			string sTmp;
			_strStream >> sTmp;
			tmpBSM[ i ] = atof( sTmp.c_str() );
			}

			for ( int i = 0; i < 16; i++ )
			bone->InvertMatrix[ i ] = tmpBSM[ i ];*/

			node = node->FirstChildElement( "node" );

			while ( node )
			{
				le::Bone out;
				ReadingMatrix( node , &out );
				bone->vChild.push_back( out );

				node = node->NextSiblingElement();
			}
		}
}

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

	TiXmlDocument LMD;
	LMD.Parse( sModel.c_str() );

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

	// Работаем с контейнером skeleton
	TiXmlElement *skeleton;
	skeleton = model->FirstChildElement( "skeleton" );

	if ( skeleton != NULL )
	{
		// Работаем с контейнером bindShapeMatrix
		TiXmlElement *bsm;
		bsm = skeleton->FirstChildElement( "bindShapeMatrix" );

		Matrixf tmpBSM;
		stringstream strStream( bsm->GetText() );

		for ( int i = 0; i < 16 && !strStream.eof(); i++ )
		{
			string sTmp;
			strStream >> sTmp;
			tmpBSM[ i ] = atof( sTmp.c_str() );
		}

		Skeleton.SetBindShape( tmpBSM );

		// Работаем с контейнером mainNode
		TiXmlElement *mainNode;
		mainNode = skeleton->FirstChildElement( "mainNode" );

		// Работаем с контейнером node
		TiXmlElement *node;
		node = mainNode->FirstChildElement( "node" );

		while ( node )
		{
			Bone tmp;
			ReadingMatrix( node , &tmp );
			Skeleton.AddBone( tmp );
			node = node->NextSiblingElement();
		}
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

	Skeleton.DrawSkeleton();
	RenderWindow->pushGLStates();
}

//-------------------------------------------------------------------------//