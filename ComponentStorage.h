
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
	index_t fieldCount() const;
	const IDataType * fieldType( index_t idx ) const;
	index_t addField( const IDataType * pType );

	// Data access
	size_t count() const;
	index_t find( uuid_t id, index_t hint = INVALID_INDEX ) const;

	// Data modification
	index_t create( uuid_t id );
	void destroy( index_t idx );

private:
	std::vector<GenericDataStorage> m_Fields;
};

} // namespace DSim
