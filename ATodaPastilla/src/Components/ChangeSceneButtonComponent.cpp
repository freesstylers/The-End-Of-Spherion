#include "Components/ChangeSceneButtonComponent.h"
#include "MotorCasaPaco.h"
#include "Audio/AudioManager.h"
#include "checkML.h"

class Ogre::Root;
class Ogre::SceneManager;

ChangeSceneButtonComponent::ChangeSceneButtonComponent(json& args) : Component(args)
{
}

ChangeSceneButtonComponent::~ChangeSceneButtonComponent()
{
	Component::~Component();
}

bool ChangeSceneButtonComponent::function(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->changeScene(sceneToLoad);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 2, false);
	return true;
}

void ChangeSceneButtonComponent::init(json& j)
{
	if (!j["buttonName"].is_null() && !j["sceneName"].is_null())
	{
		std::string interm = j["sceneName"];
		sceneToLoad = interm;
		auto helperFunction = std::bind(&ChangeSceneButtonComponent::function, this, std::placeholders::_1);
		GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(j["buttonName"]), helperFunction);
	}
}