#ifndef QUATERNION_H
#define QUATERNION_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////
/// SFML
//////////////
#include <SFML/System.hpp>
using namespace sf;

#include "Matrix.h"

//-------------------------------------------------------------------------//

struct Quat
{
	float angle;
	Vector3f Position;
};

//-------------------------------------------------------------------------//

Quat DLL_API QuatCreate( Vector3f VectorRotate, float angle )
{
	Quat quat;

	quat.angle = cos( angle / 2 );
	quat.Position.x = sin( angle / 2 )*VectorRotate.x;
	quat.Position.y = sin( angle / 2 )*VectorRotate.y;
	quat.Position.z = sin( angle / 2 )*VectorRotate.z;

	return quat;
}

//-------------------------------------------------------------------------//

void DLL_API QuatToMatrix( Quat quat, float* MatrixRotate )
{
	float xx = quat.Position.x * quat.Position.x;
	float xy = quat.Position.x * quat.Position.y;
	float xz = quat.Position.x* quat.Position.z;
	float xw = quat.Position.x * quat.angle;

	float yy = quat.Position.y * quat.Position.y;
	float yz = quat.Position.y * quat.Position.z;
	float yw = quat.Position.y * quat.angle;
	float zz = quat.Position.z * quat.Position.z;
	float zw = quat.Position.z * quat.angle;

	MatrixRotate[ 0 ] = 1 - ( 2 * ( yy + zz ) );
	MatrixRotate[ 1 ] = ( 2 * ( xy - zw ) );
	MatrixRotate[ 2 ] = ( 2 * ( xz + yw ) );

	MatrixRotate[ 4 ] = ( 2 * ( xy + zw ) );
	MatrixRotate[ 5 ] = 1 - ( 2 * ( xx + zz ) );
	MatrixRotate[ 6 ] = ( 2 * ( yz - xw ) );

	MatrixRotate[ 8 ] = ( 2 * ( xz - yw ) );
	MatrixRotate[ 9 ] = ( 2 * ( yz + xw ) );
	MatrixRotate[ 10 ] = 1 - ( 2 * ( xx + yy ) );

	//MatrixRotate[ 3 ] = 0;
	//MatrixRotate[ 7 ] = 0;
	//MatrixRotate[ 11 ] = 0;
	MatrixRotate[ 12 ] = 0;
	MatrixRotate[ 13 ] = 0;
	MatrixRotate[ 14 ] = 0;
	MatrixRotate[ 15 ] = 1;
}

//-------------------------------------------------------------------------//

#endif // QUATERNION_H