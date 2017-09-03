#include "..\Skeleton.h"

//-------------------------------------------------------------------------//

bool le::Skeleton::LoadSkeleton( TiXmlElement* Skeleton )
{
	Logger::Log( Logger::Info, "Loading Skeleton Mesh" );

	// ****************************
	// Загружаем матрицу биндинга
	// ****************************

	TiXmlElement* BindShapeMatrix;
	BindShapeMatrix = Skeleton->FirstChildElement( "bindShapeMatrix" );

	if ( BindShapeMatrix == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"bindShapeMatrix\"" );
		return false;
	}

	stringstream StrStream( BindShapeMatrix->GetText() );
	string TempString;
	glm::vec4 TempVector4;

	for ( int id = 0, axis = 1; id < 4, !StrStream.eof(); axis++ )
	{
		StrStream >> TempString;

		switch ( axis )
		{
		case 1: // x
			TempVector4.x = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
			break;

		case 2: // y
			TempVector4.y = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
			break;

		case 3: // z
			TempVector4.z = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
			break;

		case 4: // w
			TempVector4.w = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
			BindShape[ id ] = TempVector4;

			id++;
			axis = 0;
			break;
		}

		TempString.clear();
	}

	// ****************************
	// Считываем веса костей
	// ****************************

	TiXmlElement* WeightsBones;
	WeightsBones = Skeleton->FirstChildElement( "weightsBones" );

	if ( WeightsBones == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"weightsBones\"" );
		return false;
	}

	vector<float> Weights;

	StrStream.clear();
	StrStream.str( WeightsBones->GetText() );

	while ( !StrStream.eof() )
	{
		StrStream >> TempString;
		Weights.push_back( NUMBER_TO_FLOAT( atof( TempString.c_str() ) ) );
		TempString.clear();
	}

	// ****************************
	// Загружаем имена суставов
	// ****************************

	TiXmlElement* Joints;
	Joints = Skeleton->FirstChildElement( "Joints" );

	if ( Joints == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"Joints\"" );
		return false;
	}

	vector<string> vJoints;

	StrStream.clear();
	StrStream.str( Joints->GetText() );

	while ( !StrStream.eof() )
	{
		StrStream >> TempString;
		vJoints.push_back( TempString );
		TempString.clear();
	}

	// ****************************
	// Загружаем иерархию костей
	// ****************************

	TiXmlElement* MainNode;
	MainNode = Skeleton->FirstChildElement( "mainNode" );

	if ( MainNode == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"mainNode\"" );
		return false;
	}

	TiXmlElement* Node;
	Node = MainNode->FirstChildElement( "node" );

	while ( Node )
	{
		Bone TempBone;
		ReadingBone( Node, &TempBone );
		Bones.push_back( TempBone );

		Node = Node->NextSiblingElement();
	}

	// ****************************
	// Востанавливаем id предков
	// ****************************

	for ( size_t i = 0; i < Bones.size(); i++ )
	{
		Bone* bone = &Bones[ i ];

		if ( bone->ParentName != "" )
			bone->IdParent = GetIdBone( bone->ParentName );
		else
			IdRootBones.push_back( i );
	}

	// ****************************
	// Считывем кол-во костей на одну вершину
	// ****************************

	TiXmlElement* VCount;
	VCount = Skeleton->FirstChildElement( "vcount" );

	if ( VCount == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"vcount\"" );
		return false;
	}

	vector<int> TmpIdVertexs;

	StrStream.clear();
	StrStream.str( VCount->GetText() );

	while ( !StrStream.eof() )
	{
		StrStream >> TempString;
		TmpIdVertexs.push_back( atoi( TempString.c_str() ) );
		TempString.clear();
	}

	// ****************************
	// Считывем id кости и вес
	// ****************************

	TiXmlElement* IdBoneWeihgt;
	IdBoneWeihgt = Skeleton->FirstChildElement( "IdBoneWeihgt" );

	if ( IdBoneWeihgt == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"IdBoneWeihgt\"" );
		return false;
	}

	StrStream.clear();
	StrStream.str( IdBoneWeihgt->GetText() );

	Bone* TmpBone;
	int NumBones = 0;
	int TempNumBones = 0;
	int IdVertex = -1;
	int IdBone = 0;

	for ( int id = 1; !StrStream.eof(); id++ )
	{
		StrStream >> TempString;

		switch ( id )
		{
		case 1: // id кости
			TmpBone = GetBone( vJoints[ atoi( TempString.c_str() ) ] );
			break;

		case 2: // вес
			if ( TempNumBones == 0 )
			{
				IdVertex++;
				NumBones = TmpIdVertexs[ IdVertex ];

				if ( NumBones != 1 )
					TempNumBones = NumBones - 1;

				TmpBone->IdVertex.push_back( IdVertex );
			}
			else
			{
				TmpBone->IdVertex.push_back( IdVertex );
				TempNumBones--;
			}

			TmpBone->Weights.push_back( Weights[ atoi( TempString.c_str() ) ] );

			id = 0;
			break;
		}

		TempString.clear();
	}

	// ****************************
	// Инициализирываем кости скелета
	// ****************************

	for ( size_t i = 0; i < Bones.size(); i++ )
		InitMatrixBone( Bones[ i ] );

	Logger::Log( Logger::Info, "Loaded Skeleton Mesh" );

	return true;
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitSkeleton( const GLuint& VertexBuffer, const vector<MeshVertex>& VBO_Vertexs, const map<int, vector<int>>& Vertexs )
{
	this->VertexBuffer = VertexBuffer;
	this->VBO_Vertexs = VBO_Vertexs;
	this->Vertexs = Vertexs;
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMatrixBone( const string& NameBone, const glm::mat4& Matrix )
{
	Bone* Bone = GetBone( NameBone );

	if ( Bone != NULL )
	{
		Bone->InterpolatedMatrix = Matrix;

		if ( Bone->IdParent != -1 )
			Bone->Realese = Matrix * Bones[ Bone->IdParent ].Realese;

		for ( size_t i = 0; i < Bone->IdChild.size(); i++ )
			InitMatrixBone( Bones[ Bone->IdChild[ i ] ] );
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMesh()
{
	map<int, glm::mat4> BoneTransforms;

	for ( size_t i = 0; i < Bones.size(); i++ )
	{
		Bone* Bone = &Bones[ i ];

		for ( size_t j = 0; j < Bone->IdVertex.size(); j++ )
		{
			int IdVertex = Bone->IdVertex[ j ];
			float weight = Bone->Weights[ j ];

			if ( BoneTransforms.find( IdVertex ) == BoneTransforms.end() )
				BoneTransforms[ IdVertex ] = ( Bone->InvertMatrix * Bone->Realese ) * weight;
			else
				BoneTransforms[ IdVertex ] += ( Bone->InvertMatrix * Bone->Realese ) * weight;
		}
	}

	glm::vec3 newVertex;
	glm::vec3 newNormal;
	vector<MeshVertex> VBO = VBO_Vertexs; // zombiHello: Этот код (и что ниже) временно, буду переписывать скелетку в шейдер
	for ( auto it = Vertexs.begin(); it != Vertexs.end(); it++ )
	{
		MeshVertex* Vertex = &VBO_Vertexs[ it->second[ 0 ] ];
		newVertex = glm::vec4( Vertex->Position.x, Vertex->Position.y, -Vertex->Position.z, 1.0f ) * BoneTransforms[ it->first ];
		newNormal = glm::vec4( Vertex->Normal.x, Vertex->Normal.y, -Vertex->Normal.z, 0.0f ) * BoneTransforms[ it->first ];

		for ( size_t i = 0; i < it->second.size(); i++ )
		{
			VBO[ it->second[ i ] ].Position = newVertex;
			VBO[ it->second[ i ] ].Normal = glm::normalize( newNormal );
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, VBO.size() * sizeof( MeshVertex ), VBO.data(), GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

int le::Skeleton::GetIdBone( const string& NameBone )
{
	for ( size_t i = 0; i < Bones.size(); i++ )
		if ( Bones[ i ].Name == NameBone )
			return NUMBER_TO_INT( i );

	return -1;
}

//-------------------------------------------------------------------------//

le::Skeleton::Bone* le::Skeleton::GetBone( const string& NameBone )
{
	for ( size_t i = 0; i < Bones.size(); i++ )
		if ( Bones[ i ].Name == NameBone )
			return &Bones[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

void le::Skeleton::ReadingBone( TiXmlElement* Node, Bone* Bone )
{
	stringstream StrStream;
	string TempString;
	glm::vec4 TempVector4;

	// ****************************
	// Считываем стартовую матрицу
	// ****************************

	TiXmlElement* StartMatrix;
	const char* TagText = Node->Attribute( "name" );

	if ( TagText != NULL )
		Bone->Name = TagText;

	StartMatrix = Node->FirstChildElement( "startMatrix" );

	if ( StartMatrix == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"startMatrix\"" );
		return;
	}

	TagText = StartMatrix->GetText();

	if ( TagText != NULL )
	{
		StrStream.str( TagText );

		for ( int id = 0, axis = 1; id < 4, !StrStream.eof(); axis++ )
		{
			StrStream >> TempString;

			switch ( axis )
			{
			case 1:
				TempVector4.x = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 2:
				TempVector4.y = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 3:
				TempVector4.z = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 4:
				TempVector4.w = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				Bone->StartMatrix[ id ] = TempVector4;

				id++;
				axis = 0;
				break;
			}

			TempString.clear();
		}
	}

	// ****************************
	// Загружаем инвертированную матрицу
	// ****************************

	TiXmlElement* InvertMatrix;
	InvertMatrix = Node->FirstChildElement( "invertMatrix" );

	if ( InvertMatrix == NULL )
	{
		Logger::Log( Logger::Error, "Not Correct Format In Skeleton Mesh. Not Found Tag \"invertMatrix\"" );
		return;
	}

	TagText = InvertMatrix->GetText();

	if ( TagText != NULL )
	{
		StrStream.clear();
		StrStream.str( TagText );

		for ( int id = 0, axis = 1; id < 4, !StrStream.eof(); axis++ )
		{
			StrStream >> TempString;

			switch ( axis )
			{
			case 1:
				TempVector4.x = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 2:
				TempVector4.y = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 3:
				TempVector4.z = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				break;

			case 4:
				TempVector4.w = NUMBER_TO_FLOAT( atof( TempString.c_str() ) );
				Bone->InvertMatrix[ id ] = TempVector4;

				id++;
				axis = 0;
				break;
			}

			TempString.clear();
		}
	}

	Bone->Realese = Bone->InterpolatedMatrix = Bone->StartMatrix;

	Node = Node->FirstChildElement( "node" );

	while ( Node )
	{
		Skeleton::Bone Parent;
		ReadingBone( Node, &Parent );
		Parent.ParentName = Bone->Name;
		Bone->IdChild.push_back( Bones.size() );
		Bones.push_back( Parent );

		Node = Node->NextSiblingElement();
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitMatrixBone( Bone& Bone )
{
	if ( Bone.ParentName != "" )
		Bone.Realese = Bone.InterpolatedMatrix * Bones[ Bone.IdParent ].Realese;

	for ( size_t i = 0; i < Bone.IdChild.size(); i++ )
		InitMatrixBone( Bones[ Bone.IdChild[ i ] ] );
}

//-------------------------------------------------------------------------//
