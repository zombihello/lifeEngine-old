#ifndef MATRIX_H
#define MATRIX_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////
/// SFML
//////////////
#include <SFML/System.hpp>
using namespace sf;

namespace le
{
	//-------------------------------------------------------------------------//

	typedef float Matrixf[16];

	//-------------------------------------------------------------------------//

	class DLL_API Matrix
	{
	public:
		//////////////////////////
		/// КОНСТРУКТОР
		//////////////////////////
		Matrix();

		//////////////////////////
		/// ПЕРЕМЕСТИТЬ МАТРИЦУ
		//////////////////////////
		static void MatrixMove( Vector3f FactorMovie , float* Matrix );

		//////////////////////////
		/// СЛОЖИТЬ МАТРИЦЫ
		//////////////////////////
		static void MatrixAddition( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix );
		static void MatrixAddition( float value, Matrixf Matrix1, float* Matrix );

		//////////////////////////
		/// УМНОЖИТЬ МАТРИЦЫ
		//////////////////////////
		static void MatrixMultiply( Matrixf Matrix1 , Matrixf Matrix2 , float* Matrix );
		static void MatrixMultiply( float value , Matrixf Matrix1 , float* Matrix );

		//////////////////////////////
		/// ПЕРЕВЕСТИ МАТРИЦУ В XYZ
		//////////////////////////////
		static Vector3f MatrixToXYZ( Matrixf Matrix );

		Matrixf matrix;
	};
}

#endif // MATRIX_H