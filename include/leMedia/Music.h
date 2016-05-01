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

		//////////////////////
		/// ПРОИГРАТЬ МУЗЫКУ
		/////////////////////
		void Play();

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

		/////////////////////
		/// ПОЛУЧИТЬ МУЗЫКУ
		/////////////////////
		sf::Music& GetMusic();
	private:
		bool			bIsPlaying;

		string			sNameMusic;
		sf::Music		music;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // MUSIC_H
