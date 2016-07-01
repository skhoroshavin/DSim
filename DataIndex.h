
#pragma once

#include <unordered_map>
#include "DSimCommon.h"

namespace DSim {

class DataIndex
{
public:
	// Access
	index_t findById( uuid_t id, index_t hint = INVALID_INDEX ) const;
	swap_t findByGroup( group_t min_group, group_t max_group ) const;
	size_t count() const;
	const uuid_t * entities() const;
	const group_t * groups() const;

	// Modify
	index_t create( uuid_t id, group_t group = 0 );
	void destroy( index_t idx );

	// Swaps
	size_t swapCount() const;
	const swap_t * swaps() const;
	void clearSwaps();

private:
	std::unordered_map<uuid_t,index_t> m_index;
	std::vector<uuid_t>   m_entities;
	std::vector<group_t>  m_groups;
	std::vector<swap_t>   m_swaps;

	void swap( index_t a, index_t b );
};

} // namespace DSim
