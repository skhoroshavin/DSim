
#include "ComponentManager.h"

#include <assert.h>


void CComponentManager::createComponent(EntityID id)
{
	assert( !hasComponent(id) );
	m_Index[id] = m_Data.size();
	m_Data.push_back( id );
}

bool CComponentManager::hasComponent(EntityID id) const
{
	return m_Index.find(id) != m_Index.end();
}

void CComponentManager::destroyComponent(EntityID id)
{
	auto it = m_Index.find(id);
	assert( it != m_Index.end() );

	size_t idx = it->second;
	assert( idx < m_Data.size() );

	if( idx+1 != m_Data.size() )
	{
		EntityID lastID = m_Data.back();
		m_Data[idx] = lastID;
		m_Index[lastID] = idx;
	}

	m_Index.erase( it );
	m_Data.pop_back();
}

void CComponentManager::setID(ComponentID id)
{
	m_ID = id;
}
