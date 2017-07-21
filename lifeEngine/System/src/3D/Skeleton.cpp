#include "..\..\3D\Skeleton.h"
#include "..\..\3D\Model.h"

//-------------------------------------------------------------------------//

le::Skeleton::Skeleton()
{
	IsLoad = IsCollision = false;

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
	IsLoad = IsCollision = false;

	mModelVertexs = NULL;
	vVBO_Vertexs = NULL;
	VertexBuffer = NULL;

	vBones.clear();
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitMesh( map<int, ModelVertex>& ModelVertexs, vector<VBO_ModelVertex>& VBO_Vertexs )
{
	map<int, bool> IsFindVertex;

	for ( int i = 0; i < vBones.size(); i++ )
	{
		Bone* bone = &vBones[i];

		for ( int j = 0; j < bone->vIdVertex.size(); j++ )
		{
			ModelVertex* Vertex = &ModelVertexs[bone->vIdVertex[j]];
			float weight = bone->vWeights[j];

			if ( IsFindVertex.find( bone->vIdVertex[j] ) == IsFindVertex.end() )
			{
				Vertex->BoneTransform = ( bone->InvertMatrix * bone->Realese ) * weight;
				IsFindVertex[bone->vIdVertex[j]] = true;
			}
			else
				Vertex->BoneTransform += ( bone->InvertMatrix * bone->Realese ) * weight;
		}
	}

	glm::vec3 newVertex;
	glm::vec3 newNormal;

	for ( auto it = ModelVertexs.begin(); it != ModelVertexs.end(); it++ )
	{
		ModelVertex* Vertex = &it->second;
		newVertex = glm::vec4( Vertex->Position.x, Vertex->Position.z, -Vertex->Position.y, 1.0f ) * Vertex->BoneTransform;
		newNormal = glm::vec4( Vertex->Normal.x, Vertex->Normal.z, -Vertex->Normal.y, 0.0f ) * Vertex->BoneTransform;

		for ( int i = 0; i < Vertex->vIdVBO_Vertex.size(); i++ )
		{
			VBO_ModelVertex* VBO_ModelVertex = &VBO_Vertexs[Vertex->vIdVBO_Vertex[i]];
			VBO_ModelVertex->Position = newVertex;
			VBO_ModelVertex->Normal = glm::normalize( newNormal );
		}
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::InitCollisionMesh( vector<float>& Vertexs )
{
	map<int, bool> IsFindVertex;
	map<int, glm::mat4x4> mMatrix;
	map<int, glm::mat4x4> mMatrixVertex;

	for ( int i = 0; i < vBones.size(); i++ )
	{
		Bone* bone = &vBones[i];

		for ( int j = 0; j < bone->vCollision_IdVertex.size(); j++ )
		{
			int idVertex = bone->vCollision_IdVertex[j];
			float weight = bone->vCollision_Weights[j];

			if ( IsFindVertex.find( idVertex ) == IsFindVertex.end() )
			{
				IsFindVertex[idVertex] = true;
				mMatrix[idVertex] = ( bone->InvertMatrix * bone->Realese ) * weight;
			}
			else
				mMatrix[idVertex] += ( bone->InvertMatrix * bone->Realese ) * weight;
		}
	}

	for ( int i = 0, id = 0; id < Vertexs.size() / 3; id++, i += 3 )
	{
		mMatrixVertex[id][0].w = Vertexs[i];
		mMatrixVertex[id][1].w = Vertexs[i + 2];
		mMatrixVertex[id][2].w = -Vertexs[i + 1];

		mMatrixVertex[id] *= mMatrix[id];

		Vertexs[i] = mMatrixVertex[id][0].w;
		Vertexs[i + 1] = mMatrixVertex[id][1].w;
		Vertexs[i + 2] = mMatrixVertex[id][2].w;
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMesh()
{
	if ( !IsLoad ) return;

	map<int, bool> IsFindVertex;

	for ( int i = 0; i < vBones.size(); i++ )
	{
		Bone* bone = &vBones[i];

		for ( int j = 0; j < bone->vIdVertex.size(); j++ )
		{
			ModelVertex* Vertex = &( *mModelVertexs )[bone->vIdVertex[j]];
			float weight = bone->vWeights[j];

			if ( IsFindVertex.find( bone->vIdVertex[j] ) == IsFindVertex.end() )
			{
				Vertex->BoneTransform = ( bone->InvertMatrix * bone->Realese ) * weight;
				IsFindVertex[bone->vIdVertex[j]] = true;
			}
			else
				Vertex->BoneTransform += ( bone->InvertMatrix * bone->Realese ) * weight;
		}
	}

	glm::vec3 newVertex;
	glm::vec3 newNormal;

	for ( auto it = mModelVertexs->begin(); it != mModelVertexs->end(); it++ )
	{
		ModelVertex* Vertex = &it->second;
		newVertex = glm::vec4( Vertex->Position.x, Vertex->Position.z, -Vertex->Position.y, 1.0f ) * Vertex->BoneTransform;
		newNormal = glm::vec4( Vertex->Normal.x, Vertex->Normal.z, -Vertex->Normal.y, 0.0f ) * Vertex->BoneTransform;

		for ( int i = 0; i < Vertex->vIdVBO_Vertex.size(); i++ )
		{
			VBO_ModelVertex* VBO_ModelVertex = &( *vVBO_Vertexs )[Vertex->vIdVBO_Vertex[i]];
			VBO_ModelVertex->Position = newVertex;
			VBO_ModelVertex->Normal = glm::normalize( newNormal );
		}
	}

	if ( IsCollision )
	{
		IsFindVertex.clear();

		for ( int i = 0; i < vBones.size(); i++ )
		{
			Bone* bone = &vBones[i];

			for ( int j = 0; j < bone->vCollision_IdVertex.size(); j++ )
			{
				int idVertex = bone->vCollision_IdVertex[j];
				float weight = bone->vCollision_Weights[j];

				CollisionVertex* Vertex = &vCollision_Vertexs[idVertex];

				if ( IsFindVertex.find( idVertex ) == IsFindVertex.end() )
				{
					Vertex->BoneTransform = ( bone->InvertMatrix * bone->Realese ) * weight;
					IsFindVertex[idVertex] = true;
				}
				else
					Vertex->BoneTransform += ( bone->InvertMatrix * bone->Realese ) * weight;
			}
		}

		for ( int i = 0; i < vCollision_Vertexs.size(); i++ )
		{
			CollisionVertex* Vertex = &vCollision_Vertexs[i];
			newVertex = glm::vec4( Vertex->DefaultPosition, 1.0f ) * Vertex->BoneTransform;

			*Vertex->Position[0] = newVertex.x;
			*Vertex->Position[1] = newVertex.y;
			*Vertex->Position[2] = newVertex.z;
		}
	}

	glBindBuffer( GL_ARRAY_BUFFER, *VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vVBO_Vertexs->size() * sizeof( VBO_ModelVertex ), vVBO_Vertexs->data(), GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

void le::Skeleton::ReadingBone( TiXmlElement* node, Bone* bone )
{
	// Работаем с контейнером startMatrix
	TiXmlElement *startMatrix;
	startMatrix = node->FirstChildElement( "startMatrix" );

	string sTmpMatrix;

	if ( node->Attribute( "name" ) != NULL )
		bone->sNameBone = node->Attribute( "name" );

	if ( startMatrix->GetText() != NULL )
	{
		sTmpMatrix = startMatrix->GetText();
		istringstream strStream( sTmpMatrix );
		glm::vec4 tmpVec;

		for ( int id = 0, axis = 1; id < 4, !strStream.eof(); axis++ )
		{
			string sTmp;
			strStream >> sTmp;

			switch ( axis )
			{
			case 1:
				tmpVec.x = atof( sTmp.c_str() );
				break;

			case 2:
				tmpVec.y = atof( sTmp.c_str() );
				break;

			case 3:
				tmpVec.z = atof( sTmp.c_str() );
				break;

			case 4:
				tmpVec.w = atof( sTmp.c_str() );
				bone->StartMatrix[id] = tmpVec;

				id++;
				axis = 0;
				break;
			}
		}
	}

	// Работаем с контейнером invertMatrix
	TiXmlElement *invertMatrix;
	invertMatrix = node->FirstChildElement( "invertMatrix" );

	if ( invertMatrix->GetText() != NULL )
	{
		sTmpMatrix = string( invertMatrix->GetText() );
		istringstream _strStream( sTmpMatrix );

		glm::vec4 tmpVec;

		for ( int id = 0, axis = 1; id < 4, !_strStream.eof(); axis++ )
		{
			string sTmp;
			_strStream >> sTmp;

			switch ( axis )
			{
			case 1:
				tmpVec.x = atof( sTmp.c_str() );
				break;

			case 2:
				tmpVec.y = atof( sTmp.c_str() );
				break;

			case 3:
				tmpVec.z = atof( sTmp.c_str() );
				break;

			case 4:
				tmpVec.w = atof( sTmp.c_str() );
				bone->InvertMatrix[id] = tmpVec;

				id++;
				axis = 0;
				break;
			}
		}
	}

	bone->Realese = bone->InterpolatedMatrix = bone->StartMatrix;

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

void le::Skeleton::LoadSkeleton( TiXmlElement* skeleton )
{
	vector<int>			vTmpIdVertexs;
	vector<float>		vWeights;

	// Работаем с контейнером bindShapeMatrix
	TiXmlElement *bsm;
	bsm = skeleton->FirstChildElement( "bindShapeMatrix" );

	//Matrixf tmpBSM;
	stringstream strStream( bsm->GetText() );
	glm::vec4 tmpVec;

	for ( int id = 0, axis = 1; id < 4, !strStream.eof(); axis++ )
	{
		string sTmp;
		strStream >> sTmp;

		switch ( axis )
		{
		case 1:
			tmpVec.x = atof( sTmp.c_str() );
			break;

		case 2:
			tmpVec.y = atof( sTmp.c_str() );
			break;

		case 3:
			tmpVec.z = atof( sTmp.c_str() );
			break;

		case 4:
			tmpVec.w = atof( sTmp.c_str() );
			BindShape[id] = tmpVec;

			id++;
			axis = 0;
			break;
		}
	}

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
		else
			vRootBones.push_back( i );
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

	Bone* tmpBone;
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
			tmpBone = GetBone( vJoints[atoi( tmp.c_str() )] );
			break;

		case 2:
			if ( tmpNumBones == 0 )
			{
				idVertex++;
				numBones = vTmpIdVertexs[idVertex];

				if ( numBones != 1 )
					tmpNumBones = numBones - 1;

				tmpBone->vIdVertex.push_back( idVertex );
			}
			else
			{
				tmpBone->vIdVertex.push_back( idVertex );
				tmpNumBones--;
			}

			tmpBone->vWeights.push_back( vWeights[atoi( tmp.c_str() )] );

			id = 0;
			break;
		}
	}

	InitBone();
}

//-------------------------------------------------------------------------//

void le::Skeleton::LoadCollisionInfo( TiXmlElement* skeleton )
{
	vector<int>			vTmpIdVertexs;
	vector<float>		vWeights;
	vector<string>		vJoints;

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

	for ( int i = 0; !strJoints.eof(); i++ )
	{
		string tmp;
		strJoints >> tmp;
		vJoints.push_back( tmp );
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

	Bone* tmpBone;
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
			tmpBone = GetBone( vJoints[atoi( tmp.c_str() )] );
			break;

		case 2:
			if ( tmpNumBones == 0 )
			{
				idVertex++;
				numBones = vTmpIdVertexs[idVertex];

				if ( numBones != 1 )
					tmpNumBones = numBones - 1;

				tmpBone->vCollision_IdVertex.push_back( idVertex );
			}
			else
			{
				tmpBone->vCollision_IdVertex.push_back( idVertex );
				tmpNumBones--;
			}

			tmpBone->vCollision_Weights.push_back( vWeights[atoi( tmp.c_str() )] );

			id = 0;
			break;
		}
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateHierarchy()
{
	for ( int i = 0; i < vRootBones.size(); i++ )
	{
		Bone* bone = &vBones[vRootBones[i]];
		UpdateMatrixBone( *bone );
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMatrixBone( string nameBone, glm::mat4x4 Matrix )
{
	Bone* Bone = GetBone( nameBone );

	if ( Bone != NULL )
	{
		Bone->InterpolatedMatrix = Matrix;

		if ( Bone->iIdPerent != -1 )
			Bone->Realese = Matrix * vBones[Bone->iIdPerent].Realese;

		for ( int i = 0; i < Bone->vIdChild.size( ); i++ )
		{
			le::Bone* boneChild = &vBones[Bone->vIdChild[i]];
			UpdateMatrixBone( *boneChild );
		}
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::UpdateMatrixBone( le::Bone& bone )
{
	if ( bone.iIdPerent != -1 )
		bone.Realese = bone.InterpolatedMatrix * vBones[bone.iIdPerent].Realese;

	for ( int i = 0; i < bone.vIdChild.size( ); i++ )
	{
			Bone* boneChild = &vBones[bone.vIdChild[i]];
			UpdateMatrixBone( *boneChild );
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

void le::Skeleton::InitCollision( vector<float>& Vertexs )
{
	IsCollision = true;

	for ( int i = 0, j = 0; j < Vertexs.size() / 3; i += 3, j++ )
	{
		CollisionVertex collisionVertex;
		collisionVertex.DefaultPosition = glm::vec3( Vertexs[i], Vertexs[i+1], Vertexs[i+2] );

		collisionVertex.Position[0] = &Vertexs[i];
		collisionVertex.Position[1] = &Vertexs[i + 1];
		collisionVertex.Position[2] = &Vertexs[i + 2];

		vCollision_Vertexs.push_back( collisionVertex );
	}
}

//-------------------------------------------------------------------------//

void le::Skeleton::AddBone( Bone bone )
{
	vBones.push_back( bone );
}

//-------------------------------------------------------------------------//

glm::mat4x4 le::Skeleton::GetBindShape()
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
}

//-------------------------------------------------------------------------//

/*void DrawSke( vector<int> b, vector<le::Bone> v )
{
	for ( int i = 0; i < b.size(); i++ )
	{
		le::Bone* bn = &v[b[i]];

		if ( bn->iIdPerent != -1 )
		{
			Vector3f bm1 = Vector3f( v[bn->iIdPerent].Realese[0].w, v[bn->iIdPerent].Realese[1].w, v[bn->iIdPerent].Realese[2].w );
			Vector3f bm2 = Vector3f( bn->Realese[0].w, bn->Realese[1].w, bn->Realese[2].w );

			glBegin( GL_LINES );
			glColor3f( 1, 0, 0 );
			glVertex3f( bm1.x, bm1.y, bm1.z );
			glVertex3f( bm2.x, bm2.y, bm2.z );
			glColor3f( 1, 1, 1 );
			glEnd();
		}

		if ( bn->vIdChild.size() > 0 )
			DrawSke( bn->vIdChild, v );
	}
}

void le::Skeleton::DrawSkeleton( vector<Bone>& bones )
{
	for ( int i = 0; i < bones.size(); i++ )
	{
		Bone* bn = &bones[i];

		if ( bn->iIdPerent != -1 )
		{
			Vector3f bm1 = Vector3f( vBones[bn->iIdPerent].Realese[0].w, vBones[bn->iIdPerent].Realese[1].w, vBones[bn->iIdPerent].Realese[2].w );
			Vector3f bm2 = Vector3f( bn->Realese[0].w, bn->Realese[1].w, bn->Realese[2].w );

			glBegin( GL_LINES );
			glColor3f( 1, 0, 0 );
			glVertex3f( bm1.x, bm1.y, bm1.z );
			glVertex3f( bm2.x, bm2.y, bm2.z );
			glColor3f( 1, 1, 1 );
			glEnd();
		}

		if ( bn->vIdChild.size() > 0 )
			DrawSke( bn->vIdChild, vBones );
	}
}*/

//-------------------------------------------------------------------------//

void le::Skeleton::InitMatrixBone( Bone& bone )
{
	if ( bone.iIdPerent != -1 )
		bone.Realese = bone.StartMatrix * vBones[bone.iIdPerent].Realese;

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