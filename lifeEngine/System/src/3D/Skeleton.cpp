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

void le::Skeleton::AddBone( Bone* bone )
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

vector<le::Bone*> le::Skeleton::GetAllBones()
{
	return vBone;
}

//-------------------------------------------------------------------------//
le::Bone::Bone(){ Perent = NULL; }
void le::Skeleton::DrawSkeleton( vector<Bone*> bones )
{
	for ( int i = 0; i<bones.size(); i++ )
	{
		Bone* bn = bones[ i ];

		if ( bn->Perent != NULL )
		{
			Vector3f bm1 = MatrixToXYZ( bn->Perent->Realese );
			Vector3f bm2 = MatrixToXYZ( bn->Realese );

			glBegin( GL_LINES );
			glColor3f( 1 , 0 , 0 );
			glVertex3f( bm1.x , bm1.y , bm1.z );
			glVertex3f( bm2.x , bm2.y , bm2.z );
			glColor3f( 1 , 1 , 1 );
			glEnd();
		}

			if ( bn->vChild.size()>0 )
				DrawSkeleton( bn->vChild );
	}
}

void MatrixInit( le::Bone& bone )
{
	if ( bone.Perent != NULL )
	{
		MatrixMultiply( bone.StartMatrix , bone.Perent->Realese , bone.Realese );
		cout << bone.name << endl;
		Vector3f m = MatrixToXYZ( bone.Realese );
	}

	for ( int i = 0; i < bone.vChild.size(); i++ )
	{
		le::Bone* bn = bone.vChild[ i ];
		MatrixInit( *bn );
	}
}


void le::Skeleton::init( )
{
	for ( int i = 0; i < vBone.size(); i++ )
	{
		Bone* bon = vBone[ i ];
		MatrixInit( *bon );
	}
}