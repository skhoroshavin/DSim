
#pragma once

#include <stdint.h>
#include <unordered_map>
#include <vector>

#include "DataBlock.h"

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
	size_t create( EntityID id );
	bool isValid( EntityID id ) const;
	size_t index( EntityID id ) const;
	void * data( EntityID id, size_t& idx, size_t db );
	void destroy( EntityID id );

protected:
	// Configuration
	inline void setID( ComponentID id ) { m_ID = id; }
	template<typename T> void addDataBlock() { m_Data.push_back( std::make_unique<T>() ); }

private:
	ComponentID m_ID = 0;

	std::unordered_map<EntityID,size_t> m_Index;
	std::vector<EntityID>               m_Entites;
	std::vector<CDataBlock::UPtr>       m_Data;
};

class CComponent
{
public:
	// Construction
	inline CComponent() { defaultInit(); }
	inline CComponent( CComponent&& other ) { assign( std::move(other) ); }
	inline CComponent& operator=( CComponent&& other ) { reset(); assign( std::move(other) ); return *this; }
	CComponent( const CComponent& other ) = delete;
	CComponent& operator=( const CComponent& other ) = delete;
	inline ~CComponent() { reset(); }

	void reset();

protected:
	CComponent( CComponentManager * pMgr, EntityID id );

	// Data access
	inline const void * data( size_t db ) const { return m_pMgr->data( m_EntityID, m_Index, db ); }
	inline void * data( size_t db ) { return m_pMgr->data( m_EntityID, m_Index, db ); }

private:
	CComponentManager * m_pMgr;
	EntityID m_EntityID;
	mutable size_t m_Index;

	void defaultInit();
	void assign( CComponent&& other );
};
