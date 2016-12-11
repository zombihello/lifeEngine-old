#ifndef MODEL_H
#define MODEL_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct Poligon
	{
		vector<Vector3f>		vPoints;
		vector<Vector3f>		vNormals;
		vector<Vector2f>		vTextureCoords;
		vector<Color>			vVertexColors;
	};

	//-------------------------------------------------------------------------//

	struct MainPoligon
	{
		vector<Poligon>			vPoligons;
		GLuint					gl_Texture;
	};

	//-------------------------------------------------------------------------//

	class DLL_API Model
	{
	public:
		/////////////////
		///  ŒÕ—“–” “Œ–
		/////////////////
		Model( RenderWindow& RenderWindow );

		/////////////////
		/// ƒ≈—“–” “Œ–
		/////////////////
		~Model();

		//////////////////////
		/// «¿√–”« ¿ ÃŒƒ≈À»
		//////////////////////
		bool LoadModel( string route );

		//////////////////////
		/// Œ“–»—Œ¬ ¿ ÃŒƒ≈À»
		//////////////////////
		void RenderModel();

	private:
		GLuint LoadTexture( string route );

		sf::RenderWindow*				RenderWindow;

		std::vector<MainPoligon>			vMainPoligons;
	};
}

#endif // MODEL_H