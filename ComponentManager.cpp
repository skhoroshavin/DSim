
#include "ComponentManager.h"

#include <assert.h>

size_t CComponentManager::create(EntityID id)
{
	assert( !isValid(id) );

	size_t idx = m_Entites.size();
	m_Index[id] = idx;
	m_Entites.push_back( id );
	for( auto& db : m_Data )
		db->push();

	return idx;
}

bool CComponentManager::isValid(EntityID id) const
{
	return index(id) < m_Entites.size();
}

size_t CComponentManager::index(EntityID id) const
{
	auto it = m_Index.find(id);
	if( it == m_Index.end() ) return -1;
	return it->second;
}

void * CComponentManager::data(EntityID id, size_t& idx, size_t db)
{
	if( (idx >= m_Entites.size()) || (m_Entites[idx] != id) )
		idx = index(id);
	assert( idx < m_Entites.size() );

	CDataBlock * pDB = m_Data.at(db).get();
	return pDB->data( idx );
}

void CComponentManager::destroy(EntityID id)
{
	auto it = m_Index.find(id);
	assert( it != m_Index.end() );

	size_t idx = it->second;
	assert( idx < m_Entites.size() );

	size_t lastIdx = m_Entites.size()-1;

	if( idx != lastIdx )
	{
		EntityID lastID = m_Entites[lastIdx];
		std::swap( m_Entites[idx], m_Entites[lastIdx] );
		for( const auto& db : m_Data )
			db->swap( idx, lastIdx );
		m_Index[lastID] = idx;
	}

	m_Index.erase( it );
	m_Entites.pop_back();
	for( auto& db : m_Data )
		db->pop();
}

CComponent::CComponent( CComponentManager *pMgr, EntityID id )
{
	m_pMgr = pMgr;
	m_EntityID = id;
	m_Index = m_pMgr->create( m_EntityID );
}

void CComponent::reset()
{
	if( m_pMgr )
	{
		m_pMgr->destroy( m_EntityID );
		m_pMgr = 0;
		m_EntityID = 0;
		m_Index = -1;
	}
}

void CComponent::defaultInit()
{
	m_pMgr = 0;
	m_EntityID = 0;
	m_Index = -1;
}

void CComponent::assign(CComponent &&other)
{
	m_pMgr = other.m_pMgr;
	m_EntityID = other.m_EntityID;
	m_Index = other.m_Index;

	other.defaultInit();
}
