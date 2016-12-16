#ifndef MATRIX_H
#define MATRIX_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////
/// SFML
//////////////
#include <SFML/System.hpp>
using namespace sf;

//-------------------------------------------------------------------------//

typedef float Matrixf[ 16 ];

//-------------------------------------------------------------------------//

/*
1	0	0	0
0	1	0	0
0	0	1	0   } матрица движения
x	y	z	1
*/

void DLL_API MatrixMove( Vector3f FactorMovie , float* Matrix );

//-------------------------------------------------------------------------//

void DLL_API MatrixMultiply( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix );

//-------------------------------------------------------------------------//

Vector3f DLL_API MatrixToXYZ( Matrixf Matrix );

void DLL_API MatrixInvert( float* Matrix );

//-------------------------------------------------------------------------//

#endif // MATRIX_H