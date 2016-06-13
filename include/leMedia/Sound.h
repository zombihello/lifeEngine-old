#ifndef SOUND_H
#define SOUND_H

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
	
	class DLL_API Sound
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Sound( string Route, string NameSound );
		Sound( string Route, string NameSound, int CoefficientVolume );

		//////////////////////
		/// ПРОИГРАТЬ ЗВУК
		/////////////////////
		void Play();

		/////////////////////////////
		/// ПОСТАВИТЬ ЗВУК НА ПАУЗУ
		////////////////////////////
		void Pause();

		////////////////////////
		/// ОСТАНОВИТЬ ЗВУК
		///////////////////////
		void Stop();

		////////////////////////
		/// ПОЛУЧИТЬ ИМЯ ЗВУКА
		////////////////////////
		string GetNameSound();

		///////////////////////////////////////////
		/// ПОЛУЧИТЬ КОЭФФИЦИЕНТ ГРОМКОСТИ ЗВУКА
		//////////////////////////////////////////
		int GetCoefficientVolume();

		//////////////////////////
		/// ПОЛУЧИТЬ БУФЕР ЗВУКА
		//////////////////////////
		sf::SoundBuffer& GetBufferSound();

		/////////////////////
		/// ПОЛУЧИТЬ ЗВУК
		/////////////////////
		sf::Sound& GetSound();

		/////////////////////
		/// ИГРАЕТ ЛИ ЗВУК
		/////////////////////
		bool IsPlaying();
	private:
		int					iCoefficientVolume;

		string				sNameSound;
		SoundBuffer			SoundBuffer;
		sf::Sound			sound;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // SOUND_H
