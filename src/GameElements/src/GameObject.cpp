#include <stdafx.h>

#include <GameElements/GameObject.h>

namespace GameElements
{
	DesignPattern::StaticMember<System::MessageEmitter<GameObject::SelectedGameObjectMessage> > GameObject::m_selectedEmitter ;
	DesignPattern::StaticMember<System::MessageEmitter<GameObject::UnselectedGameObjectMessage> > GameObject::m_unselectedEmitter ;
	//DesignPattern::StaticMember<System::MessageEmitter<GameObject::MovedGameObjectMessage> > GameObject::m_movedEmitter ;


	GameObject::GameObject(Ogre::SceneNode * node)
		: OgreFramework::EntityAdapter(node), m_position(0.0,0.0), m_orientation(0.0)
	{
		System::ConstructionDestructionEmitter<GameObject>::getConstructionEmitter()->send(System::ConstructionMessage<GameObject>(*this)) ;
	}

	GameObject::~GameObject()
	{
		System::ConstructionDestructionEmitter<GameObject>::getDestructionEmitter()->send(System::DestructionMessage<GameObject>(*this)) ;
	}

	void GameObject::onSelect()
	{
		//::std::cout<<"Je suis GameObject::onSelect()"<<::std::endl ;
		OgreFramework::EntityAdapter::onSelect() ;		
		getSelectedGameObjectEmitter()->send(SelectedGameObjectMessage(*this)) ;
	}

	/*void GameObject::onMovement(int x, int y)
	{
		::std::cout<<"Je suis je suis je suis GameObject::onMovement()"<<::std::endl ;
		//getMovedGameObjectEmitter()->send(MovedGameObjectMessage(*this,x,y));
	}*/

	void GameObject::onUnselect()
	{
		OgreFramework::EntityAdapter::onUnselect() ;
		getUnselectedGameObjectEmitter()->send(UnselectedGameObjectMessage(*this)) ;
	}

	System::MessageEmitter<GameObject::SelectedGameObjectMessage> * GameObject::getSelectedGameObjectEmitter()
	{
		return m_selectedEmitter.getInstance() ;
	}

	/*System::MessageEmitter<GameObject::MovedGameObjectMessage> * GameObject::getMovedGameObjectEmitter()
	{
		return m_movedEmitter.getInstance() ;
	}*/

	System::MessageEmitter<GameObject::UnselectedGameObjectMessage> * GameObject::getUnselectedGameObjectEmitter()
	{
		return m_unselectedEmitter.getInstance() ;
	}

	void GameObject::setPosition( Math::Vector3<Config::Real> const & position )
	{
		m_position = position.projectZ() ;
		updateCollisionShapeTransform() ;
		EntityAdapter::setPosition(position) ;
	}

	void GameObject::setOrientation( Config::Real angleX, Config::Real angleY, Config::Real angleZ )
	{
		m_orientation = angleZ ;
		updateCollisionShapeTransform() ;
		EntityAdapter::setOrientation(angleX, angleY, angleZ) ;
	}

	void GameObject::updateCollisionShapeTransform()
	{
		Math::Matrix3x3<Config::Real> transform = Math::Matrix3x3<Config::Real>::getTranslationHomogeneous(m_position)*Math::Matrix3x3<Config::Real>::getRotationHomogeneous(m_orientation) ;
		setTransform(transform) ;
	}
}