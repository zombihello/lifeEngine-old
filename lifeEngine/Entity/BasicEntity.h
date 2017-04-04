#ifndef BASICENTITY_H
#define BASICENTITY_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System/leSystem.h"
#include "../Physic/lePhysic.h"
#include "../Animation/leAnimation.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class BasicBullet;
	class BasicItems;
	class BasicPersonages;

	//-------------------------------------------------------------------------//
	
	class DLL_API BasicEntity
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		BasicEntity( System& System, Physic& Physic );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		virtual ~BasicEntity();

		///////////////////////
		/// ОБНОВЛЕНИЕ ЭНТИТИ
		///////////////////////
		virtual void Update( vector<BasicBullet*>& vBullet, vector<BasicItems*>& vItems, vector<BasicPersonages*>& vPersonages ) = 0;

		/////////////////
		/// УБИТЬ ЭНТИТИ
		/////////////////
		void Kill();

		////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК
		////////////////////////////
		FloatRect GetRect();

		///////////////////////////////
		/// ПОЛУЧИТЬ ТЕКУЩЕЕ ЗДОРОВЬЕ
		//////////////////////////////
		int& GetTempHealtch();

		//////////////////
		/// ПОЛУЧИТЬ ТИК
		//////////////////
		bool& GetTick();

		////////////////////////////
		/// ЖИВОЙ ПЕРСОНАЖ ИЛИ НЕТ
		///////////////////////////
		bool GetLife();

		/////////////////////////
		/// ПОЛУЧИТЬ ИМЯ ЭНТИТИ
		////////////////////////
		string GetNameEntity();

		///////////////////////////////
		/// ПОЛУЧИТЬ МЕНЕДЖЕР АНИМАЦИЙ
		///////////////////////////////
		AnimationManager2D& GetAnimationManager2D();

		///////////////////
		/// ПОЛУЧИТЬ ТЕЛО
		//////////////////
		Body& GetBody();

		/////////////////////
		/// ПОЛУЧИТЬ ФИЗИКУ
		////////////////////
		Physic& GetPhysic();

		//////////////////////////////////////////////////////////////////////////////////////////
		/// СОХРАНИТЬ ПЕРЕМЕННЫЕ КЛАССА [ДЛЯ ТОГО ЧТОБЫ МЕТОТ РАБОТАЛ ЕГО НУЖНО ПЕРЕОПРЕДЕЛИТЬ]
		//////////////////////////////////////////////////////////////////////////////////////////
		virtual void Save();

		//////////////////////////////////////////////////////////////////////////////////////////
		/// ЗАГРУЗИТЬ ПЕРЕМЕННЫЕ КЛАССА [ДЛЯ ТОГО ЧТОБЫ МЕТОТ РАБОТАЛ ЕГО НУЖНО ПЕРЕОПРЕДЕЛИТЬ]
		//////////////////////////////////////////////////////////////////////////////////////////
		virtual void Load();
	protected:
		//////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ЭНТИТИ
		//////////////////////////
		void InitEntity( float fX, float fY, int MaxHealtch, string sName );

		//////////////////////
		/// ОТРИСОВКА ЭНТИТИ
		//////////////////////
		void Render();

		Texture                 Texture;
		Sprite                  Sprite;
		Body*                   EntityBody;
		AnimationManager2D*     AnimationManager2D;
		Physic*                 Physic;
		System*                 System;

		int                     iTempHealtch;
		int                     iMaxHealtch;

		bool                    bTick;
		bool                    bLife;
		bool                    bOnGround;

		string                  sNameEntity;
		FloatRect               EntityRect;
		RectangleShape			BoxDebug;
		RenderWindow*           RenderWindow;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BASICENTITY_H
