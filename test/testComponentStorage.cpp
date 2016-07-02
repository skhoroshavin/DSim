
#include "catch.hpp"
#include "fakeit.hpp"
using namespace fakeit;

#include "ComponentStorage.h"
using DSim::UUID;

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
			DSim::SimpleDataType<float> type1;
			DSim::SimpleDataType<int>   type2;
			DSim::SimpleDataType<float> type3;
			auto idx1 = storage.addField( &type1 );
			auto idx2 = storage.addField( &type2 );
			auto idx3 = storage.addField( &type3 );
			THEN( "Field count should increase accordingly" ) {
				REQUIRE( storage.fieldCount() == 3 );
			}
			THEN( "Their indices should be sequential starting with 0" ) {
				REQUIRE( idx1 == 0 );
				REQUIRE( idx2 == 1 );
				REQUIRE( idx3 == 2 );
			}
			THEN( "They should have given data types ") {
				REQUIRE( storage.fieldType( idx1 ) == &type1 );
				REQUIRE( storage.fieldType( idx2 ) == &type2 );
				REQUIRE( storage.fieldType( idx3 ) == &type3 );
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

namespace
{
DSim::SimpleDataType<float> g_floatType;
DSim::SimpleDataType<int>   g_intType;
}

class TestComponentStorage : public DSim::ComponentStorage
{
public:
	TestComponentStorage()
	{
		addField( &g_floatType  );
		addField( &g_intType );
	}

	inline float * floatData() { return static_cast<float*>( data(0) ); }
	inline int *   intData()   { return static_cast<int*>( data(1) );   }
};

SCENARIO( "Working with component storage" )
{
	TestComponentStorage storage;

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
			auto idx = storage.create( UUID(1) );
			THEN( "Storage size should be increased" ) {
				REQUIRE( storage.size() == 1 );
			}
			THEN( "It should have valid index" ) {
				REQUIRE( idx < storage.size() );
			}
			THEN( "It should be default constructed" ) {
				REQUIRE( storage.floatData()[idx] == 0.0f );
				REQUIRE( storage.intData()[idx] == 0 );
			}
			THEN( "It should be searchable" ) {
				REQUIRE( storage.find(UUID(1)) == idx );
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
		for( size_t i = 1; i <= 3; ++i )
		{
			auto idx = storage.create( UUID(i) );
			storage.floatData()[idx] = 0.1f * i;
			storage.intData()[idx] = int(i);
		}

		THEN( "Elements should have valid indices" ) {
			REQUIRE( storage.find(UUID(1)) < storage.size() );
			REQUIRE( storage.find(UUID(2)) < storage.size() );
			REQUIRE( storage.find(UUID(3)) < storage.size() );
		}
		THEN( "Elements should have valid data" ) {
			for( size_t i = 1; i <= 3; ++i )
			{
				auto idx = storage.find( UUID(i) );
				REQUIRE( storage.floatData()[idx] == Approx(0.1f * i) );
				REQUIRE( storage.intData()[idx] == int(i) );
			}
		}
		THEN( "Elements should have unique indices" ) {
			REQUIRE( storage.find(UUID(1)) != storage.find(UUID(2)) );
			REQUIRE( storage.find(UUID(2)) != storage.find(UUID(3)) );
		}
		THEN( "Elements should have corresponding data" ) {
		}
		THEN( "It should fail to create existing element" ) {
			REQUIRE_THROWS( storage.create(UUID(3)) );
		}
		THEN( "It should fail to add fields" ) {
			DSim::SimpleDataType<float> floatType;
			REQUIRE_THROWS( storage.addField( &floatType ) );
		}

		WHEN( "Trying to find nonexistant element" ) {
			auto idx = storage.find( UUID(4) );
			THEN( "Index should be invalid" ) {
				REQUIRE( idx >= storage.size() );
				AND_THEN( "It should fail to destroy it" ) {
					REQUIRE_THROWS( storage.destroy(idx) );
				}
			}
		}

		WHEN( "Destroying existing element" )
		{
			auto idx = storage.find( UUID(2) );
			storage.destroy( idx );

			THEN( "It should emit component destroyed event" ) {
				VERIFY(Method(mock,componentsDestroyed).Using(1));
			}
			THEN( "It should no longer be searchable" ) {
				REQUIRE( storage.find( UUID(2) ) >= storage.size() );
			}
		}
	}

	storage.removeListener( &mock.get() );
}
