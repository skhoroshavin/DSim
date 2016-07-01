
#pragma once

#include <unordered_map>
#include "DSimCommon.h"

namespace DSim {

typedef uint32_t bucket_t;
typedef uint64_t entity_t;

class DataIndex
{
public:
	// Access
	index_t findById( entity_t id, index_t hint = -1 ) const;
	index_pair_t findByBucket( bucket_t min_bucket, bucket_t max_bucket ) const;
	size_t count() const;
	const entity_t * entities() const;
	const bucket_t * buckets() const;

	// Modify
	index_t create( entity_t id, bucket_t bucket = 0 );
	void destroy( index_t idx );

	// Swaps
	size_t swapCount() const;
	const index_pair_t * swaps() const;
	void clearSwaps();

private:
	std::unordered_map<entity_t,index_t> m_index;
	std::vector<entity_t>     m_entities;
	std::vector<bucket_t>     m_buckets;
	std::vector<index_pair_t> m_swaps;

	void swap( index_t a, index_t b );
};

} // namespace DSim
