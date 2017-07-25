#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include <System\System.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Light3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Light3D();

		string			NameLight;

		glm::vec3		Position;
		glm::vec4		Ambient;
		glm::vec4		Diffuse;
		glm::vec4		Specular;
		glm::vec3		Attenuation;
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_3D_H