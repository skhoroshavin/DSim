
#include "GenericDataStorage.h"

namespace DSim {

GenericDataStorage::GenericDataStorage( const IDataType * type ) :
	m_type(type)
{

}

const IDataType * GenericDataStorage::type() const
{
	return m_type;
}

size_t GenericDataStorage::count() const
{
	return 0;
}

uint8_t * GenericDataStorage::data()
{

}

const uint8_t * GenericDataStorage::data() const
{

}

void GenericDataStorage::push( size_t count )
{

}

void GenericDataStorage::pop( size_t count )
{

}

void GenericDataStorage::reorder( const swap_t * pairs, size_t count )
{

}

} // namespace DSim
