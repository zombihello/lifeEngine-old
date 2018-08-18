#include <System\ResourcesManager.h>
#include <System\VAO.h>
#include "..\Model.h"

//-------------------------------------------------------------------------//

le::Model::Model() :
	Scene( NULL )
{}

//-------------------------------------------------------------------------//

le::Model::~Model()
{
	if ( Scene )
		Scene->RemoveModel( this );

	VAO::DeleteBuffer( &VertexBuffer );

	for ( size_t i = 0; i < ArrayBuffers.size(); i++ )
		VAO::DeleteVAO( &ArrayBuffers[ i ] );

	for ( size_t i = 0; i < IndexBuffers.size(); i++ )
		VAO::DeleteBuffer( &IndexBuffers[ i ] );
}

//-------------------------------------------------------------------------//

bool le::Model::LoadModel( const string& NameModel, const string& RouteToFile )
{
	if ( !ResourcesManager::LoadMesh( NameModel, RouteToFile ) )
		return false;

	LoadModel( *ResourcesManager::GetMesh( NameModel ) );

	return true;
}

//-------------------------------------------------------------------------//

void le::Model::LoadModel( Mesh& Mesh )
{
	vector<MeshVertex> VBO_Vertexs = Mesh.GetVBO_Vertexs();
	vector<GLuint> Textures = Mesh.GetTextures();
	map<GLuint, vector<unsigned int>> IdVertexs = Mesh.GetIdVertexs();
	NoSkeleton = Mesh.IsNoSkeleton();
	Skeleton = Mesh.GetSkeleton();
	BoundingBox = Mesh.GetBoundingBox();
	AnimationManager = Mesh.GetAnimations();

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, VBO_Vertexs, VAO::Dynamic_Draw );

	Scene::InfoMesh InfoMesh;
	int IdTexture = 0;

	for ( auto it = IdVertexs.begin(); it != IdVertexs.end(); it++, IdTexture++ )
	{
		GLuint VertexArray = VAO::CreateVAO();
		VAO::BindVAO( VertexArray );

		VAO::AtachBuffer( VAO::Vertex_Buffer, VertexBuffer );
		GLuint IndexArray = VAO::CreateBuffer( VAO::Index_Buffer, it->second, VAO::Static_Draw );

		InfoMesh.CountIndexs = NUMBER_TO_INT( it->second.size() );
		InfoMesh.MatrixTransformation = &MatrixTransformation;
		InfoMesh.BoundingBox = &BoundingBox; 
		InfoMesh.VertexArray = VertexArray;
		InfoMesh.Position = &Position;
		
		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), ( void* ) ( offsetof( MeshVertex, Position ) ) );
		VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), ( void* ) ( offsetof( MeshVertex, Normal ) ) );
		VAO::SetVertexAttribPointer( VERT_TEXCOORD_DIFFUSE, 2, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), ( void* ) ( offsetof( MeshVertex, TextureCoord ) ) );

		if ( !NoSkeleton )
		{
			VAO::SetVertexAttribPointer( VERT_IDBONES, 4, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), ( void* ) ( offsetof( MeshVertex, IdBones ) ) );
			VAO::SetVertexAttribPointer( VERT_WEIGHTSBONES, 4, GL_FLOAT, GL_FALSE, sizeof( MeshVertex ), ( void* ) ( offsetof( MeshVertex, Weights ) ) );

			InfoMesh.Skeleton = &Skeleton;
		}

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );

		ArrayBuffers.push_back( VertexArray );
		IndexBuffers.push_back( IndexArray );
		RenderMesh[ it->first ] = InfoMesh;
	}

	AnimationManager.SetSkeleton( Skeleton );
	AnimationManager.SetRenderMesh( RenderMesh );
}

//-------------------------------------------------------------------------//

void le::Model::AddToScene( le::Scene& Scene )
{
	Scene.AddModel( this );
}

//-------------------------------------------------------------------------//

void le::Model::RemoveFromScene()
{
	if ( Scene )
		Scene->RemoveModel( this );
}

//-------------------------------------------------------------------------//

map<GLuint, le::Scene::InfoMesh>& le::Model::GetRenderMesh()
{
	return RenderMesh;
}

//-------------------------------------------------------------------------//

le::AnimationManager* le::Model::GetAnimationManager()
{
	return &AnimationManager;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Model::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Model::GetScale()
{
	return ScaleModel;
}

//-------------------------------------------------------------------------//

glm::quat& le::Model::GetRotation()
{
	return Rotation;
}

//-------------------------------------------------------------------------//

glm::mat4& le::Model::GetTransformation()
{
	return MatrixTransformation;
}

//-------------------------------------------------------------------------//

bool le::Model::IsNoSkeleton()
{
	return NoSkeleton;
}

//-------------------------------------------------------------------------//

void le::Model::SetScene( le::Scene* Scene )
{
	this->Scene = Scene;
}

//-------------------------------------------------------------------------//

void le::Model::SetPosition( const glm::vec3& Position )
{
	this->Position = Position;
	MatrixPosition = glm::translate( Position );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::SetRotation( const glm::vec3& Rotation )
{
	glm::vec3 Axis( sin( Rotation.x / 2 ), sin( Rotation.y / 2 ), sin( Rotation.z / 2 ) );
	glm::vec3 Rotations( cos( Rotation.x / 2 ), cos( Rotation.y / 2 ), cos( Rotation.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	this->Rotation = RotateX * RotateY * RotateZ;
	MatrixRotation = glm::mat4_cast( this->Rotation );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::SetRotation( const glm::quat& Rotation )
{
	this->Rotation = Rotation;
	MatrixRotation = glm::mat4_cast( this->Rotation );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::SetScale( const glm::vec3& Scale )
{
	ScaleModel = Scale;
	MatrixScale = glm::scale( Scale );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::Move( const glm::vec3& FactorMove )
{
	Position += FactorMove;
	MatrixPosition *= glm::translate( FactorMove );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::Scale( const glm::vec3& FactorScale )
{
	ScaleModel += FactorScale;
	MatrixScale *= glm::scale( FactorScale );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::Rotate( const glm::vec3& FactorRotate )
{
	glm::vec3 Axis( sin( FactorRotate.x / 2 ), sin( FactorRotate.y / 2 ), sin( FactorRotate.z / 2 ) );
	glm::vec3 Rotations( cos( FactorRotate.x / 2 ), cos( FactorRotate.y / 2 ), cos( FactorRotate.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	Rotation *= RotateX * RotateY * RotateZ;
	MatrixRotation = glm::mat4_cast( Rotation );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//

void le::Model::Rotate( const glm::quat& FactorRotate )
{
	Rotation *= FactorRotate;
	MatrixRotation = glm::mat4_cast( FactorRotate );

	MatrixTransformation = MatrixPosition * MatrixRotation * MatrixScale;
	BoundingBox.SetTransformation( MatrixTransformation, Position, Rotation, ScaleModel );
}

//-------------------------------------------------------------------------//