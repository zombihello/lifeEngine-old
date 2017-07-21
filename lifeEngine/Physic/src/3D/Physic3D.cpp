#include "..\..\3D\Physic3D.h"
#include "..\..\3D\Body3D.h"

le::Physic3D::Physic3D( Vector3f Gravity )
{
	Broadphase = new btDbvtBroadphase();
	CollisionConfiguration = new btDefaultCollisionConfiguration();
	Dispatcher = new btCollisionDispatcher( CollisionConfiguration );
	Solver = new btSequentialImpulseConstraintSolver();

	World = new btDiscreteDynamicsWorld( Dispatcher, Broadphase, Solver, CollisionConfiguration );
	World->setGravity( btVector3( Gravity.x, Gravity.y, Gravity.z ) );
	World->setDebugDrawer( &DebugDrawer );
}

//-------------------------------------------------------------------------//

void le::Physic3D::UpdatePhysic( btScalar TimeStep, int MaxSubStep )
{
	World->stepSimulation( TimeStep, MaxSubStep );
}

//-------------------------------------------------------------------------//

void le::Physic3D::ShowDebug( Camera* PlayerCamera )
{
	glPushMatrix();

	if ( PlayerCamera != NULL )
		glLoadMatrixf( glm::value_ptr( PlayerCamera->GetViewMatrix() ) );
	else
		glLoadMatrixf( glm::value_ptr( glm::lookAt(
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 ) )
		) );

	if ( DebugDrawer.getDebugMode() != btIDebugDraw::DBG_NoDebug )
		World->debugDrawWorld();

	glPopMatrix();
}

//-------------------------------------------------------------------------//

void le::Physic3D::SetGravity( Vector3f Gravity )
{
	World->setGravity( btVector3( Gravity.x, Gravity.y, Gravity.z ) );
}

//-------------------------------------------------------------------------//

void le::Physic3D::AddBody( btRigidBody* Body )
{
	World->addRigidBody( Body );
	vBodys.push_back( Body );
}

//-------------------------------------------------------------------------//

void le::Physic3D::DestroyBody( btRigidBody* Body )
{
	if ( Body != NULL )
	{
		vBodys.erase( remove( vBodys.begin(), vBodys.end(), Body ), vBodys.end() );
		World->removeRigidBody( Body );

		delete Body;
		Body = NULL;
	}
}

//-------------------------------------------------------------------------//

void le::Physic3D::DestroyAllBodys()
{
	for ( int i = 0; i < vBodys.size(); i++ )
	{
		World->removeRigidBody( vBodys[i] );

		delete vBodys[i];
		vBodys[i] = NULL;
	}

	vBodys.clear();
}

//-------------------------------------------------------------------------//

btDiscreteDynamicsWorld* le::Physic3D::GetWorld()
{
	return World;
}

//-------------------------------------------------------------------------//

vector<btRigidBody*>* le::Physic3D::GetAllBodys()
{
	return &vBodys;
}

//-------------------------------------------------------------------------//

void le::Physic3D::EnableDebug( bool Enable )
{
	if ( Enable )
		DebugDrawer.setDebugMode( btIDebugDraw::DBG_DrawWireframe );
	else
		DebugDrawer.setDebugMode( btIDebugDraw::DBG_NoDebug );
}

//-------------------------------------------------------------------------//

le::Physic3D::~Physic3D()
{
	DestroyAllBodys();

	delete World;
	delete Dispatcher;
	delete Broadphase;
	delete Solver;
	delete CollisionConfiguration;
}

//-------------------------------------------------------------------------//