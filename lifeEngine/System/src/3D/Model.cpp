#include  "../../3D/Model.h"
#include "../../System/MaterialManager.h"

//-------------------------------------------------------------------------//

le::Model::Model( le::System& System )
{
	this->System = &System;
	ScaleModel = Vector3f( 1, 1, 1 );
}

//-------------------------------------------------------------------------//

le::Model::~Model()
{
	DeleteModel();
}

//-------------------------------------------------------------------------//

bool le::Model::LoadModel( string nameModel, string route )
{
	if ( !MaterialManager::LoadModelMesh( nameModel, route ) )
		return false;

	LoadModel( MaterialManager::GetModelMesh( nameModel ) );

	return true;
}

//-------------------------------------------------------------------------//

void le::Model::LoadModel( le::ModelMesh ModelMesh )
{
	Skeleton = ModelMesh.Skeleton;
	Skeleton.InitSkeleton( VertexBuffer, vVBO_Vertexs, mVertexs );

	AnimationManager3D = ModelMesh.AnimationManager3D;
	AnimationManager3D.SetSkeleton( Skeleton );

	mCountIndexs = ModelMesh.mCountIndexs;
	vVBO_Vertexs = ModelMesh.vVBO_Vertexs;
	mVertexs = ModelMesh.mVertexs;

	for ( auto it = ModelMesh.mIdIndexs.begin(); it != ModelMesh.mIdIndexs.end(); it++ )
	{
		GLuint IndexBuffer;
		glGenBuffers( 1, &IndexBuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, it->second.size() * sizeof( unsigned int ), it->second.data(), GL_DYNAMIC_DRAW );
		mIndexBuffers[it->first] = IndexBuffer;
	}

	glGenBuffers( 1, &VertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, vVBO_Vertexs.size() * sizeof( VBO_ModelVertex ), vVBO_Vertexs.data(), GL_DYNAMIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

void le::Model::RenderModel()
{
	glPushMatrix();
	glTranslatef( Position.x, Position.y, Position.z );
	glRotatef( Angle.x, 1, 0, 0 );
	glRotatef( Angle.y, 0, 1, 0 );
	glRotatef( Angle.z, 0, 0, 1 );
	glScalef( ScaleModel.x, ScaleModel.y, ScaleModel.z );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glNormalPointer( GL_FLOAT, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, Normal ) ) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, TextureCoord ) ) );
	glVertexPointer( 3, GL_FLOAT, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, Position ) ) );

	for ( auto it = mIndexBuffers.begin(); it != mIndexBuffers.end(); it++ )
	{
		glBindTexture( GL_TEXTURE_2D, MaterialManager::GetGLTexture( it->first ) );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, it->second );

		glDrawElements( GL_TRIANGLES, mCountIndexs[it->first], GL_UNSIGNED_INT, 0 );
	}

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );

	glPopMatrix();
}

//-------------------------------------------------------------------------//

void le::Model::SetPosition( Vector3f Position )
{
	this->Position = Position;
}

//-------------------------------------------------------------------------//

void le::Model::SetRotate( Vector3f Angle )
{
	this->Angle = Angle;
}

//-------------------------------------------------------------------------//

void le::Model::SetScale( Vector3f Scale )
{
	this->ScaleModel = Scale;
}

//-------------------------------------------------------------------------//

void le::Model::Move( Vector3f FactorMove )
{
	Position += FactorMove;
}

//-------------------------------------------------------------------------//

void le::Model::Scale( Vector3f FactorScale )
{
	ScaleModel += FactorScale;
}

//-------------------------------------------------------------------------//

void le::Model::Rotate( Vector3f Angle )
{
	this->Angle += Angle;
}

//-------------------------------------------------------------------------//

void le::Model::DeleteModel()
{
	glDeleteBuffers( 1, &VertexBuffer );

	for ( auto it = mIndexBuffers.begin(); it != mIndexBuffers.end(); it++ )
		glDeleteBuffers( 1, &it->second );

	Skeleton.ClearSkeleton();
	mCountIndexs.clear();
	vVBO_Vertexs.clear();
	mIndexBuffers.clear();
	mVertexs.clear();
}

//-------------------------------------------------------------------------//

le::AnimationManager3D& le::Model::GetAnimationManager()
{
	return AnimationManager3D;
}

//-------------------------------------------------------------------------//

Vector3f le::Model::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

Vector3f le::Model::GetScale()
{
	return ScaleModel;
}

//-------------------------------------------------------------------------//

Vector3f le::Model::GetRotate()
{
	return Angle;
}

//-------------------------------------------------------------------------//

map<int, le::ModelVertex>& le::Model::GetVertexs()
{
	return mVertexs;
}

//-------------------------------------------------------------------------//

vector<le::VBO_ModelVertex>& le::Model::GetVBO_Vertexs()
{
	return vVBO_Vertexs;
}

//-------------------------------------------------------------------------//

GLuint& le::Model::GetVertexBuffer()
{
	return VertexBuffer;
}

//-------------------------------------------------------------------------//