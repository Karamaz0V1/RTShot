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
		if(m_isActive && id!=m_button) {
			int x = arg.state.X.abs;
			int y = arg.state.Y.abs;
			if(m_lastAgent != NULL)
				m_lastAgent->setDestination();
			//::std::cout<<"Moving object: "<<m_lastSelected->getName()<<::std::endl ;
		}
	}

	void AgentPicking::onMessage(GameElements::SmithAgent::MovedObjectMessage const & msg) {
		::std::cout<<"Message personnel : la mienne est plus grande que la vôtre"<<::std::endl;
		m_lastAgent = msg.m_selected;
	}
}