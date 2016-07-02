
#pragma once

#include "DataIndex.h"
#include "GenericDataStorage.h"

namespace DSim {

class IComponentListener
{
public:
	virtual ~IComponentListener() = default;

	virtual void componentsCreated( size_t numCreated ) = 0;
	virtual void componentsDestroyed( size_t numDestroyed ) = 0;
	virtual void componentsReordered( const swap_t * swaps, size_t numSwaps ) = 0;
};

class ComponentStorage
{
public:
	// Data scheme
	size_t fieldCount() const;
	const IDataType * fieldType( size_t idx ) const;
	size_t addField( const IDataType * pType );

	// Current state
	enum State : uint8_t { Read, Update, Create, Destroy };
	void setState( State state );
	State state() const;

	// Data access
	size_t size() const;
	size_t find( UUID id, size_t hint = INVALID_INDEX ) const;
	const void * data( size_t field ) const;
	void * data( size_t field );

	// Data modification
	size_t create( UUID id );
	void destroy( size_t idx );

	// Listeners
	void addListener( IComponentListener * listener );
	void removeListener( IComponentListener * listener );

private:
	DataIndex m_index;
	std::vector<GenericDataStorage>  m_fields;
	std::vector<IComponentListener*> m_listeners;
	State m_state = Read;

	void applyCreate();
	void applyDestroy();
	void applyReorders();
};

class ComponentStorageSection
{
public:
	inline ComponentStorageSection( ComponentStorage * storage, ComponentStorage::State state ) : m_storage(storage)
	{
		m_state = m_storage->state();
		m_storage->setState( state );
	}
	inline ~ComponentStorageSection() { m_storage->setState( m_state );	}
private:
	ComponentStorage *      m_storage;
	ComponentStorage::State m_state;
};

} // namespace DSim
