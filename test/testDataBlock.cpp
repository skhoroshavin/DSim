
#include "catch.hpp"

#include "DataBlock.h"

SCENARIO( "working with datablocks")
{
	typedef std::string DataElem;
	typedef CTypedDataBlock<DataElem> DataBlock;


	GIVEN( "empty datablock" )
	{
		DataBlock db;

		REQUIRE( db.capacity() == 0 );
		REQUIRE( db.count() == 0 );

		WHEN( "pushing one element" )
		{
			db.push();

			THEN( "one element is created" )
			{
				REQUIRE( db.capacity() >= 1 );
				REQUIRE( db.count() == 1 );
			}
			AND_THEN( "element is default constructed" )
			{
				REQUIRE( db[0] == DataElem() );
			}
		}
		WHEN( "pushing five elements" )
		{
			db.push( 5 );

			THEN( "five elements are created" )
			{
				REQUIRE( db.capacity() >= 5 );
				REQUIRE( db.count() == 5 );
			}
			AND_THEN( "all elements are default constructed" )
			{
				for( int i = 0; i < 5; ++i )
					REQUIRE( db[i] == DataElem() );
			}
		}
		WHEN( "popping one element" )
		{
			size_t nPopped = db.pop();

			THEN( "datablock remains empty" )
			{
				REQUIRE( nPopped == 0 );
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() == 0 );
			}
		}
		WHEN( "clearing" )
		{
			db.clear();

			THEN( "datablock remains empty" )
			{
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() == 0 );
			}
		}
		WHEN( "resetting" )
		{
			db.reset();

			THEN( "datablock remains empty" )
			{
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() == 0 );
			}
		}
	}


	GIVEN( "datablock with 3 elements" )
	{
		DataBlock db;
		db.push( 3 );

		DataElem a = "AAA", b = "BBB", c = "CCC";
		db[0] = a;
		db[1] = b;
		db[2] = c;

		REQUIRE( db.capacity() >= 3 );
		REQUIRE( db.count() == 3 );
		REQUIRE( db[0] == a );
		REQUIRE( db[1] == b );
		REQUIRE( db[2] == c );

		WHEN( "pushing another 4 elements" )
		{
			db.push( 4 );

			THEN( "4 elements are added" )
			{
				REQUIRE( db.capacity() >= 7 );
				REQUIRE( db.count() == 7 );
			}
			AND_THEN( "new elements are default constructed" )
			{
				REQUIRE( db[4] == DataElem() );
				REQUIRE( db[5] == DataElem() );
				REQUIRE( db[6] == DataElem() );
			}
			AND_THEN( "old elements are intact" )
			{
				REQUIRE( db[0] == a );
				REQUIRE( db[1] == b );
				REQUIRE( db[2] == c );
			}
		}

		WHEN( "popping 2 elements" )
		{
			int nPopped = db.pop(2);

			THEN( "2 elements are removed" )
			{
				REQUIRE( nPopped == 2 );
				REQUIRE( db.count() == 1 );
			}
			AND_THEN( "capacity is not changed" )
			{
				REQUIRE( db.capacity() >= 3 );
			}
			AND_THEN( "remaining element is intact" )
			{
				REQUIRE( db[0] == a );
			}
		}

		WHEN( "popping 5 elements" )
		{
			int nPopped = db.pop(5);

			THEN( "only 3 elements are removed" )
			{
				REQUIRE( nPopped == 3 );
				REQUIRE( db.count() == 0 );
			}
			AND_THEN( "capacity is not changed" )
			{
				REQUIRE( db.capacity() >= 3 );
			}
		}

		WHEN( "swapping two elements" )
		{
			db.swap( 0, 2 );

			THEN( "they are really swapped" )
			{
				REQUIRE( db[0] == c );
				REQUIRE( db[2] == a );
			}
			AND_THEN( "others are intact" )
			{
				REQUIRE( db[1] == b );
			}
		}

		WHEN( "moving to another location" )
		{
			DataBlock db2 = std::move(db);

			THEN( "old datablock becomes empty" )
			{
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() == 0 );
			}
			AND_THEN( "new datablock contains all data from old datablock" )
			{
				REQUIRE( db2.capacity() >= 3 );
				REQUIRE( db2.count() == 3 );
				REQUIRE( db2[0] == a );
				REQUIRE( db2[1] == b );
				REQUIRE( db2[2] == c );
			}
		}

		WHEN( "clearing" )
		{
			db.clear();

			THEN( "datablock becomes empty, retaining capacity" )
			{
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() >= 3 );
			}
		}

		WHEN( "resetting" )
		{
			db.reset();

			THEN( "datablock becomes empty and frees memory" )
			{
				REQUIRE( db.count() == 0 );
				REQUIRE( db.capacity() == 0 );
			}
		}

	}

}

