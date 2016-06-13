#ifndef MUSIC_H
#define MUSIC_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

///////////
/// SFML
///////////
#include <SFML/Audio.hpp>

/////////////////
/// LIFEENGINE
////////////////
#include "../leSystem/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Music
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Music( string Route, string NameMusic, bool loop = false );
		Music( string Route, string NameMusic, int CoefficientVolume, bool loop = false );

		//////////////////////
		/// ПРОИГРАТЬ МУЗЫКУ
		/////////////////////
		void Play();

		///////////////////////////////
		/// ПОСТАВИТЬ МУЗЫКУ НА ПАУЗУ
		//////////////////////////////
		void Pause();

		////////////////////////
		/// ОСТАНОВИТЬ МУЗЫКУ
		///////////////////////
		void Stop();

		//////////////////////
		/// ИГРАЕТ ЛИ МУЗЫКА
		/////////////////////
		bool IsPlaying();

		////////////////////////
		/// ПОЛУЧИТЬ ИМЯ МУЗЫКИ
		////////////////////////
		string GetNameMusic();

		///////////////////////////////////////////
		/// ПОЛУЧИТЬ КОЭФФИЦИЕНТ ГРОМКОСТИ МУЗЫКИ
		//////////////////////////////////////////
		int GetCoefficientVolume();

		/////////////////////
		/// ПОЛУЧИТЬ МУЗЫКУ
		/////////////////////
		sf::Music& GetMusic();
	private:
		int				iCoefficientVolume;

		string			sNameMusic;
		sf::Music		music;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // MUSIC_H
