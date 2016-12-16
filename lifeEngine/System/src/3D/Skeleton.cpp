#include "..\..\3D\Skeleton.h"

//-------------------------------------------------------------------------//

le::Skeleton::Skeleton()
{
}

//-------------------------------------------------------------------------//

le::Skeleton::~Skeleton()
{
}

//-------------------------------------------------------------------------//

void le::Skeleton::AddBone( Bone bone )
{
	vBone.push_back( bone );
}

//-------------------------------------------------------------------------//

void le::Skeleton::SetBindShape( Matrixf bindShape )
{
	for ( int i = 0; i < 16; i++ )
		BindShape[ i ] = bindShape[ i ];
}

//-------------------------------------------------------------------------//

float* le::Skeleton::GetBindShape()
{
	return BindShape;
}

//-------------------------------------------------------------------------//

vector<le::Bone> le::Skeleton::GetAllBones()
{
	return vBone;
}

//-------------------------------------------------------------------------//


void le::Skeleton::DrawSkeleton()
{
	cout << vBone.size() << endl;;
	/*for ( int i = 0 , count = 1; i < 16; i++ , count++ )
	{
		cout << vBone[ 0 ].StartMatrix[ i ] << " ";
		if ( count == 4 )
		{
			cout << endl;
			count = 0;
		}
	}*/
}