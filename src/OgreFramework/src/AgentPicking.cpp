#include <stdafx.h>

#include <OgreFramework/AgentPicking.h>
#include <OgreFramework/PickingSelectionBuffer.h>

namespace OgreFramework
{
	//DesignPattern::StaticMember<System::MessageEmitter<GameElements::SmithAgent::MovedObjectMessage> > AgentPicking::m_movedEmitter ;

	AgentPicking::AgentPicking( Ogre::RenderWindow *renderWindow, Ogre::SceneManager * sceneManager, Ogre::Camera * camera, OIS::MouseButtonID buttonId, System::MessageEmitter<GameElements::SmithAgent::MovedObjectMessage> * emitter) 
		: Picking(sceneManager, camera, buttonId),MessageListener<GameElements::SmithAgent::MovedObjectMessage>(emitter)
	{
		mSelectionBuffer = new Ogre::SelectionBuffer(sceneManager, camera, renderWindow);
		m_lastAgent = NULL;
	}

	GameElements::SmithAgent * AgentPicking::getLastAgent() const
	{
		return m_lastAgent ;
	}

	void AgentPicking::update( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		//if(mSelectionBuffer)
		//	mSelectionBuffer->ShowOverlay(true);

		Ogre::Entity *selectedEntity ;

		if(m_isActive && id==m_button)
		{
			selectedEntity = mSelectionBuffer->OnSelectionClick(arg.state.X.abs, arg.state.Y.abs) ;
			if(static_cast<Ogre::MovableObject*>(selectedEntity)!=m_lastSelected)
			{
				notifyUnselected(getLastSelected()) ;
			}
			if(selectedEntity!=0)
			{
				m_lastSelected = selectedEntity ;
				notifySelected(selectedEntity) ;
				::std::cout<<"Picking on object: "<<selectedEntity->getName()<<::std::endl ;
			}
		}
		if(m_isActive && id==OIS::MB_Right) {
			if(m_lastAgent != NULL) {
				int x = arg.state.X.abs;
				int y = arg.state.Y.abs;
				Ogre::Ray mouseRay = m_camera->getCameraToViewportRay(x/float(arg.state.width),y/float(arg.state.height));
				std::pair<bool,Config::Real> result = mouseRay.intersects(Ogre::Plane(Ogre::Vector3(0,0,1), Ogre::Vector3(0,0,0)));
				Ogre::Real distance = result.second;
				Ogre::Vector3 position = mouseRay.getPoint(distance);
				Math::Vector3<Config::Real> posf(position[0], position[1], position[2]);
				m_lastAgent->go2(posf.projectZ());
				Ogre::Entity * laink = mSelectionBuffer->OnSelectionClick(x, y);
				if(laink != 0 && laink != m_lastSelected) {
					if(laink->getName() != "scene0Box001")
						m_lastAgent->setTarget(laink);
				}
			}
		}
	}

	void AgentPicking::onMessage(GameElements::SmithAgent::MovedObjectMessage const & msg) {
		::std::cout<<"Message personnel : la mienne est plus grande que la vôtre"<<::std::endl;
		m_lastAgent = msg.m_selected;
	}
}