
#include "WorldManager.h"

EntityID CWorldManager::createEntity()
{
	EntityID id = m_NextEntityID;
	++m_NextEntityID;

	m_Entities.insert( id );
	return id;
}

bool CWorldManager::isEntityValid( EntityID id ) const
{
	return m_Entities.find( id ) != m_Entities.end();
}

void CWorldManager::destroyEntity( EntityID id )
{
	m_Entities.erase( id );
}

void CWorldManager::createComponent(EntityID entityID, ComponentID componentID)
{
	m_Components.at(componentID).createComponent(entityID);
}

bool CWorldManager::hasComponent(EntityID entityID, ComponentID componentID) const
{
	return m_Components.at(componentID).hasComponent(entityID);
}

void CWorldManager::destroyComponent(EntityID entityID, ComponentID componentID)
{
	m_Components.at(componentID).destroyComponent(entityID);
}

void CWorldManager::registerComponent( CComponentManager &&mgr )
{
	if( m_Components.size() <= mgr.id() )
		m_Components.resize( mgr.id()+1 );

	m_Components[mgr.id()] = std::move(mgr);
}
