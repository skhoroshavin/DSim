
#include "catch.hpp"

#include "GenericDataStorage.h"

class AlignedIntDataType : public DSim::SimpleDataType<int>
{
public:
	size_t size() const override;
	size_t alignment() const override;
};

size_t AlignedIntDataType::size() const { return 16; }
size_t AlignedIntDataType::alignment() const { return 16; }

SCENARIO( "Working with generic data storage" )
{
	AlignedIntDataType type;
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
				REQUIRE( *static_cast<int*>(storage.data(0)) == 0 );
				REQUIRE( *static_cast<int*>(storage.data(1)) == 0 );
				REQUIRE( *static_cast<int*>(storage.data(2)) == 0 );
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
		*static_cast<int*>(storage.data(0)) = 1;
		*static_cast<int*>(storage.data(1)) = 2;
		*static_cast<int*>(storage.data(2)) = 3;
		*static_cast<int*>(storage.data(3)) = 4;
		*static_cast<int*>(storage.data(4)) = 5;

		WHEN( "Pushing some elements" )
		{
			storage.push( 2 );

			THEN( "It should contain new elements" ) {
				REQUIRE( storage.size() == 7 );
				REQUIRE( storage.capacity() >= 7 );
			}
			THEN( "New elements should be default constructed" ) {
				REQUIRE( *static_cast<int*>(storage.data(5)) == 0 );
				REQUIRE( *static_cast<int*>(storage.data(6)) == 0 );
			}
			THEN( "Old elements should be intact" ) {
				REQUIRE( *static_cast<int*>(storage.data(0)) == 1 );
				REQUIRE( *static_cast<int*>(storage.data(1)) == 2 );
				REQUIRE( *static_cast<int*>(storage.data(2)) == 3 );
				REQUIRE( *static_cast<int*>(storage.data(3)) == 4 );
				REQUIRE( *static_cast<int*>(storage.data(4)) == 5 );
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
				REQUIRE( *static_cast<int*>(storage.data(0)) == 1 );
				REQUIRE( *static_cast<int*>(storage.data(1)) == 2 );
				REQUIRE( *static_cast<int*>(storage.data(2)) == 3 );
			}
		}

		WHEN( "Reordering elements" )
		{
			const DSim::swap_t pairs[] =
			{
				{ 0, 2 },	// 3 2 1 4 5
				{ 2, 4 },	// 3 2 5 4 1
				{ 3, 0 }    // 4 2 5 3 1
			};
			storage.reorder( pairs, 3 );

			THEN( "Elements should be reordered" )
			{
				REQUIRE( *static_cast<int*>(storage.data(0)) == 4 );
				REQUIRE( *static_cast<int*>(storage.data(1)) == 2 );
				REQUIRE( *static_cast<int*>(storage.data(2)) == 5 );
				REQUIRE( *static_cast<int*>(storage.data(3)) == 3 );
				REQUIRE( *static_cast<int*>(storage.data(4)) == 1 );
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
