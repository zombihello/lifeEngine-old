#include "..\..\3D\Body3D.h"
#include <cmath>
//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_StaticPlane* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btStaticPlaneShape( btVector3( ShapeType->PlaneNormal.x, ShapeType->PlaneNormal.y, ShapeType->PlaneNormal.z ), ShapeType->fPlaneConstant );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Box* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btBoxShape( btVector3( ShapeType->Size.x / 2, ShapeType->Size.y / 2, ShapeType->Size.z / 2 ) );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Capsule* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btCapsuleShape( ShapeType->fRadius, ShapeType->fHeight );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Cone* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btConeShape( ShapeType->fRadius, ShapeType->fHeight );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Cylinder* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btCylinderShape( btVector3( ShapeType->Size.x / 2, ShapeType->Size.y / 2, ShapeType->Size.z / 2 ) );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Mesh* ShapeType )
{
	Physic3D = &Physic;
	vCollisionIdVertexs = ShapeType->vIdVertexs;
	vCollisionVertexs = ShapeType->vVertexs;

	IndexVertexArrays = new btTriangleIndexVertexArray
		(
		( int ) ( vCollisionIdVertexs->size( ) / 3 ),
		&(*vCollisionIdVertexs)[0],
		sizeof( int ) * 3,
		( int ) ( vCollisionVertexs->size( ) / 3 ),
		&(*vCollisionVertexs)[0],
		sizeof( float ) * 3
		);

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btConvexTriangleMeshShape( IndexVertexArrays );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, le::ShapeType_Sphere* ShapeType )
{
	Physic3D = &Physic;
	IndexVertexArrays = NULL;

	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );
	Shape = new btSphereShape( ShapeType->fRadius );

	if ( !ConstructionInfo->Static )
		Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );
	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

void le::Body3D::SetPosition( Vector3f Position )
{
	Body->clearForces();
	Transform.setOrigin( btVector3( Position.x, Position.y, Position.z ) );

	Body->setWorldTransform( Transform );
	MotionState->setWorldTransform( Transform );
	Body->activate( true );
}

//-------------------------------------------------------------------------//

void le::Body3D::SetRotation( Vector3f Rotation )
{
	Body->getMotionState()->getWorldTransform( Transform );
	Transform.setRotation( btQuaternion( Rotation.x, Rotation.y, Rotation.z, 1 ) );
	Body->getMotionState()->setWorldTransform( Transform );
}

//-------------------------------------------------------------------------//

Vector3f le::Body3D::GetPosition() // TODO: Возвращает позицию центра тела
{
	Body->getMotionState()->getWorldTransform( Transform );
	btVector3 Position = Transform.getOrigin();

	return Vector3f( Position.getX(), Position.getY(), Position.getZ() );
}

//-------------------------------------------------------------------------//

Vector3f le::Body3D::GetRotation() // TODO: Проблема с переводом кватерниона в углы Эйлера
{
	Body->getMotionState()->getWorldTransform( Transform );
	btQuaternion q = Transform.getRotation();

	float ysqr = q.y() * q.y();

	// roll (x-axis rotation)
	float t0 = +2.0 * ( q.w() * q.x() + q.y() * q.z() );
	float t1 = +1.0 - 2.0 * ( q.x() * q.x() + ysqr );
	float roll = std::atan2f( t0, t1 );

	// pitch (y-axis rotation)
	float t2 = +2.0 * ( q.w() * q.y() - q.z() * q.x() );
	t2 = ( ( t2 > 1.0 ) ? 1.0 : t2 );
	t2 = ( ( t2 < -1.0 ) ? -1.0 : t2 );
	float pitch = std::asinf( t2 );

	// yaw (z-axis rotation)
	float t3 = +2.0 * ( q.w() * q.z() + q.x() * q.y() );
	float t4 = +1.0 - 2.0 * ( ysqr + q.z() * q.z() );
	float yaw = std::atan2f( t3, t4 );

	return Vector3f( roll, pitch, yaw );
}

//-------------------------------------------------------------------------//

btRigidBody* le::Body3D::GetBulletBody()
{
	return Body;
}

//-------------------------------------------------------------------------//

le::Body3D::~Body3D()
{
	delete MotionState;
	delete Shape;
	delete Body;

	if ( IndexVertexArrays != NULL )
		delete IndexVertexArrays;
}

//-------------------------------------------------------------------------//