
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
	DSIM_ASSERT( m_index.size() == 0, "Trying to add field to non-empty component storage" );
	size_t result = m_fields.size();
	m_fields.emplace_back( pType );
	return result;
}

void ComponentStorage::setState( State state )
{
	if( m_state == state ) return;

	switch( m_state )
	{
	case State::Create:
		applyCreate();
		break;
	case State::Destroy:
		applyDestroy();
		break;
	default:
		break;
	}

	m_state = state;
}

ComponentStorage::State ComponentStorage::state() const
{
	return m_state;
}

size_t ComponentStorage::size() const
{
	return m_index.size();
}

size_t ComponentStorage::find( UUID id, size_t hint ) const
{
	return m_index.findById( id, hint );
}

const void * ComponentStorage::data( size_t field ) const
{
	DSIM_ASSERT( field < m_fields.size(), "Trying to access data from nonexistant field" );
	return m_fields[field].data();
}

void * ComponentStorage::data( size_t field )
{
	DSIM_ASSERT( field < m_fields.size(), "Trying to access data from nonexistant field" );
	return m_fields[field].data();
}

size_t ComponentStorage::create( UUID id )
{
	DSIM_ASSERT( m_state == State::Create, "Trying to create component from invalid state" );
	DSIM_ASSERT( find(id) >= size(), "Trying to create same UUID twice" );

	size_t idx = m_index.create( id );

	for( GenericDataStorage& data : m_fields )
		data.push( 1 );

	for( IComponentListener * listener : m_listeners )
		listener->componentsCreated( 1 );

	applyReorders();

	return idx;
}

void ComponentStorage::destroy( size_t idx )
{
	DSIM_ASSERT( m_state == State::Destroy, "Trying to destroy component from invalid state" );
	DSIM_ASSERT( idx < size(), "Trying to destroy nonexistant index" );

	m_index.destroy( idx );

	applyReorders();

	for( IComponentListener * listener : m_listeners )
		listener->componentsDestroyed( 1 );

	for( GenericDataStorage& data : m_fields )
		data.pop( 1 );
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

void ComponentStorage::applyCreate()
{

}

void ComponentStorage::applyDestroy()
{

}

void ComponentStorage::applyReorders()
{
	if( !m_index.swapCount() ) return;

	for( GenericDataStorage& data : m_fields )
		data.reorder( m_index.swapData(), m_index.swapCount() );

	for( IComponentListener * listener : m_listeners )
		listener->componentsReordered( m_index.swapData(), m_index.swapCount() );

	m_index.clearSwaps();
}

} // namespace DSim
