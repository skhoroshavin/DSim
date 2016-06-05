
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>

using EntityID    = uint64_t;
using ComponentID = uint32_t;

class CComponentManager
{
public:
	// Make movable only
	CComponentManager() = default;
	CComponentManager( const CComponentManager& other ) = delete;
	CComponentManager& operator=( const CComponentManager& other ) = delete;
	CComponentManager( CComponentManager&& other ) = default;
	CComponentManager& operator=( CComponentManager&& other ) = default;

	// Metadata
	inline ComponentID id() const { return m_ID; }

	// Component management
	void createComponent( EntityID id );
	bool hasComponent( EntityID id ) const;
	void destroyComponent( EntityID id );

protected:
	// Configuration
	void setID( ComponentID id );

private:
	ComponentID m_ID = 0;

	std::unordered_map<EntityID,size_t> m_Index;
	std::vector<EntityID>               m_Data;
};
