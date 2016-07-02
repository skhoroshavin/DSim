
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
	// Data fields
	size_t fieldCount() const;
	const IDataType * fieldType( size_t idx ) const;
	size_t addField( const IDataType * pType );

	// Data access
	size_t size() const;
	size_t find( uuid_t id, size_t hint = INVALID_INDEX ) const;

	// Data modification
	size_t create( uuid_t id );
	void destroy( size_t idx );

	// Listeners
	void addListener( IComponentListener * listener );
	void removeListener( IComponentListener * listener );

private:
	DataIndex m_index;
	std::vector<GenericDataStorage> m_fields;
	std::vector<IComponentListener*> m_listeners;
};

} // namespace DSim
