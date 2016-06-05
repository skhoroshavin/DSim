
#include "DataBlock.h"

void CDataBlock::push( size_t nCount )
{
	if( m_nItemCount + nCount < m_nItemReserve )
		reserve( 1.5*(m_nItemCount + nCount) );

	initImpl( data(m_nItemCount), nCount );
	m_nItemCount += nCount;
}

void CDataBlock::pop( size_t nCount )
{
	if( nCount > m_nItemCount )
		nCount = m_nItemCount;
	if( !nCount ) return;

	m_nItemCount -= nCount;
	doneImpl( data(m_nItemCount), nCount );
}

void CDataBlock::swap( size_t i, size_t j )
{
	swapImpl( data(i), data(j) );
}

void CDataBlock::clear()
{
	doneImpl( m_pData, m_nItemCount );
	m_nItemCount = 0;
}

void CDataBlock::reserve( int nCount )
{
	if( nCount <= m_nItemReserve ) return;

	void * pNewData = aligned_alloc( m_nAlignment, nCount*m_nItemSize );
	if( !pNewData )
		throw std::runtime_error( "CDataBlock: Out of memory" );
	if( m_pData )
	{
		moveImpl( m_pData, pNewData, m_nItemCount );
		doneImpl( m_pData, m_nItemCount );
		free( m_pData );
	}
	m_pData = pNewData;
	m_nItemReserve = nCount;
}

void CDataBlock::reset()
{
	clear();
	if( m_pData )
	{
		free( m_pData );
		m_nItemReserve = 0;
	}
}

CDataBlock::CDataBlock(size_t align, size_t itemSize) :
	m_nAlignment(align), m_nItemSize(itemSize)
{

}

void CDataBlock::assign(CDataBlock &&other)
{
	m_nAlignment   = other.m_nAlignment;
	m_nItemSize    = other.m_nItemSize;
	m_nItemCount   = other.m_nItemCount;
	m_nItemReserve = other.m_nItemReserve;
	m_pData        = other.m_pData;

	other.m_nItemCount   = 0;
	other.m_nItemReserve = 0;
	other.m_pData        = 0;
}
