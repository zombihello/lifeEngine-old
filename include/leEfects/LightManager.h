#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "Light.h"
#include "../leSystem/leSystem.h"
#include "../leLevel/Object.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API LightManager
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		LightManager( System& System );
		LightManager( System& System, Vector2f SizeMap );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~LightManager();

		///////////////////////////////
		/// ЗАГРУЗИТЬ МАСКУ ДЛЯ СВЕТА
		///////////////////////////////
		void LoadLightMask( string Route, bool Smooth = false );

		///////////////////
		/// СОЗДАТЬ СВЕТ
		//////////////////
		void CreateLight( Vector2f Position, float Radius, Color Color );
		void CreateLight( Object obj );

		/////////////////////
		/// НАРИСОВАТЬ СВЕТ
		////////////////////
		void DrawLight();

		//////////////////////
		/// УДАЛИТЬ ВЕСЬ СВЕТ
		/////////////////////
		void DeleteAllLight();

		////////////////////////
		/// УДАЛИТЬ СВЕТ ПО ID
		///////////////////////
		void DeleteLight( int id );

		//////////////////////////
		/// ЗАДАТЬ РАЗМЕРЫ КАРТЫ
		/////////////////////////
		void SetSizeMap( Vector2f SizeMap );

		//////////////////////////
		/// ЗАДАТЬ ПРОЗРАЧНОСТЬ
		/////////////////////////
		void SetOpencity( float Opencity );

		//////////////////////////
		/// ЗАДАТЬ ЦВЕТ ТЕНИ
		/////////////////////////
		void SetColorShadow( float r, float g, float b );
	private:
		float							color[ 3 ];

		View*							Camera;
		sf::RenderWindow*				RenderWindow;
		sf::RenderTexture				RenderTexture;
		sf::Texture						Texture;
		Vector2f						SizeMap;
		vector<Light*>					vLight;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LIGHTMANAGER_H
