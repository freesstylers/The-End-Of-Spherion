#include "Components/DisparoWiterico.h"
#include "MotorCasaPaco.h"
#include "Input/InputManager.h"
#include "Entity/Transform.h"

DisparoWiterico::DisparoWiterico(json& args):ProjectileSpawner(args)
{

}

void DisparoWiterico::init(json& args)
{
	ProjectileSpawner::init(args);
}

void DisparoWiterico::start()
{
	cadence = 250;
	timeSinceLastShot = cadence;
}

void DisparoWiterico::update()
{
	if (timeSinceLastShot >= cadence && MotorCasaPaco::getInstance()->getInputManager()->GameControllerIsButtonDown(CONTROLLER_BUTTON_A)) {
		timeSinceLastShot = 0;
		spawnProjectiles(getEntity()->getComponent<Transform>("Transform")->getPosition() + Vector3(0,0,-1)* 50, Vector3(0, 0, -1), 500, 1);
	}
	timeSinceLastShot += MotorCasaPaco::getInstance()->DeltaTime();
}
