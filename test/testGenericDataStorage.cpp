
#include "catch.hpp"

#include "GenericDataStorage.h"

SCENARIO( "Working with generic data storage" )
{
	DSim::SimpleDataType<float> type;

	GIVEN( "Default constructed data storage" )
	{
		DSim::GenericDataStorage storage(&type);

		THEN( "It should have given data type" ) {
			REQUIRE( storage.type() == &type );
		}
		THEN( "It should have no elements" ) {
			REQUIRE( storage.count() == 0 );
		}
	}

}
