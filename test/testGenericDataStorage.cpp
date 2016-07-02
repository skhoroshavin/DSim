
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
		THEN( "It should be empty " ) {
			REQUIRE( storage.size() == 0 );
			REQUIRE( storage.capacity() == 0 );
		}
		THEN( "It should fail to pop elements" ) {
			REQUIRE_THROWS( storage.pop(3) );
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

		WHEN( "Clearing" )
		{
			storage.clear();

			THEN( "Data storage remains empty" ) {
				REQUIRE( storage.size() == 0 );
				REQUIRE( storage.capacity() == 0 );
			}
			THEN( "Data type should be retained" ) {
				REQUIRE( storage.type() == &type );
			}
		}

		WHEN( "Resetting" )
		{
			storage.reset();

			THEN( "Data storage remains empty" ) {
				REQUIRE( storage.size() == 0 );
				REQUIRE( storage.capacity() == 0 );
			}
			THEN( "Data type should be retained" ) {
				REQUIRE( storage.type() == &type );
			}
		}
	}

	GIVEN( "Data storage with some elements" )
	{
		storage.push( 3 );
		static_cast<int*>(storage.data())[0] = 1;
	}


}
