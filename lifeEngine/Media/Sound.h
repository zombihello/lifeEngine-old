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
#include "../System/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class BasicEntity;

	//-------------------------------------------------------------------------//

	class DLL_API Sound
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Sound( string Route , string NameSound );
		Sound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation );
		Sound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation );
		Sound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation , int CoefficientVolume );
		Sound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation , int CoefficientVolume );
		Sound( string Route , string NameSound , int CoefficientVolume );

		////////////////////////////
		/// ОБНОВИТЬ ПОЗИЦИЮ ЗВУКА
		////////////////////////////
		void UpdatePositionSound( BasicEntity& Entity );
		void UpdatePositionSound( Vector2f Position );
		void UpdatePositionSound();

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

		BasicEntity*		Entity;

		string				sNameSound;
		SoundBuffer			SoundBuffer;
		sf::Sound			sound;
	};

	//-------------------------------------------------------------------------//
}

#endif // SOUND_H
