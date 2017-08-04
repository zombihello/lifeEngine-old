#ifndef LIGHT_MANAGER_3D
#define LIGHT_MANAGER_3D

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include <System\System.h>
#include <System\3D\Camera.h>
#include <System\3D\Scene3D.h>
#include <Efects\3D\Light3D.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class Level3D;

	//-------------------------------------------------------------------------//

	class DLL_API LightManager3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		LightManager3D( System& System );

		////////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ ТЕНИ НА УРОВНЕ
		////////////////////////////////////////
		void InitShadows( Level3D& Level );

		///////////////////////////////////////
		/// ДОБАВИТЬ ИСТОЧНИКИ СВЕТА НА СЦЕНУ
		///////////////////////////////////////
		void AddLightsToScene( Scene3D& Scene );

		///////////////////////////////
		/// ОБНОВИТЬ ИСТОЧНИКИ СВЕТА
		///////////////////////////////
		void UpdateLights();

		////////////////////////////////
		/// СОЗДАТЬ СВЕТ
		////////////////////////////////
		void CreateLight( Light3D Light3D, string NameLight = "" );

		////////////////////////////////
		/// УНИЧТОЖИТЬ СВЕТ
		////////////////////////////////
		void DestroyLight( string NameLight );

		////////////////////////////////
		/// УНИЧТОЖИТЬ ВЕСЬ СВЕТ
		////////////////////////////////
		void DestroyAllLights();

		////////////////////////////////
		/// ПОЛУЧИТЬ СВЕТ
		////////////////////////////////
		Light3D* GetLight( string NameLight );

	private:

		Shader						DepthRender;
		Vector2u					SizeWindow;
		glm::mat4x4*				ProjectionMatrix;

		Scene3D*					Scene;

		vector<Light3D>				vLights;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_MANAGER_3D