#ifndef BODY_3D
#define BODY_3D

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>
#include <Physic/3D/Body3D_ConstructionInfo.h>
#include <Physic/3D/Body3D_ShapeType.h>
#include <Physic/3D/Physic3D.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Body3D
	{
	public:

		//-------------------------------------------------------------------------//

		enum TypeMove
		{
			Force,
			Impulse
		};

		//-------------------------------------------------------------------------//

		////////////////
		/// КОНСТРУКТОР
		////////////////
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_StaticPlane ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Box ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Capsule ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Cone ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Cylinder ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Mesh ShapeType );
		Body3D( Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, ShapeType_Sphere ShapeType );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Body3D();

		/////////////////
		/// ДВИГАТЬ ТЕЛО
		/////////////////
		void Move( TypeMove typeMove, Vector3f Factor, Vector3f CenterBody = Vector3f() );

		//////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ
		//////////////////////
		void SetPosition( Vector3f Position );

		//////////////////////////
		/// ЗАДАТЬ УГОЛ ПОВОРОТА
		//////////////////////////
		void SetRotation( Vector3f Rotation );

		///////////////////////////////////////////////////////
		/// ЗАДАТЬ ЗАМОРОЖЕННИЕ ПОВОРОТА ПО ОПРЕДЕЛЕННЫМ ОСЯМ
		///////////////////////////////////////////////////////
		void SetFreezeRotation( bool FreezeX, bool FreezeY, bool FreezeZ );

		//////////////////////////////////////////////////////////
		/// ЗАДАТЬ ЗАМОРОЖЕННИЕ ПЕРЕМЕЩЕНИЯ ПО ОПРЕДЕЛЕННЫМ ОСЯМ
		//////////////////////////////////////////////////////////
		void SetFreezeMoving( bool FreezeX, bool FreezeY, bool FreezeZ );

		//////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ
		//////////////////////
		Vector3f GetPosition();

		/////////////////////////
		/// ПОЛУЧИТЬ ПОВОРОТ
		/////////////////////////
		glm::quat GetRotation();

		////////////////////
		/// ПОЛУЧИТЬ ТЕЛО 
		////////////////////
		btRigidBody* GetBulletBody();

		//////////////////////////////////////////
		/// ПОЛУЧИТЬ МАССИВ ВЕРШИН МЕША КОЛЛИЗИИ 
		//////////////////////////////////////////
		btTriangleIndexVertexArray* GetIndexVertexArrays();

	private:
	
		btTransform							Transform;
		btDefaultMotionState*				MotionState;
		btCollisionShape*					Shape;
		btRigidBody*						Body;
		btTriangleIndexVertexArray*			IndexVertexArrays;

		vector<float>*					    vCollisionVertexs;
		vector<int>*						vCollisionIdVertexs;

		Physic3D*							Physic3D;
	};

	//-------------------------------------------------------------------------//
}

#endif // BODY_3D