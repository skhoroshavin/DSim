
#include "catch.hpp"

#include "DataBlock.h"

SCENARIO( "working with datablocks")
{
	typedef int DataElem;
	typedef CTypedDataBlock<DataElem> DataBlock;

	GIVEN( "empty datablock" )
	{
		DataBlock db;

		REQUIRE( db.capacity() == 0 );
		REQUIRE( db.count() == 0 );

		WHEN( "pushing one element" )
		{
			db.push();

			THEN( "one element is created with default constructor" )
			{
				REQUIRE( db.capacity() >= 1 );
				REQUIRE( db.count() == 1 );
				REQUIRE( db[0] == DataElem() );
			}
		}
	}

}

