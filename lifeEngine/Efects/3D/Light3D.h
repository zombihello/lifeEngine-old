#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#define COMPILING_LIBRARY
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024
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
		Light3D( const Light3D& Copy );
		Light3D( glm::vec3 Position, float Radius, float Intensivity, glm::vec4 Color, glm::vec4 Specular = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );

		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~Light3D();

		/////////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ КАРТУ ГЛУБИНЫ
		/////////////////////////////////////////
		void InitDepthMap();

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

		bool					bIsInitDepthMap;

		float					fRadius;
		float					fIntensivity;

		string					sNameLight;

		GLuint					Cubemap_DepthMap;
		GLuint					FBO_DepthMap;

		glm::vec3				Position;
		glm::vec4				Specular;
		glm::vec4				Color;

		LightSphere				LightSphere;

		glm::mat4x4				ShadowTransforms[ 6 ];
		glm::mat4x4				ShadowProjection;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_3D_H