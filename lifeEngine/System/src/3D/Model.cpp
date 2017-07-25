///////////////////////////
/// BULLET PHYSICS
///////////////////////////
#include <btBulletDynamicsCommon.h>

////////////////
/// LIFEENGINE
///////////////
#include  "../../3D/Model.h"
#include "../../System/MaterialManager.h"
#include "../../../System/LoaderVAO.h"
#include <System\3D\Scene3D.h>

//-------------------------------------------------------------------------//

le::Model::Model( le::System& System )
{
	this->System = &System;
	Scene = NULL;
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

	vVBO_Vertexs = ModelMesh.vVBO_Vertexs;
	mVertexs = ModelMesh.mVertexs;

	if ( ModelMesh.IsCollisionMesh )
	{
		vCollision_Vertexs = ModelMesh.vCollision_Vertexs;
		vCollision_IdVertexs = ModelMesh.vCollision_IdVertexs;

		Skeleton.InitCollision( vCollision_Vertexs );
	}

	VertexBuffer = LoaderVAO::CreateBuffer( GL_ARRAY_BUFFER, vVBO_Vertexs, GL_DYNAMIC_DRAW );

	SceneInfoMesh InfoMesh;
	for ( auto it = ModelMesh.mIdIndexs.begin(); it != ModelMesh.mIdIndexs.end(); it++ )
	{
		GLuint VertexArray = LoaderVAO::CreateVAO();
		LoaderVAO::BindVAO( VertexArray );

		LoaderVAO::AtachBuffer( GL_ARRAY_BUFFER, VertexBuffer );
		GLuint IndexBuffer = LoaderVAO::AtachBuffer( GL_ELEMENT_ARRAY_BUFFER, it->second, GL_DYNAMIC_DRAW );

		LoaderVAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, VBO_ModelVertex::Position ) ) );
		LoaderVAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, VBO_ModelVertex::Normal ) ) );
		LoaderVAO::SetVertexAttribPointer( VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( VBO_ModelVertex ), ( void* ) ( offsetof( VBO_ModelVertex, VBO_ModelVertex::TextureCoord ) ) );

		LoaderVAO::UnbindVAO();
		LoaderVAO::UnbindBuffer( GL_ARRAY_BUFFER );
		LoaderVAO::UnbindBuffer( GL_ELEMENT_ARRAY_BUFFER );

		InfoMesh.CountIndexs = it->second.size();
		InfoMesh.MatrixTransformation = &MatrixTransformation;
		InfoMesh.VertexArray = VertexArray;	
		InfoMesh.Material = ModelMesh.mMaterials[it->first];
		
		vArrayBuffers.push_back( VertexArray );
		vIndexBuffers.push_back( IndexBuffer );
		mRenderMesh[InfoMesh.Material.Texture] = InfoMesh;
	}
}

//-------------------------------------------------------------------------//

void le::Model::AddToScene( Scene3D& Scene )
{
	this->Scene = &Scene;
}

//-------------------------------------------------------------------------//

void le::Model::UpdateModel( Shader* Shader )
{
	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	Scene->AddMeshToScene( mRenderMesh );
}

//-------------------------------------------------------------------------//

void le::Model::SetPosition( Vector3f Position )
{
	this->Position = Position;
	MatrixPosition = glm::translate( glm::vec3( Position.x, Position.y, Position.z ) );
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
	MatrixScale = glm::scale( glm::vec3( Scale.x, Scale.y, Scale.z ) );
}

//-------------------------------------------------------------------------//

void le::Model::SetScale( Vector3f Scale, btTriangleIndexVertexArray* IndexVertexArrays )
{
	this->ScaleModel = Scale;
	MatrixScale = glm::scale( glm::vec3( Scale.x, Scale.y, Scale.z ) );

	if ( IndexVertexArrays != NULL )
		IndexVertexArrays->setScaling( btVector3( Scale.x, Scale.y, Scale.z ) );
}

//-------------------------------------------------------------------------//

void le::Model::Move( Vector3f FactorMove )
{
	Position += FactorMove;
	MatrixPosition *= glm::translate( glm::vec3( FactorMove.x, FactorMove.y, FactorMove.z ) );
}

//-------------------------------------------------------------------------//

void le::Model::Scale( Vector3f FactorScale )
{
	ScaleModel += FactorScale;
	MatrixScale *= glm::scale( glm::vec3( FactorScale.x, FactorScale.y, FactorScale.z ) );
}

//-------------------------------------------------------------------------//

void le::Model::Scale( Vector3f FactorScale, btTriangleIndexVertexArray* IndexVertexArrays )
{
	ScaleModel += FactorScale;
	MatrixScale *= glm::scale( glm::vec3( FactorScale.x, FactorScale.y, FactorScale.z ) );

	if ( IndexVertexArrays != NULL )
	{
		btVector3 CollisionScale = IndexVertexArrays->getScaling();

		IndexVertexArrays->setScaling( btVector3(
			CollisionScale.x() + FactorScale.x,
			CollisionScale.y() + FactorScale.y,
			CollisionScale.z() + FactorScale.z ) );
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
	LoaderVAO::DeleteBuffer( &VertexBuffer );

	for ( int i = 0; i < vArrayBuffers.size(); i++ )
		LoaderVAO::DeleteVAO( &vArrayBuffers[i] );

	for ( int i = 0; i < vIndexBuffers.size(); i++ )
		LoaderVAO::DeleteBuffer( &vIndexBuffers[i] );

	Skeleton.ClearSkeleton();
	vArrayBuffers.clear();
	vVBO_Vertexs.clear();
	vIndexBuffers.clear();
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