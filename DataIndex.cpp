
#include "DataIndex.h"

#include <algorithm>

index_t DataIndex::findById( entity_t id, index_t hint ) const
{
	if( (hint < m_entities.size()) && (m_entities[hint] == id) )
		return hint;

	auto it = m_index.find( id );
	if( it == m_index.end() )
		return m_entities.size();
	return it->second;
}

index_pair_t DataIndex::findByBucket( bucket_t min_bucket, bucket_t max_bucket ) const
{
	auto itMin = std::lower_bound( m_buckets.begin(), m_buckets.end(), min_bucket );
	auto itMax = std::upper_bound( m_buckets.begin(), m_buckets.end(), max_bucket );

	return index_pair_t( itMin - m_buckets.begin(), itMax - m_buckets.begin() );
}

size_t DataIndex::count() const
{
	return m_entities.size();
}

const entity_t * DataIndex::entities() const
{
	return m_entities.data();
}

const bucket_t * DataIndex::buckets() const
{
	return m_buckets.data();
}

index_t DataIndex::create( entity_t id, bucket_t bucket )
{
	index_t idx = findById( id );
	if( idx < m_entities.size() )
		throw std::logic_error( "Trying to create existing component" );

	index_t target_idx = std::upper_bound( m_buckets.begin(), m_buckets.end(), bucket ) - m_buckets.begin();

	m_index[id] = idx;
	m_entities.push_back( id );
	m_buckets.push_back( bucket );

	index_t swap_idx = target_idx;
	while( swap_idx < idx )
	{
		swap( swap_idx, idx );
		swap_idx = std::upper_bound( m_buckets.begin(), m_buckets.end()-1, m_buckets.back() ) - m_buckets.begin();
	}

	return target_idx;
}

void DataIndex::destroy( index_t idx )
{
	if( idx >= m_entities.size() )
		throw std::logic_error( "Trying to remove nonexistant component" );

	swap( idx, m_entities.size()-1 );

	index_t swap_idx = std::lower_bound( m_buckets.begin() + idx + 1, m_buckets.end()-1, m_buckets[idx] ) - m_buckets.begin() - 1;
	while( swap_idx > idx )
	{
		swap( idx, swap_idx );
		swap_idx = std::lower_bound( m_buckets.begin() + idx + 1, m_buckets.end()-1, m_buckets[idx] ) - m_buckets.begin() - 1;
	}

	m_index.erase( m_entities.back() );
	m_entities.pop_back();
	m_buckets.pop_back();
}

size_t DataIndex::swapCount() const
{
	return m_swaps.size();
}

const index_pair_t * DataIndex::swaps() const
{
	return m_swaps.data();
}

void DataIndex::clearSwaps()
{
	m_swaps.clear();
}

void DataIndex::swap( index_t a, index_t b )
{
	entity_t id1 = m_entities[a];
	entity_t id2 = m_entities[b];

	m_entities[b] = id1;
	m_entities[a] = id2;

	m_index[id1] = b;
	m_index[id2] = a;

	std::swap( m_buckets[a], m_buckets[b] );

	m_swaps.emplace_back( a, b );
}
