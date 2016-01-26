#include <stdafx.h>

#include <OgreFramework/AgentPicking.h>
#include <OgreFramework/PickingSelectionBuffer.h>

namespace OgreFramework
{
	DesignPattern::StaticMember<System::MessageEmitter<GameObject::MovedObjectMessage> > AgentPicking::m_movedEmitter ;

	AgentPicking::Agentpicking( Ogre::RenderWindow *renderWindow, Ogre::SceneManager * sceneManager, Ogre::Camera * camera, OIS::MouseButtonID buttonId ) 
		: Picking(sceneManager, camera, buttonId)
	{
		mSelectionBuffer = new Ogre::SelectionBuffer(sceneManager, camera, renderWindow);
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
				notifySelected(selectedEntity,this) ;
				::std::cout<<"Picking on object: "<<selectedEntity->getName()<<::std::endl ;
			}
		}
		if(m_isActive && id!=m_button) {
			int x = arg.state.X.abs;
			int y = arg.state.Y.abs;
			m_lastAgent->setDestination();
			//::std::cout<<"Moving object: "<<m_lastSelected->getName()<<::std::endl ;
		}
	}

	void AgentPicking::onMessage(SmithAgent::MovedObjectMessage const & msg) {
		m_lastAgent = msg.m_selected;
	}