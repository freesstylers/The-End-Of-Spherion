#include "Components/DisparoWiterico.h"
#include "MotorCasaPaco.h"
#include "Audio/AudioManager.h"
#include "Input/InputManager.h"
#include "Entity/Transform.h"
#include "Components/GameManager.h"
DisparoWiterico::DisparoWiterico(json& args):ProjectileSpawner(args)
{
}

void DisparoWiterico::init(json& j)
{
	ProjectileSpawner::init(j);

	nModes = 1;
	if (!j["nModes"].is_null()) {
		nModes = j["nModes"];
	}

	shotModes = std::vector<ShotInfo>(nModes);

	if (!j["shotSound"].is_null()) {
		if (!j["shotSound"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				std::string inter = j["shotSound"];
				shotModes[i].shotSound = inter;
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				std::string inter = j["shotSound"][i];
				shotModes[i].shotSound = inter;
			}
		}
	}

	if (!j["cadence"].is_null()) {
		if (!j["cadence"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].cadence = j["cadence"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].cadence = j["cadence"][i];
			}
		}
	}
	if (!j["shotPos"].is_null() && j["shotPos"].is_array()) {
		if (!j["shotPos"][0].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].shotPos.X = j["shotPos"][0];
				shotModes[i].shotPos.Y = j["shotPos"][1];
				shotModes[i].shotPos.Z = j["shotPos"][2];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].shotPos.X = j["shotPos"][i][0];
				shotModes[i].shotPos.Y = j["shotPos"][i][1];
				shotModes[i].shotPos.Z = j["shotPos"][i][2];
			}
		}
	}
	if (!j["shotDir"].is_null()) {
		if (!j["shotDir"][0].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].shotDir.X = j["shotDir"][0];
				shotModes[i].shotDir.Y = j["shotDir"][1];
				shotModes[i].shotDir.Z = j["shotDir"][2];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].shotDir.X = j["shotDir"][i][0];
				shotModes[i].shotDir.Y = j["shotDir"][i][1];
				shotModes[i].shotDir.Z = j["shotDir"][i][2];
			}
		}
	}
	if (!j["nBullets"].is_null()) {
		if (!j["nBullets"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].nBullets = j["nBullets"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].nBullets = j["nBullets"][i];
			}
		}
	}

	if (!j["damagePerBullet"].is_null()) {
		if (!j["damagePerBullet"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].damagePerBullet = j["damagePerBullet"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].damagePerBullet = j["damagePerBullet"][i];
			}
		}
	}

	if (!j["bulletSpeed"].is_null()) {
		if (!j["bulletSpeed"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].bulletSpeed = j["bulletSpeed"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].bulletSpeed = j["bulletSpeed"][i];
			}
		}
	}
	if (!j["dispersionAngle"].is_null()) {
		if (!j["dispersionAngle"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].dispersionAngle = j["dispersionAngle"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].dispersionAngle = j["dispersionAngle"][i];
			}
		}
	}
	if (!j["inaccuracy"].is_null()) {
		if (!j["inaccuracy"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].inaccuracy = j["inaccuracy"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].inaccuracy = j["inaccuracy"][i];
			}
		}
	}
	if (!j["inacDispersion"].is_null()) {
		if (!j["inacDispersion"].is_array()) {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].inacDispersion = j["inacDispersion"];
			}
		}
		else {
			for (int i = 0; i < shotModes.size(); i++) {
				shotModes[i].inacDispersion = j["inacDispersion"][i];
			}
		}
	}
}

void DisparoWiterico::start()
{
	currMode = 0;
	timeSinceLastShot = shotModes[currMode].cadence;

}

void DisparoWiterico::update()
{
	if (timeSinceLastShot >= shotModes[currMode].cadence && MotorCasaPaco::getInstance()->getInputManager()->GameControllerIsButtonDown(CONTROLLER_BUTTON_A)) {
		timeSinceLastShot = 0;
		spawnProjectiles(shotModes[currMode].shotPos,
			shotModes[currMode].shotDir, shotModes[currMode].bulletSpeed, shotModes[currMode].nBullets, shotModes[currMode].damagePerBullet,
			shotModes[currMode].dispersionAngle, shotModes[currMode].inaccuracy, shotModes[currMode].inacDispersion);
		AudioManager::getInstance()->playMusic(shotModes[currMode].shotSound.c_str(), 3, false);
		AudioManager::getInstance()->setVolume(0.1, 3);
	}
	else if (timeSinceLastShot >= shotModes[currMode].cadence && MotorCasaPaco::getInstance()->getInputManager()->GameControllerIsButtonDown(CONTROLLER_BUTTON_B) && GameManager::getInstance()->isBombActive())
	{
		timeSinceLastShot = 0;
		spawnEutanasia();
		MotorCasaPaco::getInstance()->getAudioManager()->playMusic("assets/sound/SalidaBomba.wav", 3, false);
		AudioManager::getInstance()->setVolume(0.1, 3);
		EventManager::getInstance()->EmitEvent("BombaDown");
	}
	timeSinceLastShot += MotorCasaPaco::getInstance()->DeltaTime();
}