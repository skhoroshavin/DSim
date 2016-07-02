
#include "catch.hpp"

#include "GenericDataStorage.h"

typedef std::string TestType;
typedef DSim::SimpleDataType<TestType> TestDataType;

SCENARIO( "Working with generic data storage" )
{
	TestDataType type;
	DSim::GenericDataStorage storage(&type);

	GIVEN( "Default constructed data storage" )
	{
		THEN( "It should have given data type" ) {
			REQUIRE( storage.type() == &type );
		}
		THEN( "It should be empty" ) {
			REQUIRE( storage.size() == 0 );
			REQUIRE( storage.capacity() == 0 );
		}
		THEN( "It should fail to pop elements" ) {
			REQUIRE_THROWS( storage.pop(3) );
		}
		THEN( "It shouldn't fail when clearing" ) {
			REQUIRE_NOTHROW( storage.clear() );
		}
		THEN( "It shouldn't fail when resetting" ) {
			REQUIRE_NOTHROW( storage.reset() );
		}

		WHEN( "Pushing some elements" )
		{
			storage.push( 3 );

			THEN( "It should contain new elements" ) {
				REQUIRE( storage.size() == 3 );
				REQUIRE( storage.capacity() >= 3 );
			}
			THEN( "Memory should be properly aligned" ) {
				REQUIRE( reinterpret_cast<uintptr_t>(storage.data()) % type.alignment() == 0 );
			}
			THEN( "Elements should be default constructed" ) {
				REQUIRE( *static_cast<TestType*>(storage.data(0)) == "" );
				REQUIRE( *static_cast<TestType*>(storage.data(1)) == "" );
				REQUIRE( *static_cast<TestType*>(storage.data(2)) == "" );
			}
		}

		WHEN( "Reserving data" )
		{
			storage.reserve( 5 );

			THEN( "It should remain empty" ) {
				REQUIRE( storage.size() == 0 );
			}
			THEN( "It should have data reserved" ) {
				REQUIRE( storage.capacity() >= 5 );
			}
		}
	}

	GIVEN( "Data storage with some elements" )
	{
		storage.push( 5 );
		*static_cast<TestType*>(storage.data(0)) = "A";
		*static_cast<TestType*>(storage.data(1)) = "B";
		*static_cast<TestType*>(storage.data(2)) = "C";
		*static_cast<TestType*>(storage.data(3)) = "D";
		*static_cast<TestType*>(storage.data(4)) = "E";

		WHEN( "Pushing some elements" )
		{
			storage.push( 2 );

			THEN( "It should contain new elements" ) {
				REQUIRE( storage.size() == 7 );
				REQUIRE( storage.capacity() >= 7 );
			}
			THEN( "New elements should be default constructed" ) {
				REQUIRE( *static_cast<TestType*>(storage.data(5)) == "" );
				REQUIRE( *static_cast<TestType*>(storage.data(6)) == "" );
			}
			THEN( "Old elements should be intact" ) {
				REQUIRE( *static_cast<TestType*>(storage.data(0)) == "A" );
				REQUIRE( *static_cast<TestType*>(storage.data(1)) == "B" );
				REQUIRE( *static_cast<TestType*>(storage.data(2)) == "C" );
				REQUIRE( *static_cast<TestType*>(storage.data(3)) == "D" );
				REQUIRE( *static_cast<TestType*>(storage.data(4)) == "E" );
			}
		}

		WHEN( "Popping some elements" )
		{
			storage.pop( 2 );

			THEN( "It should contain less elements" ) {
				REQUIRE( storage.size() == 3 );
			}
			THEN( "It should not reduce capacity" ) {
				REQUIRE( storage.capacity() >= 5 );
			}
			THEN( "Old elements should be intact" ) {
				REQUIRE( *static_cast<TestType*>(storage.data(0)) == "A" );
				REQUIRE( *static_cast<TestType*>(storage.data(1)) == "B" );
				REQUIRE( *static_cast<TestType*>(storage.data(2)) == "C" );
			}
		}

		WHEN( "Reordering elements" )
		{
			const DSim::swap_t pairs[] =
			{
				{ 0, 2 },	// C B A D E
				{ 2, 4 },	// C B E D A
				{ 3, 0 }    // D B E C A
			};
			storage.reorder( pairs, 3 );

			THEN( "Elements should be reordered" )
			{
				REQUIRE( *static_cast<TestType*>(storage.data(0)) == "D" );
				REQUIRE( *static_cast<TestType*>(storage.data(1)) == "B" );
				REQUIRE( *static_cast<TestType*>(storage.data(2)) == "E" );
				REQUIRE( *static_cast<TestType*>(storage.data(3)) == "C" );
				REQUIRE( *static_cast<TestType*>(storage.data(4)) == "A" );
			}
		}

		WHEN( "Clearing" )
		{
			storage.clear();

			THEN( "It becomes empty" ) {
				REQUIRE( storage.size() == 0 );
			}
			THEN( "It retains capacity" ) {
				REQUIRE( storage.capacity() >= 3 );
			}
			THEN( "It retains data type" ) {
				REQUIRE( storage.type() == &type );
			}
		}

		WHEN( "Resetting" )
		{
			storage.reset();

			THEN( "It becomes empty" ) {
				REQUIRE( storage.size() == 0 );
				REQUIRE( storage.capacity() == 0 );
			}
			THEN( "It retains data type" ) {
				REQUIRE( storage.type() == &type );
			}
		}
	}


}
