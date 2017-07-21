#ifndef LIGHT_MANAGER_3D
#define LIGHT_MANAGER_3D

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../../System/System.h"
#include "../../System/3D/Camera.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Light3D
	{
		glm::vec3		Position;
		glm::vec3		Color;
	};

	//-------------------------------------------------------------------------//

	class DLL_API LightManager3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		LightManager3D( Camera& PlayerCamera );

		///////////////////////////////
		/// ОБНОВИТЬ ИСТОЧНИКИ СВЕТА
		///////////////////////////////
		void UpdateLights();

		/////////////////////////////
		/// НАЧАТЬ ПРИМИНЯТЬ СВЕТ
		/////////////////////////////
		void StartApplyLights();

		////////////////////////////////
		/// ПРЕКРАТИТЬ ПРИМИНЯТЬ СВЕТ
		////////////////////////////////
		void StopApplyLights();

		////////////////////////////////
		/// СОЗДАТЬ СВЕТ
		////////////////////////////////
		void CreateLight( string NameLight, Light3D Light3D );

		////////////////////////////////
		/// УНИЧТОЖИТЬ СВЕТ
		////////////////////////////////
		void DestroyLight( string NameLight );

		////////////////////////////////
		/// УНИЧТОЖИТЬ ВЕСЬ СВЕТ
		////////////////////////////////
		void DestroyAllLights();

		////////////////////////////////
		/// ЗАДАТЬ КАМЕРУ ИГРОКА
		////////////////////////////////
		void SetPlayerCamera( Camera& PlayerCamera );

		////////////////////////////////
		/// ПОЛУЧИТЬ СВЕТ
		////////////////////////////////
		Light3D* GetLight( string NameLight );

	private:

		Shader						ShaderLight;

		Camera*						PlayerCamera;

		map<string, Light3D>		mLights;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_MANAGER_3D