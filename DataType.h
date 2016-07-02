
#pragma once

#include <stddef.h>
#include <new>
#include <stdexcept>
#include <utility>

#include "DSimCommon.h"

namespace DSim {

class IDataType
{
public:
	virtual ~IDataType() = default;

	virtual size_t size() const = 0;
	virtual size_t alignment() const = 0;

	virtual void init( void * data, size_t stride, size_t count ) const = 0;
	virtual void move( void * src, void * dst, size_t stride, size_t count ) const = 0;
	virtual void reorder( void * data, size_t stride, const swap_t * pairs, size_t pair_count ) const = 0;
	virtual void done( void * data, size_t stride, size_t count ) const = 0;
};

template<typename T>
class SimpleDataType : public IDataType
{
public:
	size_t size() const override { return sizeof(T); }
	size_t alignment() const override { return alignof(T); }

	void init( void * data, size_t stride, size_t count ) const final
	{
		checkPointer( data );
		checkStride( stride );

		for( size_t i = 0; i < count; ++i )
			new (&item(data,i*stride)) T();
	}

	void move( void * src, void * dst, size_t stride, size_t count ) const final
	{
		checkPointer( src );
		checkPointer( dst );
		checkStride( stride );

		for( size_t i = 0; i < count; ++i )
			item(dst, i*stride) = std::move( item(src, i*stride) );
	}

	void reorder( void * data, size_t stride, const swap_t * pairs, size_t pairCount ) const final
	{
		checkPointer( data );
		checkStride( stride );

		for( size_t i = 0; i < pairCount; ++i )
			std::swap( item(data, stride*pairs[i].first),
			           item(data, stride*pairs[i].second) );
	}

	void done( void * data, size_t stride, size_t count ) const final
	{
		checkPointer( data );
		checkStride( stride );

		for( size_t i = 0; i < count; ++i )
			item(data,i*stride).~T();
	}

private:
	inline static void checkPointer( void * ptr )
	{
		DSIM_ASSERT( reinterpret_cast<uintptr_t>(ptr) % alignof(T) == 0, "Pointer is not properly aligned" );
	}

	inline static void checkStride( size_t stride )
	{
		DSIM_ASSERT( stride >= sizeof(T), "Stride is less than data size" );
		DSIM_ASSERT( stride % alignof(T) == 0, "Stride is not multiple of data alignment" );
	}

	inline static T& item( void * data, int offset )
	{
		data = static_cast<char*>(data) + offset;
		return *static_cast<T*>( data );
	}
};

} // namespace DSim
