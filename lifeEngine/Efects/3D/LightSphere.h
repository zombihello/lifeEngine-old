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
		/// ПОЛУЧИТЬ МАТРИЦУ ТРАНСФОРМАЦИИ
		///////////////////////////////////
		glm::mat4* GetTransformationMatrix();

	private:

		int					iCountVertexs;

		glm::vec3			Position;
		glm::mat4			transformationMatrix;

		GLuint				VertexArray;
		GLuint				VertexBuffer;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_SPHERE_H