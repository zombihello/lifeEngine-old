#ifndef BODY_3D
#define BODY_3D

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>
#include "Physic3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Body3D_ShapeType
	{
		enum PrimitivesType
		{
			Plane,
			Sphere
		};

		////////////////
		/// КОНСТРУКТОР
		////////////////
		Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius );
		Body3D_ShapeType( PrimitivesType PrimitiveType, Vector3f Size );

		float			Radius;

		PrimitivesType  PrimitiveType;
		Vector3f		Size;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API Body3D_ConstructionInfo
	{
		////////////////
		/// КОНСТРУКТОР
		////////////////
		Body3D_ConstructionInfo( float Mass, Vector3f Position, Vector3f Rotation, Vector3f Inertia = Vector3f( 0, 0, 0 ) );

		float			fMass;

		btVector3		Position;
		btVector3		Inertia;
		btQuaternion	Rotation;		
	};

	//-------------------------------------------------------------------------//

	class DLL_API Body3D
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, Body3D_ShapeType* ShapeType );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Body3D();

		//////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ
		//////////////////////
		void SetPosition( Vector3f Position );

		//////////////////////////
		/// ЗАДАТЬ УГОЛ ПОВОРОТА
		//////////////////////////
		void SetRotation( Vector3f Rotation );

		//////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ
		//////////////////////
		Vector3f GetPosition();

		////////////////////////////
		/// ПОЛУЧИТЬ УГОЛ ПОВОРОТА
		////////////////////////////
		Vector3f GetRotation();

		////////////////////
		/// ПОЛУЧИТЬ ТЕЛО 
		////////////////////
		btRigidBody* GetBulletBody();

	private:
	
		btTransform					Transform;
		btDefaultMotionState*		MotionState;
		btCollisionShape*			Shape;
		btRigidBody*				Body;

		Physic3D*					Physic3D;
	};

	//-------------------------------------------------------------------------//
}

#endif // BODY_3D