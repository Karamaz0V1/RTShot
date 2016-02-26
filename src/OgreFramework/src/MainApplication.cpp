#include <stdafx.h>

#include <OgreFramework/MainApplication.h>

#include <OGRE/OgreRay.h>
#include <OGRE/OgreSceneQuery.h>
#include <OgreFramework/EntityAdapter.h>
#include <Ogre/DotSceneLoader.h>
#include <OgreFramework/PickingSelectionBuffer.h>
#include <OgreFramework/PickingBoundingBox.h>
#include <OgreFramework/AgentPicking.h>
#include <OgreFramework/GeometryLoader.h>
#include <OgreFramework/GlobalConfiguration.h>
#include <GameElements/ConfigurationLoader.h>
#include <GameElements/NullAgent.h>
#include <GameElements/Bullet.h>
#include <GameElements/StraightBullet.h>
#include <GameElements/BallisticBullet.h>
#include <Ext/Ogre/ComputeBoundingBox.h>
#include <Triggers/BasicCollisionDetector.h>
#include <GameElements/HumanAgent.h>
#include <GameElements/ComputerAgent.h>

namespace OgreFramework
{
	MainApplication::MainApplication()
		: m_keyboardState(*KeyboardState::getInstance())
	{
		startGame = false;
		playPac = true;
		loaded = false;
		isclicked=false;
		
		gameOver=false;
		playerWon=true;
		
	}

	MainApplication::~MainApplication()
	{
	}

	void MainApplication::createScene(void)
	{
		// Application  global configuration
		GlobalConfiguration::setup("../data/xml/hot_config.xml", m_sceneManager, m_trayManager) ;

		// Creates the instance of collision detector
		new Triggers::SweepAndPrune ;
		//new Triggers::BasicCollisionDetector ;
		
		GlobalConfiguration::setCurrentMap("map01") ;

		// create your scene here :)
		::std::cout<<"Creating scene..."<<::std::flush ;
	
		// 0- Sets the camera style
		//m_cameraManager->setStyle(OgreBites::CS_ORBIT) ; //OgreBites::CS_MANUAL) ;

		// 1- Sets the shadows (shadow volumes)
		// ------------------------------------
		m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_NONE) ; // No shadows
		//m_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE) ; ////Ogre::SHADOWTYPE_STENCIL_ADDITIVE) ;//
		//////m_sceneManager->setShadowDirectionalLightExtrusionDistance(10000000) ;
		//m_sceneManager->setShadowIndexBufferSize(500000) ;
		//m_sceneManager->setShadowColour(::Ogre::ColourValue(.3,.3,.3)) ;
		//////m_sceneManager->setShowDebugShadows(true) ;

		m_camera->setFarClipDistance(Ogre::Real(3000)) ;
		m_camera->setNearClipDistance(Ogre::Real(0.01)) ;

