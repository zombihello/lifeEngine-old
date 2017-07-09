#ifndef BODY3D_SHAPE_TYPE
#define BODY3D_SHAPE_TYPE

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/System.h>

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_StaticPlane
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_StaticPlane( Vector3f PlaneNormal, float PlaneConstant );

		float				fPlaneConstant;

		Vector3f			PlaneNormal;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Sphere
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Sphere( float Radius );

		float				fRadius;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Box
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Box( Vector3f Size );

		Vector3f			Size;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Cylinder
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Cylinder( Vector3f Size );

		Vector3f			Size;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Capsule
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Capsule( float Radius, float Height );

		float				fRadius;
		float				fHeight;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Cone
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Cone( float Radius, float Height );

		float				fRadius;
		float				fHeight;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API ShapeType_Mesh
	{
		////////////////
		///  ŒÕ—“–” “Œ–
		////////////////
		ShapeType_Mesh( vector<float>* CollisionVertexs, vector<int>* CollisionIdVertexs );

		vector<float>*			vVertexs;
		vector<int>*			vIdVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // BODY3D_SHAPE_TYPE