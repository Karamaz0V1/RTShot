#ifndef _OgreFramework_AgentPicking_H
#define _OgreFramework_AgentPicking_H

#include <OgreFramework/Picking.h>
#include <Ogre/SelectionBuffer.h>
#include <GameElements/SmithAgent.h>
//#include <System/CallbackMessageListener.h>

namespace OgreFramework
{
	class AgentPicking : Picking, System::MessageListener<GameElements::SmithAgent::MovedObjectMessage>
	{
	protected:
		GameElements::SmithAgent * m_lastAgent;
		Ogre::SelectionBuffer * mSelectionBuffer;
		static DesignPattern::StaticMember<System::MessageEmitter<GameElements::SmithAgent::MovedObjectMessage> > m_movedEmitter ;

	public:
		AgentPicking(Ogre::RenderWindow *renderWindow, Ogre::SceneManager * sceneManager, Ogre::Camera * camera, OIS::MouseButtonID buttonId);
		virtual void update(const OIS::MouseEvent &arg, OIS::MouseButtonID id) = 0 ;
		GameElements::SmithAgent * getLastAgent() const;
		void onMessage(GameElements::SmithAgent::MovedObjectMessage const & msg);
	};
}
#endif