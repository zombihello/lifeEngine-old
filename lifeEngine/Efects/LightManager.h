#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "Light.h"
#include "../System/leSystem.h"
#include "../Level/Object.h"

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
		void CreateLight( Vector2f Position, float Radius, Color Color, string NameLight );
		void CreateLight( Object obj );

		/////////////////////
		/// НАРИСОВАТЬ СВЕТ
		////////////////////
		void DrawLight();

		//////////////////////
		/// УДАЛИТЬ ВЕСЬ СВЕТ
		/////////////////////
		void DeleteAllLight();

		//////////////////
		/// УДАЛИТЬ СВЕТ
		/////////////////
		void DeleteLight( int id );
		void DeleteLight( string NameLight );

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
		void SetColorShadow( Vector3f color );

		////////////////////////
		/// ЗАДАТЬ РАДИУС СВЕТА
		////////////////////////
		void SetRadius( int id, float Radius );
		void SetRadius( string NameLight, float Radius );

		/////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ СВЕТА
		////////////////////////
		void SetPosition( int id, Vector2f Position );
		void SetPosition( string NameLight, Vector2f Position );

		///////////////////////
		/// ЗАДАТЬ ЦВЕТ СВЕТА
		//////////////////////
		void SetColor( int id, Color Color );
		void SetColor( string NameLight, Color Color );

		///////////////////
		/// ПОЛУЧИТЬ СВЕТ
		//////////////////
		Light* GetLight( int id );
		Light* GetLight( string NameLight );

		///////////////////////////
		/// ПОЛУЧИТЬ РАДИУС СВЕТА
		//////////////////////////
		float GetRadius( int id );
		float GetRadius( string NameLight );

		///////////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ СВЕТА
		//////////////////////////
		Vector2f GetPosition( int id );
		Vector2f GetPosition( string NameLight );

		///////////////////////////
		/// ПОЛУЧИТЬ ЦВЕТ СВЕТА
		//////////////////////////
		Color GetColor( int id );
		Color GetColor( string NameLight );

		//////////////////////////
		/// ПОЛУЧИТЬ ЦВЕТ ТЕНИ
		/////////////////////////
		Vector3f GetColorShadow();
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
