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
		Scene->RemoveModelFromScene( this );

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
	vector<Mesh::MeshVertex> VBO_Vertexs = Mesh.GetVBO_Vertexs();
	vector<GLuint> Textures = Mesh.GetTextures();
	map<GLuint, vector<unsigned int>> IdVertexs = Mesh.GetIdVertexs();

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, VBO_Vertexs, VAO::Dynamic_Draw );

	Scene::InfoMesh InfoMesh;
	int IdTexture = 0;

	for ( auto it = IdVertexs.begin(); it != IdVertexs.end(); it++, IdTexture++ )
	{
		GLuint VertexArray = VAO::CreateVAO();
		VAO::BindVAO( VertexBuffer );

		VAO::AtachBuffer( VAO::Vertex_Buffer, VertexBuffer );
		GLuint IndexArray = VAO::CreateBuffer( VAO::Index_Buffer, it->second, VAO::Static_Draw );

		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::MeshVertex ), ( void* ) ( offsetof( Mesh::MeshVertex, Position ) ) );
		VAO::SetVertexAttribPointer( VERT_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof( Mesh::MeshVertex ), ( void* ) ( offsetof( Mesh::MeshVertex, Normal ) ) );
		VAO::SetVertexAttribPointer( VERT_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof( Mesh::MeshVertex ), ( void* ) ( offsetof( Mesh::MeshVertex, TextureCoord ) ) );

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );

		InfoMesh.CountIndexs = NUMBER_TO_INT( it->second.size() );
		InfoMesh.MatrixTransformation = &MatrixTransformation;
		InfoMesh.VertexArray = VertexArray;

		ArrayBuffers.push_back( VertexArray );
		IndexBuffers.push_back( IndexArray );
		RenderMesh[ it->first ] = InfoMesh;
	}
}

//-------------------------------------------------------------------------//

void le::Model::AddToScene( le::Scene& Scene )
{
	Scene.AddModelToScene( this );
}

//-------------------------------------------------------------------------//

void le::Model::RemoveFromScene()
{
	if ( Scene )
		Scene->RemoveModelFromScene( this );
}

//-------------------------------------------------------------------------//

map<GLuint, le::Scene::InfoMesh>& le::Model::GetRenderMesh()
{
	return RenderMesh;
}

//-------------------------------------------------------------------------//

void le::Model::SetScene( le::Scene* Scene )
{
	this->Scene = Scene;
}

//-------------------------------------------------------------------------//