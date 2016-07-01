
#include "DataIndex.h"

#include <algorithm>

namespace DSim {

index_t DataIndex::findById( uuid_t id, index_t hint ) const
{
	if( (hint < m_entities.size()) && (m_entities[hint] == id) )
		return hint;

	auto it = m_index.find( id );
	if( it == m_index.end() )
		return m_entities.size();
	return it->second;
}

swap_t DataIndex::findByGroup(group_t min_group, group_t max_group ) const
{
	auto itMin = std::lower_bound( m_groups.begin(), m_groups.end(), min_group );
	auto itMax = std::upper_bound( m_groups.begin(), m_groups.end(), max_group );

	return swap_t( itMin - m_groups.begin(), itMax - m_groups.begin() );
}

size_t DataIndex::count() const
{
	return m_entities.size();
}

const uuid_t * DataIndex::entities() const
{
	return m_entities.data();
}

const group_t * DataIndex::groups() const
{
	return m_groups.data();
}

index_t DataIndex::create(uuid_t id, group_t group )
{
	index_t idx = findById( id );
	if( idx < m_entities.size() )
		throw std::logic_error( "Trying to create existing component" );

	index_t target_idx = std::distance( m_groups.begin(), std::upper_bound( m_groups.begin(), m_groups.end(), group ) );

	m_index[id] = idx;
	m_entities.push_back( id );
	m_groups.push_back( group );

	index_t swap_idx = target_idx;
	while( swap_idx < idx )
	{
		swap( swap_idx, idx );
		swap_idx = std::upper_bound( m_groups.begin(), m_groups.end()-1, m_groups.back() ) - m_groups.begin();
	}

	return target_idx;
}

void DataIndex::destroy( index_t idx )
{
	if( idx >= m_entities.size() )
		throw std::logic_error( "Trying to remove nonexistant component" );

	swap( idx, m_entities.size()-1 );

	index_t swap_idx = std::lower_bound( m_groups.begin() + idx + 1, m_groups.end()-1, m_groups[idx] ) - m_groups.begin() - 1;
	while( swap_idx > idx )
	{
		swap( idx, swap_idx );
		swap_idx = std::lower_bound( m_groups.begin() + idx + 1, m_groups.end()-1, m_groups[idx] ) - m_groups.begin() - 1;
	}

	m_index.erase( m_entities.back() );
	m_entities.pop_back();
	m_groups.pop_back();
}

size_t DataIndex::swapCount() const
{
	return m_swaps.size();
}

const swap_t * DataIndex::swaps() const
{
	return m_swaps.data();
}

void DataIndex::clearSwaps()
{
	m_swaps.clear();
}

void DataIndex::swap( index_t a, index_t b )
{
	uuid_t id1 = m_entities[a];
	uuid_t id2 = m_entities[b];

	m_entities[b] = id1;
	m_entities[a] = id2;

	m_index[id1] = b;
	m_index[id2] = a;

	std::swap( m_groups[a], m_groups[b] );

	m_swaps.emplace_back( a, b );
}

} // namespace DSim
