
#include "GenericDataStorage.h"

#include <stdlib.h>

namespace DSim {

GenericDataStorage::GenericDataStorage( const IDataType * type ) :
	m_type(type)
{

}

GenericDataStorage::~GenericDataStorage()
{
	if( m_data )
	{

		m_data = 0;
	}
}

const IDataType * GenericDataStorage::type() const
{
	return m_type;
}

size_t GenericDataStorage::size() const
{
	return m_size;
}

size_t GenericDataStorage::capacity() const
{
	return m_capacity;
}

void * GenericDataStorage::data( size_t idx )
{
	DSIM_ASSERT( m_type, "Trying to access typeless data storage" );
	return static_cast<char*>(m_data) + idx * m_type->size();
}

const void * GenericDataStorage::data( size_t idx ) const
{
	DSIM_ASSERT( m_type, "Trying to access typeless data storage" );
	return static_cast<char*>(m_data) + idx * m_type->size();
}

void GenericDataStorage::reserve( size_t capacity )
{
	DSIM_ASSERT( m_type, "Trying to modify typeless data storage" );

	enum { MIN_ALIGN = 0x10 };
	size_t capacityTail = capacity & (MIN_ALIGN-1);
	if( capacityTail )
		capacity += MIN_ALIGN - capacityTail;
	if( capacity <= m_capacity ) return;

	size_t alignment = m_type->alignment();
	if( alignment < MIN_ALIGN ) alignment = MIN_ALIGN;

	void * newData;
	if( int err = posix_memalign( &newData, alignment, capacity * m_type->size() ) )
		throw std::runtime_error( "GenericDataStorage: Failed to allocate memory" );

	if( m_data )
	{
		m_type->move( m_data, newData, m_type->size(), m_size );
		m_type->done( m_data, m_type->size(), m_size );
		free( m_data );
	}
	m_data = newData;
	m_capacity = capacity;
}

void GenericDataStorage::push( size_t count )
{
	DSIM_ASSERT( m_type, "Trying to modify typeless data storage" );
	reserve( m_size + count );
	m_type->init( data(m_size), m_type->size(), count );
	m_size += count;
}

void GenericDataStorage::pop( size_t count )
{
	DSIM_ASSERT( count <= m_size, "Trying to pop more elements than contained" );

}

void GenericDataStorage::reorder( const swap_t * pairs, size_t count )
{

}

void GenericDataStorage::clear()
{

}

void GenericDataStorage::reset()
{

}

} // namespace DSim
