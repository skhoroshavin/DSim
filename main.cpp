
#define CATCH_CONFIG_MAIN
#include "test/catch.hpp"

/*
#include "WorldManager.h"
#include <assert.h>

class TestComponentManager : public CComponentManager
{
public:
	TestComponentManager()
	{
		setID(1);
	}

};

class TestWorldManager : public CWorldManager
{
public:
	TestWorldManager()
	{
		registerComponent( TestComponentManager() );
	}
};

void testWorldManager()
{
	TestWorldManager world;

	// Test single entity
	{
		EntityID id = world.createEntity();
		assert( world.isEntityValid(id) );
		world.destroyEntity( id );
		assert( !world.isEntityValid(id) );
	}

	// Test non-repeatability
	{
		EntityID id1 = world.createEntity();
		world.destroyEntity( id1 );
		EntityID id2 = world.createEntity();
		assert( id1 != id2 );
		assert( !world.isEntityValid(id1) );
		assert( world.isEntityValid(id2) );
	}
}

int main(int argc, char *argv[])
{
	testWorldManager();

	return 0;
}
*/
