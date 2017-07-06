#ifndef BODY3D_CONSTRUCTION_INFO
#define BODY3D_CONSTRUCTION_INFO

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>
#include "Physic3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Body3D_ConstructionInfo
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		Body3D_ConstructionInfo( float Mass, Vector3f Position, Vector3f Rotation, bool IsStatic = false, Vector3f Inertia = Vector3f( 0, 0, 0 ) );

		float			fMass;

		bool			Static;

		btVector3		Position;
		btVector3		Inertia;
		btQuaternion	Rotation;
	};

	//-------------------------------------------------------------------------//
}

#endif // BODY3D_CONSTRUCTION_INFO