		// 2- Sets fog
		// -----------
		m_sceneManager->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(Ogre::Real(0.0), Ogre::Real(0.0), Ogre::Real(0.0)), Ogre::Real(0.03), Ogre::Real(2000.0), Ogre::Real(3000.0)) ;

		// 3- Adds lights
		Ogre::Light * light = m_sceneManager->createLight("light1siga") ;
		light->setType(Ogre::Light::LT_DIRECTIONAL) ;
		Ogre::Vector3 direction(-10.0, -10.0, -1.0) ;
		direction.normalise() ;
		light->setDirection(direction) ;
		light->setDiffuseColour(0.5,0.5,0.5) ;
		light->setSpecularColour(0.5, 0.5, 0.5) ;
		m_sceneManager->setAmbientLight(::Ogre::ColourValue(0.5,0.5,0.5)) ;
		m_sceneManager->getRootSceneNode()->attachObject(light) ;

		
		// Gets the root scene node
		Ogre::SceneNode * rootSceneNode = getRootSceneNode() ;
		
		// --------------------------------------
		// Setups the GUI (it's a test / example)
		// --------------------------------------
		
		{
			Ogre::StringVector tmp ;
			tmp.push_back("5") ;
			tmp.push_back("10") ;
			tmp.push_back("20") ;
			OgreBites::SelectMenu * menu1 = m_trayManager->createThickSelectMenu(OgreBites::TL_TOPLEFT, "Menu", "Number Units", 200, 10, tmp) ;
		}

		{
			OgreBites::Button * but = m_trayManager->createButton(OgreBites::TL_TOPLEFT, "But", "Select", 200) ;
		}

		{
			Ogre::DisplayString cap1 = "Bienvenue dans RTShot\n Selectionnez le nombre d'unite que vous desirez controler dans le menu en haut a droite puis cliquer sur le bouton en dessous pour les charger\n\nespace pour lancer la partie";
			OgreBites::TextBox * text = m_trayManager->createTextBox(OgreBites::TL_CENTER,"txt1","RTShot",300,400);
			text->appendText(cap1);
		}
		// Setups the picking
		//m_picking = new PickingBoundingBox(m_sceneManager, m_camera, OIS::MB_Left) ;
		//m_picking = new PickingSelectionBuffer(m_window, m_sceneManager, m_camera, OIS::MB_Left) ;
		m_picking = new AgentPicking(m_window, m_sceneManager, m_camera, OIS::MB_Left, GameElements::SmithAgent::getMovedEmitter());
		// Setups the camera control system
		m_cameraManager = new RTSCameraManager(m_sceneManager, m_camera, &m_keyboardState) ;

		// ----------------------------------------
		// Creates two entities for testing purpose
		// ----------------------------------------
		
		//m_entityAdapter = new GameElements::NullAgent(configurationLoader.getUnitsArchetypes().get("HippoB"), configurationLoader.getWeaponsArchetypes().get()) ;

		// Test pour vérifier la correspondance carte <-> représentation graphique
		//for(int x=0 ; x<GlobalConfiguration::getCurrentMap()->width() ; x+=10)
		//	for(int y=0 ; y<GlobalConfiguration::getCurrentMap()->height() ; y+=10)
		//	{
		//		if(GlobalConfiguration::getCurrentMap()->getCell(Math::Vector2<int>(x,y)).m_speedReduction!=1.0)
		//		{
		//			unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get("CrocoB") ;
		//			weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;
		//			if(weapon==NULL) { ::std::cout<<"CrocoB: bad weapon!" ; char c ; ::std::cin>>c ; }
		//			GameElements::NullAgent::Pointer ptr = new GameElements::NullAgent(unit, weapon) ; // HippoB, CrocoB, MousticB
		//			ptr->setOrientation(0.0,0.0,Math::piDiv4) ;
		//			ptr->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(Math::Vector2<int>(x,y)).push(0.0)) ;
		//		}
		//	}
	}

	void MainApplication::loadUnits(int a,::std::vector<::std::string> types,int b){

		for(int cpt=0 ; cpt<a ; ++cpt)
		{
			const GameElements::UnitsArchetypes::Archetype * unit = GlobalConfiguration::getConfigurationLoader()->getUnitsArchetypes().get(types[b]) ;
			const GameElements::WeaponsArchetypes::Archetype * weapon = GlobalConfiguration::getConfigurationLoader()->getWeaponsArchetypes().get(unit->m_weapon) ;

			if(weapon==NULL) { ::std::cout<<"HippoB: bad weapon!" ; char c ; ::std::cin>>c ; }			
			//GameElements::RandomAgent * anderson = new GameElements::RandomAgent(unit, weapon);
			GameElements::SmithAgent * anderson;
			if (b < 3) anderson = new GameElements::ComputerAgent(unit, weapon);
			else  anderson = new GameElements::HumanAgent(unit, weapon);

			agentBlackBox.push_back(anderson);
			GameElements::RandomAgent::Pointer m_entityAdapter = anderson ;
			m_entityAdapter->setPosition(GlobalConfiguration::getCurrentMap()->toWorldCoordinates(GlobalConfiguration::getCurrentMap()->findFreeLocation()).push(0.0)) ;
		}
	}

	void MainApplication::update(Ogre::Real dt)
	{
		// Necessary for GUI...

		if(startGame==false){
			::std::vector<::std::string> types ;
			

			if((((OgreBites::Button*)m_trayManager->getWidget(OgreBites::TL_TOPLEFT,"But"))->getState()==OgreBites::BS_DOWN)&&(isclicked==false))
			{
				loaded=false;
				isclicked=true;
			}

			if(((OgreBites::SelectMenu*)m_trayManager->getWidget(OgreBites::TL_TOPLEFT,"Menu"))->isExpanded())
			{
				isclicked=false;
			}

			if(!loaded){
				
				if(agentBlackBox.size()!=0){
					int b=agentBlackBox.size()-1;
					for (int i=b;i>-1;i--){
						GameElements::SmithAgent * a = agentBlackBox[i];
						agentBlackBox.pop_back();
						a->destroy();
					}
					agentBlackBox.clear();
				}
				
				

				types.push_back("MousticB") ;
				types.push_back("CrocoB") ;
				types.push_back("HippoB") ;
				types.push_back("MousticR") ;
				types.push_back("CrocoR") ;
				types.push_back("HippoR") ;
				std::string p = ((OgreBites::SelectMenu*)m_trayManager->getWidget(OgreBites::TL_TOPLEFT,"Menu"))->getSelectedItem();

				switch (std::stoi(p)){
					case 5:
						loadUnits(2,types,0);
						loadUnits(2,types,1);
						loadUnits(1,types,2);
						loadUnits(2,types,3);
						loadUnits(2,types,4);
						loadUnits(1,types,5);

						break;
					case 10:
						
						loadUnits(4,types,0);
						loadUnits(4,types,1);
						loadUnits(2,types,2);
						loadUnits(4,types,3);
						loadUnits(4,types,4);
						loadUnits(2,types,5);
						break;

					case 20:
						
						loadUnits(8,types,0);
						loadUnits(8,types,1);
						loadUnits(4,types,2);
						loadUnits(8,types,3);
						loadUnits(8,types,4);
						loadUnits(4,types,5);
						break;
				}
				
				loaded=true;
			}
		}
		
		//dt = ::std::min(dt,0.01f) ;
		static double time = 0 ;
		time += dt ;

		//if(time>4)
		//{
		//	for(float cpt=-3 ; cpt<3 ; cpt+=0.6)
		//	{
		//		//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), Math::Vector2<Config::Real>(0.5+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), Math::Vector2<Config::Real>(-0.5-+double(rand())/RAND_MAX,0), 12) ;
		//		new GameElements::StraightBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), Math::Vector2<Config::Real>(0.5+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::StraightBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), Math::Vector2<Config::Real>(-0.5-+double(rand())/RAND_MAX,0), 12) ;
		//		//new GameElements::BallisticBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(-6,cpt), 2+double(rand())/RAND_MAX, Math::Vector2<Config::Real>(4-8*double(rand())/RAND_MAX,4-8*double(rand())/RAND_MAX), 2) ;
		//		new GameElements::BallisticBullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 2, 0, Math::Vector2<Config::Real>(6,cpt), 2+double(rand())/RAND_MAX, Math::Vector2<Config::Real>(4-8*double(rand())/RAND_MAX,4-8*double(rand())/RAND_MAX), 2) ;
		//	}
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(-2,1)*3.0, Math::Vector2<Config::Real>(0.5,0), 4*3.0) ;
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(-2,0)*3.0, Math::Vector2<Config::Real>(0.5,0), 4*3.0) ;
		//	//new GameElements::Bullet(GlobalConfiguration::getGeometryLoader()->load("JetEngine.scene"), 20, 0, Math::Vector2<Config::Real>(2,0)*3.0, Math::Vector2<Config::Real>(-0.5,0), 4*3.0) ;
		//	time = 0 ;
		//}

		//m_trayManager->showAll();

		//Ogre::AxisAlignedBox box = Ext::Ogre::computeBoundingBox(m_shipShape) ;
		//::std::cout<<box.getMinimum()<<" / "<<box.getMaximum()<<::std::endl ;

		static Ogre::Real absoluteTime  = 0.0 ;

		absoluteTime += dt ;
		//m_entityAdapter->setPosition(Math::Vector3<Config::Real>((int(absoluteTime*1000)%10000)/2000.0, (int(absoluteTime*1000)%10000)/2000.0, 0.0)) ;
		//m_entityAdapter->setOrientation(0.0,0.0,absoluteTime) ;

		// Updates camera manager
		m_cameraManager->update(dt,!startGame,playPac) ;
		playPac=false;
		// Updates (animation, behavoir & son on) are called here :)
		if(startGame==true){
			GlobalConfiguration::getController()->update(dt) ;
		}

		if(m_keyboardState.isDown(OIS::KC_SPACE)&&startGame==false){
			startGame=true;
			gameOver=false;
			m_trayManager->hideTrays();
		}
		
		//static bool explosionFired = false ;
		//if(absoluteTime>10.0 && !explosionFired)
		//{
		//	explosionFired = true ;
		//	// Trying explosion
		//	// create a particle system named explosions using the explosionTemplate
		//	Ogre::ParticleSystem* particleSystem = m_sceneManager->createParticleSystem("explosions2", "explosionTemplate");
		//	particleSystem->setParameter("particle_width", "1") ;
		//	particleSystem->setParameter("particle_height", "1") ;
		//	// fast forward 1 second  to the point where the particle has been emitted
		//	particleSystem->fastForward(1.0);
		//	// attach the particle system to a scene node
		//	getRootSceneNode()->attachObject(particleSystem);
		//}



		//now it's time to see how you die, remember the death is not the end, but only a transition
		//std::cout<<agentBlackBox.size()<<"\n";
		//*
		int nbMoi=agentBlackBox.size()/2;
		int nbIA=agentBlackBox.size()/2;
		for (int i = 0;i<agentBlackBox.size();i++){
			if(agentBlackBox[i]!=NULL){
				if(agentBlackBox[i]->isDead()){
					agentBlackBox[i]->destroy();
					agentBlackBox[i]=NULL;
				}
			}
			if(agentBlackBox[i]==NULL){
				if(i<agentBlackBox.size()/2){
					nbMoi--;
				}else{
					nbIA--;
				}
			}
		}

		if(nbMoi==0){
			gameOver=true;
			playerWon=false;
		}else if(nbIA==0){
			gameOver=true;
			playerWon=true;
		}

		if((gameOver==true)&&(startGame==true)){
			Ogre::DisplayString capt ;
			if(playerWon){
				capt="Le joueur a gagne";
			}else{
				capt="L'IA a gagne";
			}
				((OgreBites::TextBox*)m_trayManager->getWidget(OgreBites::TL_CENTER,"txt1"))->clearText();
			
				m_trayManager->showAll();
				((OgreBites::TextBox*)m_trayManager->getWidget(OgreBites::TL_CENTER,"txt1"))->appendText(capt);
				startGame=false;
		}
	}

	bool MainApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		bool result = BaseApplication::mousePressed(arg, id) ;
		m_picking->update(arg, id) ;
		return result ;
	}

	void MainApplication::itemSelected( OgreBites::SelectMenu* menu )
	{
		::std::cout<<"Selection in menu "<<menu->getCaption()<<", item: "<<menu->getSelectedItem()<<::std::endl ;
	}

	bool MainApplication::keyPressed( const OIS::KeyEvent &arg )
	{
		// Keeps the keyboard state up to date
		m_keyboardState.notifyKeyPressed(arg.key) ;
		// Sends notification to super class
		BaseApplication::keyPressed(arg) ;
		return true ;
	}

	bool MainApplication::keyReleased( const OIS::KeyEvent &arg )
	{
		// Keeps the keyboard state up to date
		m_keyboardState.notifyKeyReleased(arg.key) ;
		// Sends notification to super class
		BaseApplication::keyReleased(arg) ;
		return true ;
	}


}


