
#include "DataBlock.h"

void CDataBlock::push( size_t nCount )
{
	if( m_nItemCount + nCount > m_nItemCapacity )
		reserve( 1.5*(m_nItemCount + nCount) );

	m_pConfig->init( data(m_nItemCount), nCount );
	m_nItemCount += nCount;
}

size_t CDataBlock::pop( size_t nCount )
{
	if( nCount > m_nItemCount )
		nCount = m_nItemCount;
	if( !nCount ) return 0;

	m_nItemCount -= nCount;
	m_pConfig->done( data(m_nItemCount), nCount );
	return nCount;
}

void CDataBlock::swap( size_t i, size_t j )
{
	m_pConfig->swap( data(i), data(j) );
}

void CDataBlock::clear()
{
	m_pConfig->done( m_pData, m_nItemCount );
	m_nItemCount = 0;
}

void CDataBlock::reserve( int nCount )
{
	if( nCount <= m_nItemCapacity ) return;

	void * pNewData = aligned_alloc( m_pConfig->align, nCount*m_pConfig->size );
	if( !pNewData )
		throw std::runtime_error( "CDataBlock: Out of memory" );
	if( m_pData )
	{
		m_pConfig->move( m_pData, pNewData, m_nItemCount );
		m_pConfig->done( m_pData, m_nItemCount );
		free( m_pData );
	}
	m_pData = pNewData;
	m_nItemCapacity = nCount;
}

void CDataBlock::reset()
{
	clear();
	if( m_pData )
	{
		free( m_pData );
		m_nItemCapacity = 0;
	}
}

void CDataBlock::setConfig( const Config * pConfig )
{
	m_pConfig = pConfig;
}

void CDataBlock::assign( CDataBlock &&other )
{
	m_pConfig = other.m_pConfig;
	m_nItemCount    = other.m_nItemCount;
	m_nItemCapacity = other.m_nItemCapacity;
	m_pData         = other.m_pData;

	other.m_nItemCount   = 0;
	other.m_nItemCapacity = 0;
	other.m_pData        = 0;
}
