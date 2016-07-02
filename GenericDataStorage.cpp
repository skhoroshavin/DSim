
#include "GenericDataStorage.h"

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
	if( capacity <= m_capacity ) return;

	void * newData = aligned_alloc( m_type->alignment(), capacity * m_type->size() );
	if( !newData )
		throw std::runtime_error( "GenericDataStorage: Out of memory" );

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
