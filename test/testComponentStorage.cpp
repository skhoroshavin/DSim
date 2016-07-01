
#include "catch.hpp"

#include "ComponentStorage.h"

SCENARIO( "Working with component storage" )
{
	GIVEN( "Default constructed storage" )
	{
		DSim::ComponentStorage storage;

		THEN( "It should contain no fields" ) {
			REQUIRE( storage.fieldCount() == 0 );
		}

		THEN( "It should contain no elements" ) {
			REQUIRE( storage.count() == 0 );
		}

		WHEN( "Adding one field" )
		{
			auto type = std::make_unique<DSim::SimpleDataType<float>>();
			auto idx = storage.addField( type.get() );
			THEN( "Field count should become one" ) {
				REQUIRE( storage.fieldCount() == 1 );
			}
			THEN( "It's index should be 0" ) {
				REQUIRE( idx == 0 );
			}
			THEN( "It should have given data type" ) {
				REQUIRE( storage.fieldType(idx) == type.get() );
			}
		}

		WHEN( "Adding two fields" )
		{
			auto type1 = std::make_unique<DSim::SimpleDataType<float>>();
			auto type2 = std::make_unique<DSim::SimpleDataType<double>>();
			auto idx1 = storage.addField( type1.get() );
			auto idx2 = storage.addField( type2.get() );
			THEN( "Field count should become two" ) {
				REQUIRE( storage.fieldCount() == 2 );
			}
			THEN( "Their indices should be 0 and 1" )
			{
				REQUIRE( idx1 == 0 );
				REQUIRE( idx2 == 1 );
			}
			THEN( "They should have given data types ")
			{
				REQUIRE( storage.fieldType( idx1 ) == type1.get() );
				REQUIRE( storage.fieldType( idx2 ) == type2.get() );
			}
		}

		WHEN( "Trying to find nonexistant element" ) {
			auto idx = storage.find(1);
			THEN( "It should return invalid index ") {
				REQUIRE( idx == DSim::INVALID_INDEX );
			}
		}

	}
}
