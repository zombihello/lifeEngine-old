#include <System\System.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "../Media/materials/scripts", "FileSystem", "General" );
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "../Media/materials/textures", "FileSystem", "General" );
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( "../Media/models", "FileSystem", "General" );

		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps( 5 );
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup( "General" );

		Camera = Scene->createCamera( "PlayerCam" );

		// Position it at 500 in Z direction
		Camera->setPosition( Ogre::Vector3( 0, 100, -400 ) );
		// Look back along -Z
		Camera->lookAt( Ogre::Vector3( 0, 0, 300 ) );
		Camera->setNearClipDistance( 5 );

		Ogre::Viewport* vp = Window->addViewport( Camera );
		vp->setBackgroundColour( Ogre::ColourValue( 0, 0, 0 ) );

		Camera->setAspectRatio(
			Ogre::Real( vp->getActualWidth() ) / Ogre::Real( vp->getActualHeight() ) );

		Ogre::Entity* ogreHead = Scene->createEntity( "Head", "ninja.mesh" );
		Ogre::SceneNode* headNode = Scene->getRootSceneNode()->createChildSceneNode();
		headNode->attachObject( ogreHead );

		Scene->setAmbientLight( Ogre::ColourValue( 0.5, 0.5, 0.5 ) );

		Ogre::Light* l = Scene->createLight( "MainLight" );
		l->setPosition( 20, 80, 500 );
	}

	void Update()
	{
	}

	Ogre::Camera* Camera; 
};

int main()
{
	le::System System( "../Config.cfg", "../Game.log", "../Plugins" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}