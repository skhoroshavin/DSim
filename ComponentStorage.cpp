
#include "ComponentStorage.h"

namespace DSim {

index_t ComponentStorage::fieldCount() const
{
	return m_Fields.size();
}

const IDataType * ComponentStorage::fieldType(index_t idx ) const
{
	return m_Fields.at(idx).type();
}

index_t ComponentStorage::addField( const IDataType * pType )
{
	index_t result = m_Fields.size();
	m_Fields.emplace_back( pType );
	return result;
}

size_t ComponentStorage::count() const
{
	return 0;
}

index_t ComponentStorage::find( uuid_t id, index_t hint ) const
{
	return INVALID_INDEX;
}

index_t ComponentStorage::create( uuid_t id )
{
//	DSIM_ASSERT( false, "Fails to create" );
}

void ComponentStorage::destroy( index_t idx )
{
	//	DSIM_ASSERT( false );
}

} // namespace DSim
