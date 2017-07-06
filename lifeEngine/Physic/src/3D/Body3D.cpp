#include "..\..\3D\Body3D.h"
#include <cmath>
//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, Body3D_ShapeType* ShapeType )
{
	Vector3f Size = ShapeType->Size;
	Physic3D = &Physic;
	IndexVertexArrays = NULL;
	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );

	switch ( ShapeType->PrimitiveType )
	{
	case Body3D_ShapeType::StaticPlane:
		Shape = new btStaticPlaneShape( btVector3( 0, 1, 0 ), 1 );
		break;

	case Body3D_ShapeType::Sphere:
		Shape = new btSphereShape( ShapeType->Radius );
		break;

	case Body3D_ShapeType::Box:
		Shape = new btBoxShape( btVector3( Size.x / 2, Size.y / 2, Size.z / 2 ) );
		break;

	case Body3D_ShapeType::Cylinder:
		Shape = new btCylinderShape( btVector3( Size.x / 2, Size.y / 2, Size.z / 2 ) );
		break;

	case Body3D_ShapeType::Capsule:
		Shape = new btCapsuleShape( ShapeType->Radius, ShapeType->Height );
		break;

	case Body3D_ShapeType::Cone:
		Shape = new btConeShape( ShapeType->Radius, ShapeType->Height );
		break;

	case Body3D_ShapeType::Mesh:
		vCollisionIdVertexs = ShapeType->vCollisionIdVertexs;
		vCollisionVertexs = ShapeType->vCollisionVertexs;

		IndexVertexArrays = new btTriangleIndexVertexArray
			(
			( int ) ( vCollisionIdVertexs.size( ) / 3 ),
			&vCollisionIdVertexs[0],
			sizeof( int ) * 3,
			( int ) ( vCollisionVertexs.size( ) / 3 ),
			&vCollisionVertexs[0],
			sizeof( float ) * 3
			);

		Shape = new btConvexTriangleMeshShape( IndexVertexArrays );
		break;
	}

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