#ifndef BODY3D_SHAPE_TYPE
#define BODY3D_SHAPE_TYPE

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

	struct DLL_API Body3D_ShapeType
	{
		enum PrimitivesType
		{
			StaticPlane,
			Sphere,
			Box,
			Cylinder,
			Capsule,
			Cone,
			Mesh
		};

		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius );
		Body3D_ShapeType( PrimitivesType PrimitiveType, float Radius, float Height );
		Body3D_ShapeType( PrimitivesType PrimitiveType, Vector3f Size );
		Body3D_ShapeType( PrimitivesType PrimitiveType, vector<float> CollisionVertexs, vector<int> CollisionIdVertexs );

		float				Radius;
		float				Height;

		PrimitivesType		PrimitiveType;
		Vector3f			Size;

		vector<float>		vCollisionVertexs;
		vector<int>			vCollisionIdVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // BODY3D_SHAPE_TYPE