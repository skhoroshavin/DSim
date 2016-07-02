
#include "ComponentStorage.h"

namespace DSim {

size_t ComponentStorage::fieldCount() const
{
	return m_fields.size();
}

const IDataType * ComponentStorage::fieldType(size_t idx ) const
{
	return m_fields.at(idx).type();
}

size_t ComponentStorage::addField( const IDataType * pType )
{
	size_t result = m_fields.size();
	m_fields.emplace_back( pType );
	return result;
}

size_t ComponentStorage::size() const
{
	return m_index.size();
}

size_t ComponentStorage::find( uuid_t id, size_t hint ) const
{
	return m_index.findById( id, hint );
}

size_t ComponentStorage::create( uuid_t id )
{
	DSIM_ASSERT( find(id) >= size(), "Trying to create same UUID twice" );

	size_t idx = m_index.create( id );

	for( GenericDataStorage& data : m_fields )
	{
		data.push( 1 );
		data.swap( m_index.swaps(), m_index.swapCount() );
	}

	for( IComponentListener * listener : m_listeners )
	{
		listener->componentsCreated( 1 );
		listener->componentsReordered( m_index.swaps(), m_index.swapCount() );
	}

	m_index.clearSwaps();
	return idx;
}

void ComponentStorage::destroy( size_t idx )
{

}

void ComponentStorage::addListener( IComponentListener * listener )
{
	DSIM_ASSERT( std::find( m_listeners.begin(), m_listeners.end(), listener ) == m_listeners.end(), "Trying to add same listener twice" );
	m_listeners.push_back( listener );
}

void ComponentStorage::removeListener( IComponentListener * listener )
{
	auto it = std::find( m_listeners.begin(), m_listeners.end(), listener );
	DSIM_ASSERT( it != m_listeners.end(), "Trying to delete nonexistant listener" );
	m_listeners.erase( it );
}

} // namespace DSim
