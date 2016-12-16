#include "Matrix.h"

//-------------------------------------------------------------------------//

void MatrixMove( Vector3f FactorMovie , float* Matrix )
{
	Matrix[ 1 ] = Matrix[ 2 ] = Matrix[ 3 ] = Matrix[ 4 ] = Matrix[ 6 ] = Matrix[ 7 ] = Matrix[ 8 ] = Matrix[ 9 ] = Matrix[ 11 ] = 0;
	Matrix[ 0 ] = Matrix[ 5 ] = Matrix[ 10 ] = Matrix[ 15 ] = 1;
	Matrix[ 3 ] = FactorMovie.x;
	Matrix[ 7 ] = FactorMovie.y;
	Matrix[ 11 ] = FactorMovie.z;
}

//-------------------------------------------------------------------------//

void MatrixMultiply( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix )
{
	for ( int x = 0; x < 4; x++ )
		for ( int y = 0; y < 4; y++ )
		{
			Matrix[ x + y * 4 ] = 0.0f;
			for ( int z = 0; z < 4; z++ )
				Matrix[ x + y * 4 ] += Matrix1[ x + z * 4 ] * Matrix2[ z + y * 4 ];
		}
}

//-------------------------------------------------------------------------//

Vector3f MatrixToXYZ( Matrixf Matrix )
{
	return Vector3f( Matrix[ 3 ] , Matrix[ 7 ] , Matrix[ 11 ] );
}

void MatrixInvert( float* Matrix )
{
	//Matrixf tmp = { *Matrix };
	int tmp = 0;
	for ( int i = 0; i<16 / 2 + 1; i++ )
	{
		tmp = Matrix[ i ];
		Matrix[ i ] = Matrix[ 15 - i ];
		Matrix[ 15 - i ] = tmp;
	}
}

//-------------------------------------------------------------------------//