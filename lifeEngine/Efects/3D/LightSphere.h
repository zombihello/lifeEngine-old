#ifndef LIGHT_SPHERE_H
#define LIGHT_SPHERE_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

////////////////
/// LIFEENGINE
///////////////
#include <System\System.h>
#include <System\LoaderVAO.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API LightSphere
	{
	public:

		////////////////////////////////////
		/// КОНСТРУКТОР
		///////////////////////////////////
		LightSphere();
		LightSphere( const LightSphere& Copy );

		////////////////////////////////////
		/// ДЕСТРУКТОР
		///////////////////////////////////
		~LightSphere();

		////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ СФЕРУ
		///////////////////////////////////
		void InitSphere( float Radius, float Detail );

		////////////////////////////////////
		/// ОТРЕНДЕРИТЬ СФЕРУ
		///////////////////////////////////
		void RenderSphere();

		////////////////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ СФЕРЫ
		///////////////////////////////////
		void SetPosition( glm::vec3 Position );

		////////////////////////////////////
		/// ЗАДАТЬ РАДИУС СФЕРЫ
		///////////////////////////////////
		void SetRadius( float Radius );

		////////////////////////////////////
		/// ПОЛУЧИТЬ МАТРИЦУ ТРАНСФОРМАЦИИ
		///////////////////////////////////
		glm::mat4& GetTransformationMatrix();

	private:

		int					iCountVertexs;

		float				fDetail;
		float				fRadius;

		glm::vec3			Position;
		glm::mat4			transformationMatrix;

		GLuint				VertexArray;
		GLuint				VertexBuffer;

		vector<glm::vec3>	vVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_SPHERE_H