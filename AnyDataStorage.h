
#pragma once

#include "DataCommon.h"
#include "DataType.h"

class AnyDataStorage
{
public:
	// Access
	const IDataType * type() const;
	size_t count() const;
	uint8_t * data();
	const uint8_t * data() const;

	// Modification
	void push( size_t count );
	void pop( size_t count );
	void swap( const index_pair_t * pairs, size_t count );

private:
	const IDataType * m_type = 0;
	uint8_t * m_data = 0;
	size_t m_capacity = 0;
	size_t m_count = 0;
};
