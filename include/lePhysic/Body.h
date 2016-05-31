#ifndef BODY_H
#define BODY_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////
/// BOX2D
///////////
#include <Box2D.h>

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/leSystem.h"
#include "CircleShape.h"
#include "PolygonShape.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class Physic;

	//-------------------------------------------------------------------------//
	
	class DLL_API Body
	{
	public:
		enum TYPE_BODY
		{
			STATIC = b2_staticBody,
			DINAMIC = b2_dynamicBody,
			KINEMATIC = b2_kinematicBody
		};

		enum TYPE_MOVE
		{
			FORCE,
			LINEAR_IMPULSE
		};

		////////////////
		/// КОНСТРУКТОР
		////////////////
		Body( Vector2f Position, string NameBody, TYPE_BODY TypeBody = STATIC );
		Body( Vector2f Position, float Angle, string NameBody, TYPE_BODY TypeBody = STATIC );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Body();

		/////////////////////////
		/// ДОБАВИТЬ КРУГ В ТЕЛО
		/////////////////////////
		void CreateCircleShape( float Radius );
		void CreateCircleShape( float Radius, Vector2f Center  );

		/////////////////////////////////////
		/// ДОБАВИТЬ ПОЛИГОНОУЮ ФОРМУ В ТЕЛО
		/////////////////////////////////////
		void CreatePolygonShape( Vector2f SizeBody );
		void CreatePolygonShape( Vector2f SizeBody, Vector2f Center, float Angle = 0 );

		/////////////////
		/// ДВИГАТЬ ТЕЛО
		/////////////////
		void MoveBody( Vector2f Factor, TYPE_MOVE TypeMove );

		////////////////////////
		/// ЗАДАТЬ РАЗМЕР ТЕЛА
		///////////////////////
		void SetSize( Vector2f SizeBody );

		////////////////////////////
		/// ЗАДАТЬ СВОЙСТВА ТЕЛА
		////////////////////////////
		void SetPropirtes( float Density, float Restitution, float Friction );

		////////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ ТЕЛА
		////////////////////////////
		void SetPosition( Vector2f Position );

		////////////////////////////
		/// ЗАДАТЬ ГРУППУ ТЕЛА
		////////////////////////////
		void SetGroupBody( int NumberGroup );

		////////////////////////////////////
		/// ЗАДАТЬ КАТЕГОРИЮ СТОЛКНОВЕНИЙ
		///////////////////////////////////
		void SetCategorieCollisions( uint16 Category );

		/////////////////////////////////////////
		/// ИГНОРИРОВАТЬ КАТЕГОРИЮ СТОЛКНОВЕНИЙ
		////////////////////////////////////////
		void SetIgnoreCategoryCollisions( uint16 Category );

		//////////////////////////////////////////////
		/// ЗАФИКСИРОВАТЬ ТЕЛО ( ДАБЫ НЕ ВРАЩАЛОСЬ )
		/////////////////////////////////////////////
		void SetFixedRotation( bool FixedRotation );

		/////////////////////////
		/// ЗАДАТЬ СКОРОСТЬ ТЕЛУ
		/////////////////////////
		void SetSpeedBody( Vector2f Speed );

		////////////////////////////////////////
		/// СТОЛКНУЛОСЬ ЛИ ТЕЛО С ДРУГИМ ТЕЛОМ
		///////////////////////////////////////
		bool IsBodyColision( Body* Body );

		//////////////////////
		/// ТЕЛО СТОЛКНУЛОСЬ
		//////////////////////
		bool IsColision();

		//////////////////
		/// ТЕЛО НА ЗЕМЛЕ
		//////////////////
		bool IsOnGround( Vector2f SizeBody );

		////////////////////////////////////
		/// ПОЛУЧИТЬ КАТЕГОРИЮ СТОЛКНОВЕНИЙ
		///////////////////////////////////
		uint16 GetCategorieCollisions();

		////////////////////////
		/// ПОЛУЧИТЬ ИМЯ ТЕЛА
		///////////////////////
		string GetNameBody();

		////////////////////////////
		/// ПОЛУЧИТЬ СКОРОСТЬ ТЕЛА
		///////////////////////////
		Vector2f GetSpeedBody();

		b2Body*                         body;
		b2BodyDef                       bDef;
	private:
		float                           fDensity;
		float                           fRestitution;
		float							fFriction;


		bool							bMoveLeft;

		string                          sNameBody;
		b2Filter*						Filter;
		vector<CircleShape*>            vCircle;
		vector<PolygonShape*>           vShape;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BODY_H
