#include <Graphics\Level\Level.h>
#include <Physics\Physics.h>

//-------------------------------------------------------------------------//

le::Physics::Physics( const glm::vec3& Gravity ) :
	Level( NULL )
{
	//TODO: [zombiHello] Сделать изменение гравитации
}

//-------------------------------------------------------------------------//

le::Physics::~Physics()
{
}

//-------------------------------------------------------------------------//

void le::Physics::Update()
{
	// **************************
	// Проверяем столкновение 
	// с брашами уровня

	if ( Level )
		for ( size_t Index = 0; Index < Bodyes.size(); ++Index )
		{
			Body* Ptr_Body = &Bodyes[ Index ];
			Ptr_Body->SetPosition( Level->Trace( Ptr_Body->GetRay() ) );
		}
}

//-------------------------------------------------------------------------//

void le::Physics::AddLevel( le::Level& Level )
{
	this->Level = &Level;
}

//-------------------------------------------------------------------------//

void le::Physics::RemoveLevel()
{
	Level = NULL;
}

//-------------------------------------------------------------------------//

le::Body* le::Physics::CreateBody()
{
	size_t IndexLastBody = Bodyes.size();
	Bodyes.push_back( Body() );

	return &Bodyes[ IndexLastBody ];
}

//-------------------------------------------------------------------------//

void le::Physics::DeleteBody( Body* Body )
{
	for ( size_t Index = 0; Index < Bodyes.size(); ++Index )
		if ( Bodyes[ Index ] == *Body )
		{
			Bodyes.erase( Bodyes.begin() + Index );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::Physics::DeleteAllBodyes()
{
	Bodyes.clear();
}

//-------------------------------------------------------------------------//

void le::Physics::Clear()
{
	Level = NULL;
	Bodyes.clear();
}

//-------------------------------------------------------------------------//

void le::Physics::SetGravity( const glm::vec3& Gravity )
{
	//TODO: [zombiHello] Сделать изменение гравитации
}

//-------------------------------------------------------------------------//
