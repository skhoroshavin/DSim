
#pragma once

#include <unordered_set>

#include "ComponentManager.h"

class CWorldManager
{
public:
	// Make movable only
	CWorldManager() = default;
	CWorldManager( const CWorldManager& other ) = delete;
	CWorldManager& operator=( const CWorldManager& other ) = delete;

	// Entities
	EntityID createEntity();
	bool isEntityValid( EntityID id ) const;
	void destroyEntity( EntityID id );

	// Components
	void createComponent( EntityID entityID, ComponentID componentID );
	bool hasComponent( EntityID entityID, ComponentID componentID ) const;
	void destroyComponent( EntityID entityID, ComponentID componentID );

	// Events

	// Properties


protected:
	// Configuration
	void registerComponent( CComponentManager&& mgr );
	//void registerEvent();


private:
	EntityID m_NextEntityID = 0;
	std::unordered_set<EntityID> m_Entities;
	std::vector<CComponentManager> m_Components;
};
