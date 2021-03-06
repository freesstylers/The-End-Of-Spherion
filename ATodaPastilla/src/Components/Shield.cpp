#include "Components\Shield.h"
#include "Entity/Entity.h"
#include "Events/Event.h"
#include "Graphics/ParticleSystem.h"

#include "MotorCasaPaco.h"
#include "Graphics/Mesh.h"
#include "Scene/SceneManager.h"
#include "checkML.h"

Shield::Shield(json& args) : Component(args)
{

}

Shield::~Shield()
{

}

void Shield::update()
{
	if (activo) {
		if (playerVida->GetVida() < 2) {
			activo = false;
			e_->getComponent<Mesh>("Mesh")->setEnabled(false);
			e_->getComponent<ParticleSystem>("ParticleSystem")->emit();
			EventManager::getInstance()->EmitEvent("EscudoDown");
		}
		else {
			e_->getComponent<Transform>("Transform")->setPosition(playerTrans->getPosition()+ Vector3(0,0,-35));
		}
	}
	else {
		if (playerVida->GetVida() >= 2) {
			activo = true;
			e_->getComponent<Mesh>("Mesh")->setEnabled(true);
		}
	}
}

void Shield::init(json& j)
{
	playerTrans = SceneManager::getInstance()->getCurrentScene()->getEntity("Player")->getComponent<Transform>("Transform");
	playerVida = SceneManager::getInstance()->getCurrentScene()->getEntity("Player")->getComponent<VidaPlayer>("VidaPlayer");
	e_->getComponent<Mesh>("Mesh")->setEnabled(false);
}
