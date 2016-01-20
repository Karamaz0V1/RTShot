#include <stdafx.h>

#include <OgreFramework/PickableObject.h>
#include <Ogre/CollectEntities.h>

namespace OgreFramework
{
	//DesignPattern::StaticMember<::std::map<Ogre::Node*, PickableObject*> > PickableObject::m_instances;
	DesignPattern::StaticMember<::std::map<Ogre::Entity*, PickableObject*> > PickableObject::m_pickableInstances ;
	
	DesignPattern::StaticMember<System::MessageEmitter<PickableObject::SelectedObjectMessage> > PickableObject::m_selectedEmitter ;
	DesignPattern::StaticMember<System::MessageEmitter<PickableObject::MovedObjectMessage> > PickableObject::m_movedEmitter ;
	DesignPattern::StaticMember<System::MessageEmitter<PickableObject::UnselectedObjectMessage> > PickableObject::m_unselectedEmitter ;

	PickableObject::PickableObject(Ogre::Node * object)
		: m_object(object), m_isSelected(false)
	{
		//(*m_instances.getInstance())[object] = this ;

		//::std::vector<Ogre::Entity*> entities ;
		Ogre::collectEntities(m_object, m_entities) ;
		for(auto it=m_entities.begin(), end=m_entities.end() ; it!=end ; ++it)
		{
			(*m_pickableInstances.getInstance())[*it] = this ;
		}
		//::std::cout<<"Referenced "<<entities.size()<<", size of map: "<<m_pickableInstances.getInstance()->size()<<::std::endl ;
		//char c ;
		//::std::cin>>c ;
	}

	PickableObject::~PickableObject()
	{
		//(*m_instances.getInstance()).erase(m_object) ;

		for(auto it=m_entities.begin(), end=m_entities.end() ; it!=end ; ++it)
		{
			(*m_pickableInstances.getInstance()).erase(*it) ;
		}
	}

	bool PickableObject::isSelected() const
	{
		return m_isSelected ;
	}

	void PickableObject::onSelect()
	{
		//::std::cout<<"Je suis PickableObject::onSelect()"<<::std::endl ;
		m_isSelected = true ;		
		getSelectedEmitter()->send(SelectedObjectMessage(*this)) ;
	}

	void PickableObject::onMovement(int x, int y)
	{
		::std::cout<<"Je suis PickableObject::onMovement()"<<::std::endl ;
		getMovedEmitter()->send(MovedObjectMessage(*this,x,y)) ;
	}

	void PickableObject::onUnselect()
	{
		m_isSelected = false ;
		getUnselectedEmitter()->send(UnselectedObjectMessage(*this)) ;
	}

	System::MessageEmitter<PickableObject::SelectedObjectMessage> * PickableObject::getSelectedEmitter()
	{
		return m_selectedEmitter.getInstance() ;
	}

	System::MessageEmitter<PickableObject::MovedObjectMessage> * PickableObject::getMovedEmitter()
	{
		return m_movedEmitter.getInstance() ;
	}

	System::MessageEmitter<PickableObject::UnselectedObjectMessage> * PickableObject::getUnselectedEmitter()
	{
		return m_unselectedEmitter.getInstance()	;
	}
}