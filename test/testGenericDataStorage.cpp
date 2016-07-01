
#include "catch.hpp"

#include "GenericDataStorage.h"

SCENARIO( "Working with generic data storage" )
{
	auto type = std::make_unique<DSim::SimpleDataType<float>>();

	GIVEN( "Default constructed data storage" )
	{
		DSim::GenericDataStorage storage(type.get());

		THEN( "It should have given data type" ) {
			REQUIRE( storage.type() == type.get() );
		}
		THEN( "It should have no elements" ) {
			REQUIRE( storage.count() == 0 );
		}
	}

}
