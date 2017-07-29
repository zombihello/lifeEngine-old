#ifndef GBUFFER_H
#define GBUFFER_H

#define COMPILING_LIBRARY
#define GBUFFER_NUM_TEXTURES 3
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API GBuffer
	{
	public:

		//-------------------------------------------------------------------------//

		enum GBuffer_Texture_Type
		{
			Position,
			Textures,
			Normal		
		};

		//-------------------------------------------------------------------------//

		////////////////////////////////////
		/// КОНСТРУКТОР
		///////////////////////////////////
		GBuffer();

		////////////////////////////////////
		/// ДЕСТРУКТОР
		///////////////////////////////////
		~GBuffer();

		////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ G-БУФФЕР
		///////////////////////////////////
		bool InitGBuffer( float WindowWidth, float WindowHeight );

		///////////////////////////////
		/// ПРИВЯЗАТЬ FBO НА ЗАПИСЬ
		//////////////////////////////
		void BindForWriting();

		//////////////////////////////
		/// ПРИВЯЗАТЬ FBO НА ВВОД
		//////////////////////////////
		void BindForReading();
	
private:

		bool				IsGBufferInit;

		GLuint				FBO;
		GLuint				Buffers[ GBUFFER_NUM_TEXTURES ];
		GLuint				DepthTexture;
	};

	//-------------------------------------------------------------------------//
}

#endif // GBUFFER_H