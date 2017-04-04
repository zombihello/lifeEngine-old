#include "Matrix.h"

//-------------------------------------------------------------------------//

le::Matrix::Matrix()
{
	for ( int i = 0; i < 16; i++ )
		matrix[ i ] = 0;
}

//-------------------------------------------------------------------------//

void le::Matrix::MatrixMove( Vector3f FactorMovie , float* Matrix )
{
	Matrix[ 1 ] = Matrix[ 2 ] = Matrix[ 4 ] = Matrix[ 6 ] = Matrix[ 8 ] = Matrix[ 9 ] = Matrix[ 12 ] = Matrix[ 13 ] = Matrix[ 14 ] = 0;
	Matrix[ 0 ] = Matrix[ 5 ] = Matrix[ 10 ] = Matrix[ 15 ] = 1;
	Matrix[ 3 ] += FactorMovie.x;
	Matrix[ 11 ] += -FactorMovie.y;
	Matrix[ 7 ] += FactorMovie.z;
}

//-------------------------------------------------------------------------//

void le::Matrix::MatrixAddition( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix )
{
	for ( int i = 0; i < 16; i++ )
		Matrix[ i ] = Matrix1[ i ] + Matrix2[ i ];
}

//-------------------------------------------------------------------------//

void le::Matrix::MatrixAddition( float value, Matrixf Matrix1, float* Matrix )
{
	for ( int i = 0; i < 16; i++ )
		Matrix[i] = Matrix1[i] + value;
}

//-------------------------------------------------------------------------//

void le::Matrix::MatrixMultiply( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix )
{
	Matrixf matrixTmp;

	for ( int x = 0; x < 4; x++ )
	for ( int y = 0; y < 4; y++ )
	{
		matrixTmp[ x + y * 4 ] = 0.0f;
		for ( int z = 0; z < 4; z++ )
			matrixTmp[ x + y * 4 ] += Matrix1[ x + z * 4 ] * Matrix2[ z + y * 4 ];
	}

	for ( int i = 0; i < 16; i++ )
		Matrix[ i ] = matrixTmp[ i ];
}

//-------------------------------------------------------------------------//

void le::Matrix::MatrixMultiply( float value , Matrixf Matrix1 , float* Matrix )
{
	for ( int i = 0; i < 16; i++ )
		Matrix[ i ] = Matrix1[ i ] * value;
}

//-------------------------------------------------------------------------//

Vector3f le::Matrix::MatrixToXYZ( Matrixf Matrix )
{
	return Vector3f( Matrix[ 3 ] , -Matrix[ 11 ] , Matrix[ 7 ] );
}

//-------------------------------------------------------------------------//