#include "..\..\3D\Body3D.h"

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius )
{
	this->PrimitiveType = PrimitiveType;
	this->Radius = Radius;
}

//-------------------------------------------------------------------------//

le::Body3D_ShapeType::Body3D_ShapeType( PrimitivesType PrimitiveType, Vector3f Size )
{
	this->PrimitiveType = PrimitiveType;
	this->Size = Size;
}

//-------------------------------------------------------------------------//

le::Body3D_ConstructionInfo::Body3D_ConstructionInfo( float Mass, Vector3f Position, Vector3f Rotation, Vector3f Inertia )
{
	fMass = Mass;
	this->Position.setValue( Position.x, Position.y, Position.z );
	this->Rotation.setValue( Rotation.x, Rotation.y, Rotation.z, 1 );
	this->Inertia.setValue( Inertia.x, Inertia.y, Inertia.z );
}

//-------------------------------------------------------------------------//

le::Body3D::Body3D( le::Physic3D& Physic, Body3D_ConstructionInfo* ConstructionInfo, Body3D_ShapeType* ShapeType )
{
	Physic3D = &Physic;
	MotionState = new btDefaultMotionState( btTransform( ConstructionInfo->Rotation, ConstructionInfo->Position ) );

	switch ( ShapeType->PrimitiveType )
	{
	case Body3D_ShapeType::Sphere:
		Shape = new btSphereShape( ShapeType->Radius );
		break;

	case Body3D_ShapeType::Plane:
		Vector3f Size = ShapeType->Size;
		Shape = new btStaticPlaneShape( btVector3( Size.x, Size.y, Size.z ), 1 );
		break;
	}

	Shape->calculateLocalInertia( ConstructionInfo->fMass, ConstructionInfo->Inertia );

	btRigidBody::btRigidBodyConstructionInfo BulletConstructionInfo( ConstructionInfo->fMass, MotionState, Shape, ConstructionInfo->Inertia );

	Body = new btRigidBody( BulletConstructionInfo );

	Physic3D->AddBody( this );
}

//-------------------------------------------------------------------------//

void le::Body3D::SetPosition( Vector3f Position )
{
	Body->getMotionState( )->getWorldTransform( Transform );
	Transform.setOrigin( btVector3( Position.x, Position.y, Position.z ) );
	Body->getMotionState( )->setWorldTransform( Transform );
}

//-------------------------------------------------------------------------//

void le::Body3D::SetRotation( Vector3f Rotation )
{
	Body->getMotionState( )->getWorldTransform( Transform );
	Transform.setRotation( btQuaternion( Rotation.x, Rotation.y, Rotation.z, 1 ) );
	Body->getMotionState( )->setWorldTransform( Transform );
}

//-------------------------------------------------------------------------//

Vector3f le::Body3D::GetPosition( )
{
	Body->getMotionState( )->getWorldTransform( Transform );
	btVector3 Position = Transform.getOrigin( );

	return Vector3f( Position.getX( ), Position.getY( ), Position.getZ( ) );
}

//-------------------------------------------------------------------------//

Vector3f le::Body3D::GetRotation( )
{
	Body->getMotionState( )->getWorldTransform( Transform );
	btQuaternion Rotate = Transform.getRotation( );

	return Vector3f( Rotate.getX( ), Rotate.getY( ), Rotate.getZ( ) );
}

//-------------------------------------------------------------------------//

btRigidBody* le::Body3D::GetBulletBody( )
{
	return Body;
}

//-------------------------------------------------------------------------//

le::Body3D::~Body3D( )
{
	delete Body;
}

//-------------------------------------------------------------------------//