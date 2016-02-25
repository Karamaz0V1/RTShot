#ifndef _OgreFramework_AgentPicking_H
#define _OgreFramework_AgentPicking_H

#include <OgreFramework/Picking.h>
#include <Ogre/SelectionBuffer.h>
#include <GameElements/SmithAgent.h>
//#include <System/CallbackMessageListener.h>

namespace OgreFramework
{
	class AgentPicking : public Picking, System::MessageListener<GameElements::SmithAgent::MovedObjectMessage>
	{
	protected:
		bool m_rightClick; //true if right-click, false if left
		GameElements::SmithAgent * m_target;
		GameElements::SmithAgent * m_lastAgent;
		Ogre::SelectionBuffer * mSelectionBuffer;
		Math::Vector3<Config::Real> getDestinationPos(const OIS::MouseEvent &arg, int x, int y);
		//static DesignPattern::StaticMember<System::MessageEmitter<GameElements::SmithAgent::MovedObjectMessage> > m_movedEmitter ;

	public:
		AgentPicking(Ogre::RenderWindow *renderWindow, Ogre::SceneManager * sceneManager, Ogre::Camera * camera, OIS::MouseButtonID buttonId, System::MessageEmitter<GameElements::SmithAgent::MovedObjectMessage> * emitter);
		void update(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		GameElements::SmithAgent * getLastAgent() const;
		void onMessage(GameElements::SmithAgent::MovedObjectMessage const & msg);
	};
}
#endif