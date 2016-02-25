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
		m_target = NULL;
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
			m_rightClick = false;
			selectedEntity = mSelectionBuffer->OnSelectionClick(arg.state.X.abs, arg.state.Y.abs) ;
			if(static_cast<Ogre::MovableObject*>(selectedEntity)!=m_lastSelected)
			{
				notifyUnselected(getLastSelected()) ;
			}
			if(selectedEntity!=0)
			{
				m_lastSelected = selectedEntity ;
				notifySelected(selectedEntity) ;
			}
		}
		if(m_isActive && id==OIS::MB_Right) {
			m_rightClick = true;
			int x = arg.state.X.abs;
			int y = arg.state.Y.abs;
			Ogre::Entity * laink = mSelectionBuffer->OnSelectionClick(x, y);
			if(m_lastAgent != NULL) {
				if(laink!=0)
				{
					if(laink->getName() != "scene0Box001") {
						notifySelected(laink) ;
					}
					else {
						m_lastAgent->setTarget(NULL);
						m_lastAgent->go2(getDestinationPos(arg,x,y).projectZ());
					}
				}	
			}
		}
	}

	Math::Vector3<Config::Real> AgentPicking::getDestinationPos(const OIS::MouseEvent &arg, int x, int y) {
		Ogre::Ray mouseRay = m_camera->getCameraToViewportRay(x/float(arg.state.width),y/float(arg.state.height));
		std::pair<bool,Config::Real> result = mouseRay.intersects(Ogre::Plane(Ogre::Vector3(0,0,1), Ogre::Vector3(0,0,0)));
		Ogre::Real distance = result.second;
		Ogre::Vector3 position = mouseRay.getPoint(distance);
		Math::Vector3<Config::Real> posf(position[0], position[1], position[2]);
		return posf;
	}

	void AgentPicking::onMessage(GameElements::SmithAgent::MovedObjectMessage const & msg) {
		if(!m_rightClick) {
			m_lastAgent = msg.m_selected;
		}
		else {
			m_target = msg.m_selected;
			m_lastAgent->setTarget(m_target);
		}
	}
}