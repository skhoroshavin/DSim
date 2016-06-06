
#pragma once

#include <stdlib.h>
#include <memory>

class CDataBlock
{
	struct Config
	{
		size_t size;
		size_t align;

		void (*init)( void * pDst, int nCount );
		void (*move)( void * pSrc, void * pDst, int nCount );
		void (*swap)( void * p1, void * p2 );
		void (*done)( void * pDst, int nCount );
	};

public:
	// Typedefs
	using UPtr = std::unique_ptr<CDataBlock>;

	// Construction
	CDataBlock() = default;
	inline CDataBlock( CDataBlock&& other ) { assign( std::move(other) ); }
	inline ~CDataBlock() { reset(); }
	inline CDataBlock& operator=( CDataBlock&& other ) { reset(); assign( std::move(other) ); return *this; }
	CDataBlock( const CDataBlock& other ) = delete;
	CDataBlock& operator=( const CDataBlock& other ) = delete;

	// Data access
	inline size_t count() const { return m_nItemCount; }
	inline size_t capacity() const { return m_nItemCapacity; }
	inline const void * data( size_t i ) const { return reinterpret_cast<const char*>(m_pData) + i*m_pConfig->size; }
	inline void * data( size_t i ) { return reinterpret_cast<char*>(m_pData) + i*m_pConfig->size; }

	// Data modification
	void   push( size_t nCount = 1 );
	size_t pop( size_t nCount = 1 );
	void   swap( size_t i, size_t j );
	void   clear();

	// Memory management
	void reserve( int nCount );
	void reset();

protected:
	void setConfig( const Config * pConfig );

private:
	const Config * m_pConfig = 0;
	size_t m_nItemCount      = 0;
	size_t m_nItemCapacity   = 0;
	void * m_pData           = 0;

	void assign( CDataBlock&& other );
};

template<typename T> class CTypedDataBlock : public CDataBlock
{
public:
	CTypedDataBlock()
	{
		static const Config s_Config =
		{
			sizeof(T),
			alignof(T),
			&initImpl,
			&moveImpl,
			&swapImpl,
			&doneImpl
		};
		setConfig( &s_Config );
	}

	// Data access
	inline T& operator[]( size_t i ) { return *reinterpret_cast<T*>(data(i)); }
	inline const T& operator[]( size_t i ) const { return *reinterpret_cast<T*>(data(i)); }

private:

	static void initImpl( void * pDst, int nCount )
	{
		auto dstBegin = reinterpret_cast<T*>(pDst);
		std::uninitialized_fill_n( dstBegin, nCount, T() );
	}
	static void moveImpl( void * pSrc, void * pDst, int nCount )
	{
		auto srcBegin = std::make_move_iterator(reinterpret_cast<T*>(pSrc));
		auto dstBegin = reinterpret_cast<T*>(pDst);
		std::uninitialized_copy_n( srcBegin, nCount, dstBegin );
	}
	static void swapImpl( void * p1, void * p2 )
	{
		std::swap( *reinterpret_cast<T*>(p1), *reinterpret_cast<T*>(p2) );
	}
	static void doneImpl( void * pDst, int nCount )
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
