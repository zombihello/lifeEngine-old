#include "..\..\3D\Skeleton.h"
#include "..\..\3D\Model.h"

//-------------------------------------------------------------------------//

le::Skeleton::Skeleton()
{
	IsLoad = false;

	mModelVertexs = NULL;
	vVBO_Vertexs = NULL;
	VertexBuffer = NULL;
}

//-------------------------------------------------------------------------//

le::Skeleton::~Skeleton()
{
	ClearSkeleton();
}

//-------------------------------------------------------------------------//

void le::Skeleton::ClearSkeleton()
{
	IsLoad = false;

	mModelVertexs = NULL;
	vVBO_Vertexs = NULL;
	VertexBuffer = NULL;

	vBones.clear();
	vUpdateBones.clear();
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitMesh( map<int, ModelVertex>& ModelVertexs, vector<VBO_ModelVertex>& VBO_Vertexs )
{
	for ( auto it = ModelVertexs.begin(); it != ModelVertexs.end(); it++ )
	{
		Matrix matrixBone;
		Matrix matrixVertex;
		le::ModelVertex& Vertex = it->second;
		string nameBone;

		for ( int j = 0; j < Vertex.vIdBones.size(); j++ )
		{
			le::Bone* Bone = &vBones[Vertex.vIdBones[j]];
			nameBone = Bone->sNameBone;

			float weight = Vertex.vWeights[j];
			Matrixf matrixBoneTmp;

			if ( j == 0 )
			{
				Matrix::MatrixMultiply( Bone->InvertMatrix, Bone->Realese, matrixBone.matrix );
				Matrix::MatrixMultiply( weight, matrixBone.matrix, matrixBone.matrix );
			}
			else
			{
				Matrix::MatrixMultiply( Bone->InvertMatrix, Bone->Realese, matrixBoneTmp );
				Matrix::MatrixMultiply( weight, matrixBoneTmp, matrixBoneTmp );
				Matrix::MatrixAddition( matrixBone.matrix, matrixBoneTmp, matrixBone.matrix );
			}
		}

		Matrix::MatrixMove( Vertex.Position, matrixVertex.matrix );
		Matrix::MatrixMultiply( matrixVertex.matrix, matrixBone.matrix, matrixVertex.matrix );

		Vector3f posVer = Matrix::MatrixToXYZ( matrixVertex.matrix );
		//Vertex.Position = posVer;

		for ( int i = 0; i < Vertex.vIdVBO_Vertex.size(); i++ )
		{
			VBO_ModelVertex* VBO_ModelVertex = &VBO_Vertexs[Vertex.vIdVBO_Vertex[i]];
			VBO_ModelVertex->Position = posVer;
		}
	}

	//vUpdateBone.clear();
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMesh()
{
	for ( auto it = mModelVertexs->begin(); it != mModelVertexs->end(); it++ )
	{
		Matrix matrixBone;
		Matrix matrixVertex;
		le::ModelVertex& Vertex = it->second;
		string nameBone;

		for ( int j = 0; j < Vertex.vIdBones.size(); j++ )
		{
			le::Bone* Bone = &vBones[Vertex.vIdBones[j]];
			nameBone = Bone->sNameBone;

			float weight = Vertex.vWeights[j];
			Matrixf matrixBoneTmp;

			if ( j == 0 )
			{
				Matrix::MatrixMultiply( Bone->InvertMatrix, Bone->Realese, matrixBone.matrix );
				Matrix::MatrixMultiply( weight, matrixBone.matrix, matrixBone.matrix );
			}
			else
			{
				Matrix::MatrixMultiply( Bone->InvertMatrix, Bone->Realese, matrixBoneTmp );
				Matrix::MatrixMultiply( weight, matrixBoneTmp, matrixBoneTmp );
				Matrix::MatrixAddition( matrixBone.matrix, matrixBoneTmp, matrixBone.matrix );
			}
		}

		Matrix::MatrixMove( Vertex.Position, matrixVertex.matrix );
		Matrix::MatrixMultiply( matrixVertex.matrix, matrixBone.matrix, matrixVertex.matrix );

		Vector3f posVer = Matrix::MatrixToXYZ( matrixVertex.matrix );
		//Vertex.Position = posVer;

		for ( int i = 0; i < Vertex.vIdVBO_Vertex.size(); i++ )
		{
			VBO_ModelVertex* VBO_ModelVertex = &( *vVBO_Vertexs )[Vertex.vIdVBO_Vertex[i]];
			VBO_ModelVertex->Position = posVer;
		}
	}

	glGenBuffers( 1, VertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, *VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vVBO_Vertexs->size() * sizeof( VBO_ModelVertex ), vVBO_Vertexs->data(), GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	//vUpdateBone.clear();
}

//-------------------------------------------------------------------------//

void le::Skeleton::ReadingBone( TiXmlElement* node, Bone* bone )
{
	// Работаем с контейнером startMatrix
	TiXmlElement *startMatrix;
	startMatrix = node->FirstChildElement( "startMatrix" );

	string sTmpMatrix;

	if ( startMatrix->GetText() != NULL )
	{
		sTmpMatrix = startMatrix->GetText();
		istringstream strStream( sTmpMatrix );

		for ( int i = 0; i < 16 && !strStream.eof(); i++ )
		{
			string sTmp;
			strStream >> sTmp;
			bone->StartMatrix[i] = atof( sTmp.c_str() );
		}
	}

	// Работаем с контейнером invertMatrix
	TiXmlElement *invertMatrix;
	invertMatrix = node->FirstChildElement( "invertMatrix" );

	if ( invertMatrix->GetText() != NULL )
	{
		sTmpMatrix = string( invertMatrix->GetText() );
		istringstream _strStream( sTmpMatrix );

		for ( int i = 0; i < 16 && !_strStream.eof(); i++ )
		{
			string sTmp;
			_strStream >> sTmp;
			bone->InvertMatrix[i] = atof( sTmp.c_str() );
		}
	}
	if ( node->Attribute( "name" ) != NULL )
		bone->sNameBone = node->Attribute( "name" );

	for ( int i = 0; i < 16; i++ )
		bone->Realese[i] = bone->StartMatrix[i];

	node = node->FirstChildElement( "node" );

	while ( node )
	{
		Bone out;
		ReadingBone( node, &out );
		bone->vIdChild.push_back( vBones.size() );
		out.iIdPerent = vBones.size() + 1;
		out.sNameParentBone = bone->sNameBone;
		AddBone( out );

		node = node->NextSiblingElement();
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::LoadSkeleton( TiXmlElement* skeleton, map<int, ModelVertex>& mVertexs )
{
	vector<int>			vTmpIdVertexs;
	vector<float>		vWeights;

	// Работаем с контейнером bindShapeMatrix
	TiXmlElement *bsm;
	bsm = skeleton->FirstChildElement( "bindShapeMatrix" );

	Matrixf tmpBSM;
	stringstream strStream( bsm->GetText() );

	for ( int i = 0; i < 16 && !strStream.eof(); i++ )
	{
		string sTmp;
		strStream >> sTmp;
		tmpBSM[i] = atof( sTmp.c_str() );
	}

	SetBindShape( tmpBSM );

	// Работаем с контейнером weightsBones
	TiXmlElement *weightsBones;
	weightsBones = skeleton->FirstChildElement( "weightsBones" );

	stringstream strWeights( weightsBones->GetText() );

	for ( int id = 0; !strWeights.eof(); id++ )
	{
		string tmp;
		strWeights >> tmp;

		vWeights.push_back( atof( tmp.c_str() ) );
	}

	// Работаем с контейнером Joints
	TiXmlElement *Joints;
	Joints = skeleton->FirstChildElement( "Joints" );

	stringstream strJoints( Joints->GetText() );
	vector<string> vJoints;

	for ( int i = 0; !strJoints.eof(); i++ )
	{
		string tmp;
		strJoints >> tmp;
		vJoints.push_back( tmp );
	}
	// Работаем с контейнером mainNode
	TiXmlElement *mainNode;
	mainNode = skeleton->FirstChildElement( "mainNode" );

	// Работаем с контейнером node
	TiXmlElement *node;
	node = mainNode->FirstChildElement( "node" );

	while ( node )
	{
		Bone tmp;
		ReadingBone( node, &tmp );
		AddBone( tmp );

		node = node->NextSiblingElement();
	}

	for ( int i = 0; i < vBones.size(); i++ )
	{
		Bone* bone = &vBones[i];
		if ( bone->sNameParentBone != "" )
			bone->iIdPerent = GetIdBone( bone->sNameParentBone );
	}

	// Работаем с контейнером vcount
	TiXmlElement *vcount;
	vcount = skeleton->FirstChildElement( "vcount" );

	stringstream strVertexIDs( vcount->GetText() );

	for ( int id = 1; !strVertexIDs.eof(); id++ )
	{
		string tmp;
		strVertexIDs >> tmp;

		vTmpIdVertexs.push_back( atoi( tmp.c_str() ) );
	}

	// Работаем с контейнером IdBoneWeihgt
	TiXmlElement *IdBoneWeihgt;
	IdBoneWeihgt = skeleton->FirstChildElement( "IdBoneWeihgt" );

	stringstream strIDs( IdBoneWeihgt->GetText() );

	int numBones = 1;
	int tmpNumBones = 0;
	int idVertex = -1;
	int idBone = 0;

	for ( int id = 1; !strIDs.eof(); id++ )
	{
		string tmp;
		strIDs >> tmp;

		switch ( id )
		{
		case 1:
			idBone = GetIdBone( vJoints[atoi( tmp.c_str() )] );
			break;

		case 2:
			if ( tmpNumBones == 0 )
			{
				idVertex++;
				numBones = vTmpIdVertexs[idVertex];

				if ( numBones != 1 )
					tmpNumBones = numBones - 1;

				mVertexs[idVertex].vIdBones.push_back( idBone );
			}
			else
			{
				mVertexs[idVertex].vIdBones.push_back( idBone );
				tmpNumBones--;
			}

			mVertexs[idVertex].vWeights.push_back( vWeights[atoi( tmp.c_str() )] );

			id = 0;
			break;
		}
	}

	InitBone();
}

//-------------------------------------------------------------------------//

void le::Skeleton::SetMatrixBone( string nameBone, Matrixf matrix )
{
	Bone* bone = GetBone( nameBone );
	//vUpdateBone.push_back( bone );

	if ( bone != NULL )
	if ( bone->iIdPerent != -1 )
	{
		Matrix::MatrixMultiply( matrix, vBones[bone->iIdPerent].Realese, bone->Realese );

		for ( int i = 0; i < bone->vIdChild.size(); i++ )
		{
			Bone* boneChild = &vBones[bone->vIdChild[i]];
			//vUpdateBone.push_back( boneChild );
			InitMatrixBone( *boneChild );
		}

	}
	else
	{
		for ( int i = 0; i < 16; i++ )
			bone->Realese[i] = matrix[i];
	}

}

//-------------------------------------------------------------------------//

void le::Skeleton::InitSkeleton( GLuint& VertexBuffer, vector<VBO_ModelVertex>& VBO_Vertexs, map<int, ModelVertex>& ModelVertexs )
{
	IsLoad = true;

	mModelVertexs = &ModelVertexs;
	vVBO_Vertexs = &VBO_Vertexs;
	this->VertexBuffer = &VertexBuffer;
}

//-------------------------------------------------------------------------//

void le::Skeleton::AddBone( Bone bone )
{
	vBones.push_back( bone );
}

//-------------------------------------------------------------------------//

void le::Skeleton::SetBindShape( Matrixf bindShape )
{
	for ( int i = 0; i < 16; i++ )
		BindShape[i] = bindShape[i];
}

//-------------------------------------------------------------------------//

float* le::Skeleton::GetBindShape()
{
	return BindShape;
}

//-------------------------------------------------------------------------//

vector<le::Bone> le::Skeleton::GetAllBones()
{
	return vBones;
}

//-------------------------------------------------------------------------//

le::Bone* le::Skeleton::GetBone( string NameBone )
{
	for ( int i = 0; i < vBones.size(); i++ )
	if ( vBones[i].sNameBone == NameBone )
		return &vBones[i];

	return NULL;
}

//-------------------------------------------------------------------------//

le::Bone* le::Skeleton::GetBone( int idBone )
{
	if ( idBone < vBones.size() )
		return &vBones[idBone];

	return NULL;
}

//-------------------------------------------------------------------------//

int	 le::Skeleton::GetIdBone( string NameBone )
{
	for ( int i = 0; i < vBones.size(); i++ )
	if ( vBones[i].sNameBone == NameBone )
		return i;

	return -1;
}

//-------------------------------------------------------------------------//

le::Bone::Bone()
{
	iIdPerent = -1;
	sNameParentBone = "";

	Matrix Matrix;
	for ( int i = 0; i < 16; i++ )
		StartMatrix[i] = InvertMatrix[i] = Realese[i] = Matrix.matrix[i];
}

//-------------------------------------------------------------------------//

void le::Skeleton::DrawSkeleton( map<string, le::Bone> bones )
{
	/*for ( int i = 0; i < bones.size(); i++ )
	{
	Bone* bn = &bones[i];

	if ( bn->Perent != NULL )
	{
	Vector3f bm1 = Matrix::MatrixToXYZ( bn->Perent->Realese );
	Vector3f bm2 = Matrix::MatrixToXYZ( bn->Realese );

	glBegin( GL_LINES );
	glColor3f( 1, 0, 0 );
	glVertex3f( bm1.x, bm1.y, bm1.z );
	glVertex3f( bm2.x, bm2.y, bm2.z );
	glColor3f( 1, 1, 1 );
	glEnd();
	}

	if ( bn->vChild.size() > 0 )
	DrawSkeleton( bn->vChild );
	}*/
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitMatrixBone( Bone& bone )
{
	if ( bone.iIdPerent != -1 )
		Matrix::MatrixMultiply( bone.StartMatrix, vBones[bone.iIdPerent].Realese, bone.Realese );

	for ( int i = 0; i < bone.vIdChild.size(); i++ )
	{
		Bone* boneChild = &vBones[bone.vIdChild[i]];
		InitMatrixBone( *boneChild );
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitBone()
{
	for ( int i = 0; i < vBones.size(); i++ )
		InitMatrixBone( vBones[i] );
}

//-------------------------------------------------------------------------//

bool le::Skeleton::IsInit()
{
	return IsLoad;
}

//-------------------------------------------------------------------------//