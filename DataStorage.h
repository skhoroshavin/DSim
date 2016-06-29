
#pragma once

#include "DataCommon.h"

template<typename T>
class DataStorage
{
public:
	// Access
	size_t count() const { return m_data.size();}
	T * data() { return m_data.data(); }
	const T * data() const { return m_data.data(); }


	// Modification
	void push( size_t count )
	{
		m_data.resize( m_data.size() + count );
	}

	void pop( size_t count )
	{
		if( count > m_data.size() )
			throw std::logic_error( "Trying to pop more elements than size" );
		m_data.resize( m_data.size() - count );
	}

	void swap( const index_pair_t * pairs, size_t count )
	{
		for( size_t i = 0; i < count; ++i )
		{
			index_t a = pairs[i].first;
			index_t b = pairs[i].second;
			std::swap( m_data[a], m_data[b] );
		}
	}

private:
	std::vector<T> m_data;
};
