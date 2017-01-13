#ifndef MODEL_H
#define MODEL_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"
#include "Skeleton.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Vertex
	{
		Vector3f			Position;
		vector<int>			vIdBones;
		vector<float>		vIdWeihgt;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API TextureCoord
	{
		TextureCoord();

		Vector2f		Coords;
		GLuint			gl_Texture;
		bool			TextureEmpty;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API IDsVNTC
	{
		IDsVNTC();

		int		idVertex;
		int		idNormal;
		int		idTextureCoord;
		int		idVertexColor;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API Mesh
	{
		vector<IDsVNTC>			vIDs;

		vector<Vertex>			vVertexs;
		vector<Vector3f>		vNormals;
		vector<TextureCoord>	vTextureCoords;
		vector<Color>			vVertexColors;
	};

	//-------------------------------------------------------------------------//

	struct TMP
	{
		Color col = Color::Green;
		Vector3f pos;
	};

	class DLL_API Model
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Model( RenderWindow& RenderWindow );

		//////////////////////
		/// ЗАГРУЗКА МОДЕЛИ
		//////////////////////
		bool LoadModel( string route );

		//////////////////////
		/// ОТРИСОВКА МОДЕЛИ
		//////////////////////
		void RenderModel();

	private:
		GLuint LoadTexture( string route );

		Mesh							Mesh;
		Skeleton						Skeleton;

		sf::RenderWindow*				RenderWindow;
	};

	//-------------------------------------------------------------------------//
}

#endif // MODEL_H