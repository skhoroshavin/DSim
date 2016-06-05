
#pragma once

#include <stdlib.h>
#include <memory>

class CDataBlock
{
public:
	// Typedefs
	using UPtr = std::unique_ptr<CDataBlock>;

	// Construction
	inline CDataBlock( CDataBlock&& other ) { assign( std::move(other) ); }
	inline ~CDataBlock() { reset(); }
	inline CDataBlock& operator=( CDataBlock&& other ) { reset(); assign( std::move(other) ); return *this; }
	CDataBlock( const CDataBlock& other ) = delete;
	CDataBlock& operator=( const CDataBlock& other ) = delete;

	// Data access
	inline size_t count() const { return m_nItemCount; }
	inline const void * data( size_t i ) const { return reinterpret_cast<const char*>(m_pData) + i*m_nItemSize; }
	inline void * data( size_t i ) { return reinterpret_cast<char*>(m_pData) + i*m_nItemSize; }

	// Data modification
	void push( size_t nCount = 1 );
	void pop( size_t nCount = 1 );
	void swap( size_t i, size_t j );
	void clear();

	// Memory management
	void reserve( int nCount );
	void reset();

protected:
	CDataBlock( size_t align, size_t itemSize );

	virtual void initImpl( void * pDst, int nCount ) const = 0;
	virtual void moveImpl( void * pSrc, void * pDst, int nCount ) const = 0;
	virtual void swapImpl( void * p1, void * p2 ) const = 0;
	virtual void doneImpl( void * pDst, int nCount ) const = 0;

private:
	size_t m_nAlignment;
	size_t m_nItemSize;

	size_t m_nItemCount   = 0;
	size_t m_nItemReserve = 0;
	void * m_pData        = 0;

	void assign( CDataBlock&& other );
};

template<typename T> class CTypedDataBlock : public CDataBlock
{
public:
	CTypedDataBlock() : CDataBlock(alignof(T),sizeof(T)) { }

	// Data access
	inline T& operator[]( size_t i ) { return *reinterpret_cast<T*>(data(i)); }
	inline const T& operator[]( size_t i ) const { return *reinterpret_cast<T*>(data(i)); }

protected:
	void initImpl( void * pDst, int nCount ) const
	{
		auto dstBegin = reinterpret_cast<T*>(pDst);
		std::uninitialized_fill_n( dstBegin, nCount, T() );
	}
	void moveImpl( void * pSrc, void * pDst, int nCount ) const
	{
		auto srcBegin = std::make_move_iterator(reinterpret_cast<T*>(pSrc));
		auto dstBegin = reinterpret_cast<T*>(pDst);
		std::uninitialized_copy_n( srcBegin, nCount, dstBegin );
	}
	void swapImpl( void * p1, void * p2 ) const
	{
		std::swap( *reinterpret_cast<T*>(p1), *reinterpret_cast<T*>(p2) );
	}
	void doneImpl( void * pDst, int nCount ) const
	{
		auto dstBegin = reinterpret_cast<T*>(pDst);
		auto dstEnd = dstBegin + nCount;
		while( dstBegin != dstEnd )
		{
			dstBegin->~T();
			++dstBegin;
		}
	}
};
