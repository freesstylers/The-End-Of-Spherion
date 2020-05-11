
#include "Entity/Entity.h"
#include "Events/Event.h"
#include "Scene/SceneManager.h"
#include "MotorCasaPaco.h"
#include <iostream>
#include <stdlib.h>
#include "Components/SpawnerMejora.h"

#include "Entity/Transform.h"

SpawnerMejora::SpawnerMejora(json& args):EntitySpawner(args)
{
	
}

SpawnerMejora::~SpawnerMejora()
{
	Component::~Component();
}

void SpawnerMejora::update()
{
	if (timeToSpawn >= timeSinceLastSpawn) {
		spawnEntity();
		std::cout << "spawned" << std::endl;
		timeToSpawn = rand() % maxSpawnTime + minSpawnTime;
	}
	timeSinceLastSpawn += MotorCasaPaco::getInstance()->DeltaTime();
}

void SpawnerMejora::init(json& j)
{
	EntitySpawner::init(j);
	timeToSpawn = rand() % maxSpawnTime + minSpawnTime;

}

Entity* SpawnerMejora::spawnEntity(Vector3 pos, float speed,std::string prefab)
{
	float iniAngle = 0;

	Entity* prj = EntitySpawner::spawnEntity(pos, prefab);


	prj->getComponent<Transform>("Transform")->setRotation(Vector3(0, 90, 0));

	std::cout << "spawned" << std::endl;
	return nullptr;
}
