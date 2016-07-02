
#pragma once

#include <unordered_map>
#include "DSimCommon.h"

namespace DSim {

class DataIndex
{
public:
	// Access
	size_t findById( uuid_t id, size_t hint = INVALID_INDEX ) const;
	swap_t findByGroup( group_t min_group, group_t max_group ) const;
	size_t size() const;
	const uuid_t * entities() const;
	const group_t * groups() const;

	// Modify
	size_t create( uuid_t id, group_t group = 0 );
	void destroy( size_t idx );

	// Swaps
	size_t swapCount() const;
	const swap_t * swapData() const;
	void clearSwaps();

private:
	std::unordered_map<uuid_t,size_t> m_index;
	std::vector<uuid_t>  m_entities;
	std::vector<group_t> m_groups;
	std::vector<swap_t>  m_swaps;

	void swap( size_t a, size_t b );
};

} // namespace DSim
