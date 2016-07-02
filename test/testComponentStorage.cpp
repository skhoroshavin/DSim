
#include "catch.hpp"
#include "fakeit.hpp"
using namespace fakeit;

#include "ComponentStorage.h"

SCENARIO( "Initializing component storage data scheme" )
{
	GIVEN( "Default constructed storage" )
	{
		DSim::ComponentStorage storage;

		THEN( "It should contain no fields" ) {
			REQUIRE( storage.fieldCount() == 0 );
		}

		THEN( "It should contain no elements" ) {
			REQUIRE( storage.size() == 0 );
		}

		WHEN( "Adding some fields" )
		{
			auto type1 = std::make_unique<DSim::SimpleDataType<float>>();
			auto type2 = std::make_unique<DSim::SimpleDataType<double>>();
			auto type3 = std::make_unique<DSim::SimpleDataType<double>>();
			auto idx1 = storage.addField( type1.get() );
			auto idx2 = storage.addField( type2.get() );
			auto idx3 = storage.addField( type3.get() );
			THEN( "Field count should increase accordingly" ) {
				REQUIRE( storage.fieldCount() == 3 );
			}
			THEN( "Their indices should be sequential starting with 0" ) {
				REQUIRE( idx1 == 0 );
				REQUIRE( idx2 == 1 );
				REQUIRE( idx3 == 2 );
			}
			THEN( "They should have given data types ") {
				REQUIRE( storage.fieldType( idx1 ) == type1.get() );
				REQUIRE( storage.fieldType( idx2 ) == type2.get() );
				REQUIRE( storage.fieldType( idx3 ) == type3.get() );
			}
		}
	}
}

SCENARIO( "Adding/removing component storage listeners" )
{
	fakeit::Mock<DSim::IComponentListener> mock;
	fakeit::Mock<DSim::IComponentListener> mock2;

	DSim::IComponentListener * listener = &mock.get();
	DSim::IComponentListener * listener2 = &mock2.get();

	GIVEN( "Default constructed storage" )
	{
		DSim::ComponentStorage storage;

		THEN( "It should be possible to add listener" )
		{
			REQUIRE_NOTHROW( storage.addListener( listener ) );

			AND_THEN( "It should be possible to remove it" ) {
				REQUIRE_NOTHROW( storage.removeListener( listener ) );

				AND_THEN( "It shouldn't be possible to remove it again" ) {
					REQUIRE_THROWS( storage.removeListener( listener ) );
				}
			}

			AND_THEN( "It shouldn't be possible to add it again" ) {
				REQUIRE_THROWS( storage.addListener( listener ) );
				storage.removeListener( listener );
			}

			AND_THEN( "It should be possible to add another" ) {
				REQUIRE_NOTHROW( storage.addListener( listener2 ) );
				storage.removeListener( listener );
				storage.removeListener( listener2 );
			}
		}
	}
}

SCENARIO( "Working with component storage" )
{
	DSim::ComponentStorage storage;
	auto type1 = std::make_unique<DSim::SimpleDataType<float>>();
	auto type2 = std::make_unique<DSim::SimpleDataType<double>>();
	storage.addField( type1.get() );
	storage.addField( type2.get() );

	fakeit::Mock<DSim::IComponentListener> mock;
	When(Method(mock,componentsCreated)).AlwaysReturn();
	When(Method(mock,componentsReordered)).AlwaysReturn();
	When(Method(mock,componentsDestroyed)).AlwaysReturn();
	storage.addListener( &mock.get() );

	GIVEN( "Empty storage with some fields" )
	{
		THEN( "It should contain no elements" ) {
			REQUIRE( storage.size() == 0 );
		}

		WHEN( "Adding element" ) {
			auto idx = storage.create( 1 );
			THEN( "Storage size should be increased" ) {
				REQUIRE( storage.size() == 1 );
			}
			THEN( "It should have valid index" ) {
				REQUIRE( idx < storage.size() );
			}
			THEN( "It should be searchable" ) {
				REQUIRE( storage.find(1) == idx );
			}
			THEN( "It should be destroyable" ) {
				REQUIRE_NOTHROW( storage.destroy(idx) );
			}
			THEN( "It should emit component added event" ) {
				VERIFY(Method(mock,componentsCreated).Using(1));
			}
		}
	}

	GIVEN( "Storage with some data" )
	{
		storage.create( 1 );
		storage.create( 2 );
		storage.create( 3 );

		THEN( "Elements should have valid indices" ) {
			REQUIRE( storage.find(1) < storage.size() );
			REQUIRE( storage.find(2) < storage.size() );
			REQUIRE( storage.find(3) < storage.size() );
		}
		THEN( "Elements should have unique indices" ) {
			REQUIRE( storage.find(1) != storage.find(2) );
			REQUIRE( storage.find(2) != storage.find(3) );
		}
		THEN( "It should fail to create existing element" ) {
			REQUIRE_THROWS( storage.create(3) );
		}
		THEN( "It should fail to add fields" ) {
			auto type = std::make_unique<DSim::SimpleDataType<float>>();
			REQUIRE_THROWS( storage.addField( type.get() ) );
		}

		WHEN( "Trying to find nonexistant element" ) {
			auto idx = storage.find( 4 );
			THEN( "Index should be invalid" ) {
				REQUIRE( idx >= storage.size() );
				AND_THEN( "It should fail to destroy it" ) {
					REQUIRE_THROWS( storage.destroy(idx) );
				}
			}
		}

		WHEN( "Destroying existing element" )
		{
			auto idx = storage.find( 2 );
			storage.destroy( idx );

			THEN( "It should emit component destroyed event" ) {
				VERIFY(Method(mock,componentsDestroyed).Using(1));
			}
			THEN( "It should no longer be searchable" ) {
				REQUIRE( storage.find(2) >= storage.size() );
			}
		}
	}

	storage.removeListener( &mock.get() );
}
