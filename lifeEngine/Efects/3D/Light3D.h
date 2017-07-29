#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include <System\System.h>
#include "LightSphere.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Light3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Light3D();
		Light3D( glm::vec3 Position, float Radius, float Intensivity, glm::vec4 Color, glm::vec4 Specular = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

		///////////////////////////
		/// ЗАДАТЬ РАДИУС СВЕТА
		///////////////////////////
		void SetRadius( float Radius );

		////////////////////////////////
		/// ЗАДАТЬ ИНТЕНСИВНОСТЬ СВЕТА
		//////////////////////////////
		void SetIntensivity( float Intensivity );

		///////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ СВЕТА
		///////////////////////////
		void SetPosition( glm::vec3 Position );

		///////////////////////////
		/// ЗАДАТЬ ЦВЕТ СВЕТА
		///////////////////////////
		void SetColor( glm::vec4 Color );

		//////////////////////////////
		/// ЗАДАТЬ ОТРАЖЕНИЕ СВЕТА
		//////////////////////////////
		void SetSpecular( glm::vec4 Specular );

		//////////////////////////////
		/// ЗАДАТЬ ИМЯ СВЕТА
		//////////////////////////////
		void SetName( string NameLight );

		float			fRadius;
		float			fIntensivity;

		string			NameLight;

		glm::vec3		Position;
		glm::vec4		Specular;
		glm::vec4		Color;

		LightSphere     LightSphere;	
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_3D_H