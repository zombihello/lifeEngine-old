#ifndef PHYSIC_3D
#define PHYSIC_3D

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

///////////////////////////
/// BULLET PHYSICS
///////////////////////////
#include <btBulletDynamicsCommon.h>

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>


namespace le
{
	//-------------------------------------------------------------------------//

	class Body3D;

	//-------------------------------------------------------------------------//

	class DLL_API Physic3D
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		Physic3D( Vector3f Gravity );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Physic3D();

		///////////////////////
		/// ОБНОВЛЕНИЕ ФИЗИКИ
		//////////////////////
		void UpdatePhysic( btScalar TimeStep, int MaxSubStep = 1 );

		//////////////////////////
		/// ДОБАВИТЬ ТЕЛО В МИР
		//////////////////////////
		void AddBody( Body3D* Body );

		/////////////////////////////
		/// УНИЧТОЖИТЬ ТЕЛО ИЗ МИРА
		/////////////////////////////
		void DestroyBody( Body3D* Body );

		/////////////////////////////////
		/// УНИЧТОЖИТЬ ВСЕ ТЕЛА ИЗ МИРА
		/////////////////////////////////
		void DestroyAllBodys();

		///////////////////////
		/// ЗАДАТЬ ГРАВИТАЦИЮ
		//////////////////////
		void SetGravity( Vector3f Gravity );

		///////////////////////////
		/// ПОЛУЧИТЬ МИР BULLET
		///////////////////////////
		btDiscreteDynamicsWorld* GetWorld();

		///////////////////////////////
		/// ПОЛУЧИТЬ ВСЕ ТЕЛА В МИРЕ
		///////////////////////////////
		vector<Body3D*>* GetAllBodys();

	private:

		btDiscreteDynamicsWorld*						World;
		btCollisionDispatcher*							Dispatcher;
		btBroadphaseInterface*							Broadphase;
		btSequentialImpulseConstraintSolver*			Solver;
		btDefaultCollisionConfiguration*				CollisionConfiguration;

		vector<Body3D*>									vBodys;
	};

	//-------------------------------------------------------------------------//
}

#endif // PHYSIC_3D