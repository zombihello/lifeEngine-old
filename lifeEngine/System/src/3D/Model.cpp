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

	LoadModel( *MaterialManager::GetModelMesh( nameModel ) );

	return true;
}

//-------------------------------------------------------------------------//

void le::Model::LoadModel( le::ModelMesh ModelMesh )
{
	SizeModel = ModelMesh.SizeModel;
	Skeleton = ModelMesh.Skeleton;
	Skeleton.InitSkeleton( VertexBuffer, vVBO_Vertexs, mVertexs );

	AnimationManager3D = ModelMesh.AnimationManager3D;
	AnimationManager3D.SetSkeleton( Skeleton );

	mCountIndexs = ModelMesh.mCountIndexs;
	vVBO_Vertexs = ModelMesh.vVBO_Vertexs;
	mVertexs = ModelMesh.mVertexs;

	if ( ModelMesh.IsCollisionMesh )
	{
		vCollision_DefaultVertexs = vCollision_Vertexs = ModelMesh.vCollision_Vertexs;
		vCollision_IdVertexs = ModelMesh.vCollision_IdVertexs;

		Skeleton.InitCollision( vCollision_Vertexs );
	}

	for ( auto it = ModelMesh.mIdIndexs.begin(); it != ModelMesh.mIdIndexs.end(); it++ )
	{
		GLuint IndexBuffer;
		glGenBuffers( 1, &IndexBuffer );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IndexBuffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, it->second.size() * sizeof( unsigned int ), it->second.data(), GL_DYNAMIC_DRAW );
		mIndexBuffers[ it->first ] = IndexBuffer;
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
	glMultMatrixf( glm::value_ptr( MatrixRotation ) );
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

		glDrawElements( GL_TRIANGLES, mCountIndexs[ it->first ], GL_UNSIGNED_INT, 0 );
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

void le::Model::SetRotate( Vector3f Rotation )
{
	Vector3f Axis( sin( Rotation.x / 2 ), sin( Rotation.y / 2 ), sin( Rotation.z / 2 ) );
	Vector3f Rotations( cos( Rotation.x / 2 ), cos( Rotation.y / 2 ), cos( Rotation.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	this->Rotation = RotateX * RotateY * RotateZ;

	MatrixRotation = glm::mat4_cast( this->Rotation );
}

//-------------------------------------------------------------------------//

void le::Model::SetRotate( glm::quat Rotation )
{
	this->Rotation = Rotation;
	MatrixRotation = glm::mat4_cast( this->Rotation );
}

//-------------------------------------------------------------------------//

void le::Model::SetScale( Vector3f Scale )
{
	this->ScaleModel = Scale;

	for ( int i = 0, id = 0; id < vCollision_DefaultVertexs.size() / 3; id++, i += 3 )
	{
		vCollision_Vertexs[ i ] = vCollision_DefaultVertexs[ i ] * Scale.x;
		vCollision_Vertexs[ i + 1 ] = vCollision_DefaultVertexs[ i + 1 ] * Scale.y;
		vCollision_Vertexs[ i + 2 ] = vCollision_DefaultVertexs[ i + 2 ] * Scale.z;
	}
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

	for ( int i = 0, id = 0; id < vCollision_Vertexs.size( ) / 3; id++, i += 3 )
	{
		vCollision_Vertexs[ i ] *= FactorScale.x;
		vCollision_Vertexs[ i + 1 ] *= FactorScale.y;
		vCollision_Vertexs[ i + 2 ] *= FactorScale.z;
	}
}

//-------------------------------------------------------------------------//

void le::Model::Rotate( Vector3f Rotation )
{
	Vector3f Axis( sin( Rotation.x / 2 ), sin( Rotation.y / 2 ), sin( Rotation.z / 2 ) );
	Vector3f Rotations( cos( Rotation.x / 2 ), cos( Rotation.y / 2 ), cos( Rotation.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	this->Rotation *= RotateX * RotateY * RotateZ;

	MatrixRotation = glm::mat4_cast( this->Rotation );
}

//-------------------------------------------------------------------------//

void le::Model::Rotate( glm::quat Rotation )
{
	this->Rotation *= Rotation;
	MatrixRotation = glm::mat4_cast( this->Rotation );
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

Vector3f le::Model::GetSize()
{
	return SizeModel;
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

glm::quat le::Model::GetRotate()
{
	return Rotation;
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

vector<float>& le::Model::GetCollisionVertexs()
{
	return vCollision_Vertexs;
}

//-------------------------------------------------------------------------//

vector<int>& le::Model::GetCollisionIdVertexs()
{
	return vCollision_IdVertexs;
}

//-------------------------------------------------------------------------//