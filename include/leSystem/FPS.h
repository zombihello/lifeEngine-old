#ifndef FPS_H
#define FPS_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////
/// SFML
//////////////
#include <SFML/Graphics.hpp>
using namespace sf;

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API FPS
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		FPS();

		//////////////////////////////
		/// ОБНОВЛЕНИЕ СЧЕТЧИКА FPS
		/////////////////////////////
		void UpdateFPS();

		//////////////////////////////
		/// ПОЛУЧИТЬ FPS
		/////////////////////////////
		unsigned int GetFPS() const;
	private:
		unsigned int		iFrame;
		unsigned int		iFps;
		Clock				Clock;
	};

	//-------------------------------------------------------------------------//
}

#endif // FPS_H
