
#pragma once

#include "DSimCommon.h"
#include "DataType.h"

namespace DSim {

class GenericDataStorage
{
public:
	// Construction/destruction
	GenericDataStorage() = delete;
	explicit GenericDataStorage( const IDataType * type );
	~GenericDataStorage();

	// Access
	const IDataType * type() const;
	size_t size() const;
	size_t capacity() const;
	void * data( size_t idx = 0 );
	const void * data( size_t idx = 0 ) const;

	// Modification
	void reserve( size_t capacity );
	void push( size_t size );
	void pop( size_t size );
	void reorder( const swap_t * pairs, size_t count );
	void clear();
	void reset();

private:
	const IDataType * m_type = 0;
	void * m_data = 0;
	size_t m_capacity = 0;
	size_t m_size = 0;
};

} // namespace DSim
