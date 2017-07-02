#include "..\..\3D\Physic3D.h"
#include "..\..\3D\Body3D.h"

//-------------------------------------------------------------------------//

le::Physic3D::Physic3D( Vector3f Gravity )
{
	Broadphase = new btDbvtBroadphase();
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher( CollisionConfiguration );
	Solver = new btSequentialImpulseConstraintSolver();

	World = new btDiscreteDynamicsWorld( Dispatcher, Broadphase, Solver, CollisionConfiguration );
	World->setGravity( btVector3( Gravity.x, Gravity.y, Gravity.z ) );
}

//-------------------------------------------------------------------------//

void le::Physic3D::UpdatePhysic( btScalar TimeStep, int MaxSubStep )
{
	World->stepSimulation( TimeStep, MaxSubStep );
}

//-------------------------------------------------------------------------//

void le::Physic3D::SetGravity( Vector3f Gravity )
{
	World->setGravity( btVector3( Gravity.x, Gravity.y, Gravity.z ) );
}

//-------------------------------------------------------------------------//

void le::Physic3D::AddBody( le::Body3D* Body )
{
	World->addRigidBody( Body->GetBulletBody() );
	vBodys.push_back( Body );
}

//-------------------------------------------------------------------------//

void le::Physic3D::DestroyBody( Body3D* Body )
{
	for ( int i = 0; i < vBodys.size( ); i++ )
	if ( vBodys[i] == Body )
	{
		World->removeRigidBody( Body->GetBulletBody() );
		vBodys.erase( vBodys.end() + i );
		delete Body;
		Body = NULL;
		return;
	}
}

//-------------------------------------------------------------------------//

void le::Physic3D::DestroyAllBodys()
{
	for ( int i = 0; i < vBodys.size(); i++ )
	{
		World->removeRigidBody( vBodys[i]->GetBulletBody() );
		delete vBodys[i];
	}

	vBodys.clear();
}

//-------------------------------------------------------------------------//

btDiscreteDynamicsWorld* le::Physic3D::GetWorld()
{
	return World;
}

//-------------------------------------------------------------------------//

vector<le::Body3D*>* le::Physic3D::GetAllBodys()
{
	return &vBodys;
}

//-------------------------------------------------------------------------//

le::Physic3D::~Physic3D()
{
	delete World;
	delete Dispatcher;
	delete Broadphase;
	delete Solver;
	delete CollisionConfiguration;
}

//-------------------------------------------------------------------------//