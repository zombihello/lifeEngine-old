#ifndef LIGHT_3D_H
#define LIGHT_3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include <System\System.h>
#include "LightSphere.h"

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
		Light3D( float Radius, glm::vec3 Position, glm::vec4 Ambient, glm::vec4 Diffuse, glm::vec4 Specular, glm::vec3 Attenuation );

		string			NameLight;

		glm::vec3		Position;
		glm::vec4		Ambient;
		glm::vec4		Diffuse;
		glm::vec4		Specular;
		glm::vec3		Attenuation;

		LightSphere     LightSphere;	
	};

	//-------------------------------------------------------------------------//
}

#endif // LIGHT_3D_H