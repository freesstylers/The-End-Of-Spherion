#include "Components/PauseMenuComponent.h"
#include "MotorCasaPaco.h"
#include "Input/InputManager.h"
#include "Audio/AudioManager.h"
#include "Components/GameManager.h"

PauseMenuComponent::PauseMenuComponent(json& args): Component(args)
{
	std::vector<std::string> res43;
	std::vector<std::string> res54;
	std::vector<std::string> res1610;
	std::vector<std::string> res169;

	res43.push_back("640 x 480");
	res43.push_back("800 x 600");
	res43.push_back("832 x 624");
	res43.push_back("1024 x 768");
	res43.push_back("1152 x 872");

	res54.push_back("1280 x 1024");

	res1610.push_back("1280 x 800");
	res1610.push_back("1680 x 1050");

	res169.push_back("1280 x 720");
	res169.push_back("1600 x 900");
	res169.push_back("1920 x 1080");

	resolutions.push_back(res43);
	resolutions.push_back(res54);
	resolutions.push_back(res169);
	resolutions.push_back(res1610);
}

PauseMenuComponent::~PauseMenuComponent()
{
	Component::~Component();
}

bool PauseMenuComponent::ReceiveEvent(Event& event)
{
	if (event.type == "RESET_GRAPHIC_INFO") {
		if (MotorCasaPaco::getInstance()->getFullScreen())
		{
			GUI_Manager::getInstance()->changeText(graphicTexts[1], "Si");
		}
		else
		{
			GUI_Manager::getInstance()->changeText(graphicTexts[1], "No");
		}

		currentFormat = MotorCasaPaco::getInstance()->getScreenProportion();
		currentRes = MotorCasaPaco::getInstance()->getResolution();
		ForResPosition = getCurrentPosForRes(currentFormat, currentRes);
		GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
		GUI_Manager::getInstance()->changeText(graphicTexts[2], currentFormat);

		if (MotorCasaPaco::getInstance()->getVSync())
		{
			GUI_Manager::getInstance()->changeText(graphicTexts[4], "Si");
		}
		else
		{
			GUI_Manager::getInstance()->changeText(graphicTexts[4], "No");
		}
	}
	else if (event.type == "RESET_ADVANCED_GRAPHIC_INFO")
	{
		GUI_Manager::getInstance()->changeText(advancedTexts[2], MotorCasaPaco::getInstance()->getShadows());
		shadowsPos = getShadowsPosition(MotorCasaPaco::getInstance()->getShadows());

		if (MotorCasaPaco::getInstance()->getGamma())
		{
			GUI_Manager::getInstance()->changeText(advancedTexts[1], "Si");
		}
		else
		{
			GUI_Manager::getInstance()->changeText(advancedTexts[1], "No");
		}

		GUI_Manager::getInstance()->changeText(advancedTexts[0], "X " + MotorCasaPaco::getInstance()->getFSAA());
		getFSAAPosition(MotorCasaPaco::getInstance()->getFSAA());
	}
	return false;
}

bool PauseMenuComponent::functionPauseReturn(const CEGUI::EventArgs& e)
{
	//MotorCasaPaco::getInstance()->pause();
	GameManager::getInstance()->pause();
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, false); //El orden de la UI es Ingame/Menu de Pausa/Opciones/Opciones Graficas/Opciones Avanzadas...
	return true;
}

bool PauseMenuComponent::functionPauseReset(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->changeScene(level);
	
	//MotorCasaPaco::getInstance()->pause();
	GameManager::getInstance()->pause();

	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, false); //El orden de la UI es Ingame/Menu de Pausa/Opciones/Opciones Graficas/Opciones Avanzadas...
	EventManager::getInstance()->EmitEvent("inicioNivel");
	return true;
}

bool PauseMenuComponent::functionPauseSettings(const CEGUI::EventArgs& e)
{
	//vainas de layouts
	layoutLayer++;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, false); //Men� de Pausa Out
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, true); //Opciones B�sicas In
	currentXTopButtons = 0;
	currentYTopButtons = tamBasicTop - 1;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
	return true;
}

bool PauseMenuComponent::functionPauseMainMenu(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->changeScene(mainMenu);

	GameManager::getInstance()->pause();
	//MotorCasaPaco::getInstance()->pause();
	
	EventManager::getInstance()->EmitEvent("finNivel");
	return true;
}

bool PauseMenuComponent::functionPauseExit(const CEGUI::EventArgs& e)
{
	//MotorCasaPaco::getInstance()->pause();
	GameManager::getInstance()->pause();

	MotorCasaPaco::getInstance()->exit();
	return true;
}

bool PauseMenuComponent::functionBasicGraphicOptions(const CEGUI::EventArgs& e)
{
	layoutLayer++;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, false); //Men� de Pausa Out
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(3, true); //Opciones B�sicas In
	currentXTopButtons = 0;
	currentYTopButtons = tamGraphicTop - 1;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
	return true;
}

bool PauseMenuComponent::functionBasicBack(const CEGUI::EventArgs& e)
{
	layoutLayer--;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, true); //Men� de Pausa In
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, false); //Opciones B�sicas Out
	currenPos = 2;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
	return true;
}

bool PauseMenuComponent::functionBasicApply(const CEGUI::EventArgs& e)
{
	return true;
}

bool PauseMenuComponent::functionBasicRevert(const CEGUI::EventArgs& e)
{
	return true;
}

bool PauseMenuComponent::functionGraphicAdvancedOptions(const CEGUI::EventArgs& e)
{
	layoutLayer++;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(3, false); //Men� de Pausa Out
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(4, true); //Opciones B�sicas In
	currentXTopButtons = 0;
	currentYTopButtons = tamAdvancedTop - 1;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
	return true;
}

bool PauseMenuComponent::functionGraphicApply(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->changeGraphicComponents();
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);
	return true;
}

bool PauseMenuComponent::functionGraphicRevert(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->revertGraphicChanges();
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	Event evt = Event("RESET_GRAPHIC_INFO");
	EventManager::getInstance()->EmitEvent(evt);

	return true;
}

bool PauseMenuComponent::functionGraphicBack(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->revertGraphicChanges();
	layoutLayer--;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, true); //Opciones B�sicas In
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(3, false); //Men� de Graficos Out
	currenPos = 2;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);

	Event evt = Event("RESET_GRAPHIC_INFO");
	EventManager::getInstance()->EmitEvent(evt);

	return true;
}

bool PauseMenuComponent::functionGraphicFullScreen(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getFullScreen())
	{
		MotorCasaPaco::getInstance()->setFullScreen(false);
		GUI_Manager::getInstance()->changeText(graphicTexts[1], "No");
	}
	else
	{
		MotorCasaPaco::getInstance()->setFullScreen(true);
		GUI_Manager::getInstance()->changeText(graphicTexts[1], "Si");
	}

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionGraphicVSync(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getVSync())
	{
		MotorCasaPaco::getInstance()->setVSync(false);
		GUI_Manager::getInstance()->changeText(graphicTexts[4], "No");
	}
	else
	{
		MotorCasaPaco::getInstance()->setVSync(true);
		GUI_Manager::getInstance()->changeText(graphicTexts[4], "Si");
	}

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionGraphicFormatMore(const CEGUI::EventArgs& e)
{
	if (currentFormat == "4 : 3")
	{
		currentFormat = "5 : 4";
		currentRes = resolutions[1][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "5 : 4")
	{
		currentFormat = "16 : 9";
		currentRes = resolutions[2][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "16 : 9")
	{
		currentFormat = "16 : 10";
		currentRes = resolutions[3][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "16 : 10")
	{
		currentFormat = "4 : 3";
		currentRes = resolutions[0][0];
		ForResPosition = 0;
	}

	GUI_Manager::getInstance()->changeText(graphicTexts[2], currentFormat);
	GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
	MotorCasaPaco::getInstance()->setScreenProportion(currentFormat);
	MotorCasaPaco::getInstance()->setResolution(currentRes);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionGraphicFormatLess(const CEGUI::EventArgs& e)
{
	if (currentFormat == "4 : 3")
	{
		currentFormat = "16 : 10";
		currentRes = resolutions[3][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "5 : 4")
	{
		currentFormat = "4 : 3";
		currentRes = resolutions[0][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "16 : 9")
	{
		currentFormat = "5 : 4";
		currentRes = resolutions[1][0];
		ForResPosition = 0;
	}
	else if (currentFormat == "16 : 10")
	{
		currentFormat = "16 : 9";
		currentRes = resolutions[2][0];
		ForResPosition = 0;
	}

	GUI_Manager::getInstance()->changeText(graphicTexts[2], currentFormat);
	GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
	MotorCasaPaco::getInstance()->setScreenProportion(currentFormat);
	MotorCasaPaco::getInstance()->setResolution(currentRes);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionGraphicResolutionMore(const CEGUI::EventArgs& e)
{
	if (currentFormat == "4 : 3")
	{
		if (currentRes == resolutions[0][4])
		{
			currentRes = resolutions[0][0];
			ForResPosition = 0;
		}
		else
		{
			currentRes = resolutions[0][ForResPosition + 1];
			ForResPosition++;
		}
	}
	else if (currentFormat == "5 : 4")
	{
		currentRes = currentRes;
	}
	else if (currentFormat == "16 : 9")
	{
		if (currentRes == resolutions[2][2])
		{
			currentRes = resolutions[2][0];
			ForResPosition = 0;
		}
		else
		{
			currentRes = resolutions[2][ForResPosition + 1];
			ForResPosition++;
		}
	}
	else if (currentFormat == "16 : 10")
	{
		if (currentRes == resolutions[3][1])
		{
			currentRes = resolutions[3][0];
			ForResPosition = 0;
		}
		else
		{
			currentRes = resolutions[3][ForResPosition + 1];
			ForResPosition++;
		}
	}

	GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
	MotorCasaPaco::getInstance()->setResolution(currentRes);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionGraphicResolutionLess(const CEGUI::EventArgs& e)
{
	if (currentFormat == "4 : 3")
	{
		if (currentRes == resolutions[0][0])
		{
			currentRes = resolutions[0][4];
			ForResPosition = 4;
		}
		else
		{
			currentRes = resolutions[0][ForResPosition - 1];
			ForResPosition--;
		}
	}
	else if (currentFormat == "5 : 4")
	{
		currentRes = currentRes;
	}
	else if (currentFormat == "16 : 9")
	{

		if (currentRes == resolutions[2][0])
		{
			currentRes = resolutions[2][2];
			ForResPosition = 2;
		}
		else
		{
			currentRes = resolutions[2][ForResPosition - 1];
			ForResPosition--;
		}
	}
	else if (currentFormat == "16 : 10")
	{
		if (currentRes == resolutions[3][0])
		{
			currentRes = resolutions[3][1];
			ForResPosition = 1;
		}
		else
		{
			currentRes = resolutions[3][ForResPosition - 1];
			ForResPosition--;
		}
	}

	GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
	MotorCasaPaco::getInstance()->setResolution(currentRes);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedApply(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->changeAdvancedGraphicComponents();
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);
	return true;
}

bool PauseMenuComponent::functionAdvancedRevert(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->revertAdvancedGraphicChanges();

	Event evt = Event("RESET_ADVANCED_GRAPHIC_INFO");
	EventManager::getInstance()->EmitEvent(evt);

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedBack(const CEGUI::EventArgs& e)
{
	MotorCasaPaco::getInstance()->revertAdvancedGraphicChanges();
	layoutLayer--;
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(3, true); //Opciones Graficos In
	MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(4, false); //Menu Avanzado Out
	currentXTopButtons = 0;
	currentYTopButtons = tamGraphicTop - 1;
	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
	Event evt = Event("RESET_ADVANCED_GRAPHIC_INFO");
	EventManager::getInstance()->EmitEvent(evt);
	return true;
}

bool PauseMenuComponent::functionAdvancedShadowsLess(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getShadows() == "No")
	{
		shadowsPos = 3;
	}
	else
	{
		shadowsPos--;
	}

	MotorCasaPaco::getInstance()->setShadows(shadowValues[shadowsPos]); //Hace el set directamente
	GUI_Manager::getInstance()->changeText(advancedTexts[2], shadowValues[shadowsPos]);

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedShadowsMore(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getShadows() == "Alto")
	{
		shadowsPos = 0;
	}
	else
	{
		shadowsPos++;
	}

	MotorCasaPaco::getInstance()->setShadows(shadowValues[shadowsPos]); //Hace el set directamente
	GUI_Manager::getInstance()->changeText(advancedTexts[2], shadowValues[shadowsPos]);

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedGamma(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getGamma())
	{
		MotorCasaPaco::getInstance()->setGamma(false); //Hace el set directamente
		GUI_Manager::getInstance()->changeText(advancedTexts[1], "No");
	}
	else
	{
		MotorCasaPaco::getInstance()->setGamma(true); //Hace el set directamente
		GUI_Manager::getInstance()->changeText(advancedTexts[1], "Si");
	}

	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedFSAALess(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getFSAA() == "0")
	{
		fsaaPos = 3;
	}
	else
	{
		fsaaPos--;
	}

	MotorCasaPaco::getInstance()->setFSAA(fsaaValues[fsaaPos]); //Hace el set directamente
	GUI_Manager::getInstance()->changeText(advancedTexts[0], "X " + fsaaValues[fsaaPos]);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

bool PauseMenuComponent::functionAdvancedFSAAMore(const CEGUI::EventArgs& e)
{
	if (MotorCasaPaco::getInstance()->getFSAA() == "8")
	{
		fsaaPos = 0;
	}
	else
	{
		fsaaPos++;
	}

	MotorCasaPaco::getInstance()->setFSAA(fsaaValues[fsaaPos]); //Hace el set directamente
	GUI_Manager::getInstance()->changeText(advancedTexts[0], "X " + fsaaValues[fsaaPos]);
	AudioManager::getInstance()->playMusic("assets/sound/buttonSound.mp3", 0, false);

	return true;
}

void PauseMenuComponent::update()
{
	if (MotorCasaPaco::getInstance()->getTimeDifference(currentTime) > delayPause)
	{
		if (InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_START) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_ESCAPE))
		{
			//MotorCasaPaco::getInstance()->pause();
			GameManager::getInstance()->pause();

			MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, true); //El orden de la UI es Ingame/Menu de Pausa/Opciones/Opciones Graficas/Opciones Avanzadas...

			currentTime = MotorCasaPaco::getInstance()->getTime();
		}
	}
}

void PauseMenuComponent::pausedUpdate()
{
	if (MotorCasaPaco::getInstance()->getTimeDifference(currentTime) > delay)
	{
		switch (layoutLayer)
		{
			case 0: //Pause Menu
			{
				if (InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_START) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_ESCAPE))
				{
					if (MotorCasaPaco::getInstance()->getTimeDifference(currentTime) > delayPause)
					{
						GameManager::getInstance()->pause();
						//MotorCasaPaco::getInstance()->pause();

						MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, false); //El orden de la UI es Ingame/Menu de Pausa/Opciones/Opciones Graficas/Opciones Avanzadas...
						
						layoutLayer = 0;

						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}

				if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_UP) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_W) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_UP))
				{
					if (currenPos > 0)
						currenPos--;
					else
						currenPos = tamPause - 1;

					MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
					currentTime = MotorCasaPaco::getInstance()->getTime();
				}

				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_DOWN) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_S) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_DOWN))
				{
					if (currenPos < (tamPause - 1))
						currenPos++;
					else
						currenPos = 0;

					MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
					currentTime = MotorCasaPaco::getInstance()->getTime();
				}
			}
			break;

			case 1: //Basic Options
			{
				if (InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_B) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_ESCAPE))
				{
					layoutLayer--;
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, true); //Men� de Pausa Out
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, false); //Opciones B�sicas In
					currenPos = 2;
					MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
				}

				if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_LEFT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_A) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_LEFT))
				{
					if (basicTopDown == 2)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}

					else if (basicTopDown == 0)
					{
						if (currentPosDownButtons > 0)
						{
							currentPosDownButtons--;
						}
						else
						{
							currentPosDownButtons = 2;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_RIGHT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_D) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_RIGHT))
				{
					if (basicTopDown == 2)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (basicTopDown == 0)
					{
						if (currentPosDownButtons < 2)
						{
							currentPosDownButtons++;
						}
						else
						{
							currentPosDownButtons = 0;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}

				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_DOWN) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_S) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_DOWN))
				{
					if (basicTopDown == 2)
					{
						if (currentYTopButtons > 0) //Sigue en los botones de arriba
						{
							currentYTopButtons--;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							basicTopDown--;
							currentYTopButtons = 0;
							currentXTopButtons = 0;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xBasicExtra, yBasicExtra);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (basicTopDown == 1)
					{
						basicTopDown--;
						currentPosDownButtons = 1;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsBasic[1], yBasicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (basicTopDown == 0)
					{
						basicTopDown = 2;
						currentYTopButtons = tamBasicTop - 1;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_UP) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_W) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_UP))
				{
					if (basicTopDown == 2)
					{
						if (currentYTopButtons < tamBasicTop - 1) //Sigue en los botones de arriba
						{
							currentYTopButtons++;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							basicTopDown = 0;
							currentPosDownButtons = 1;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsBasic[1], yBasicBot);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (basicTopDown == 1)
					{
						basicTopDown++;
						currentYTopButtons = 0;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsBasic[currentXTopButtons], positionsYBasic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (basicTopDown == 0)
					{
						basicTopDown++;
						currentYTopButtons = 0;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xBasicExtra, yBasicExtra);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
			}
			break;

			case 2: //Graphic Options
			{
				if (InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_B) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_ESCAPE))
				{
					layoutLayer--;
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(1, true); //Men� de Pausa Out
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(2, false); //Opciones B�sicas In
					currenPos = 2;
					MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
				}

				if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_LEFT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_A) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_LEFT))
				{
					if (graphicTopDown == 2)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (graphicTopDown == 0)
					{
						if (currentPosDownButtons == 0)
							currentPosDownButtons = tamGraphicDown - 1;
						else
							currentPosDownButtons--;

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_RIGHT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_D) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_RIGHT))
				{
					if (graphicTopDown == 2)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (graphicTopDown == 0)
					{
						if (currentPosDownButtons < 2)
							currentPosDownButtons++;
						else
							currentPosDownButtons = 0;

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}

					//MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsX[currenPos], positionsY[currenPos]);
					//currentTime = MotorCasaPaco::getInstance()->getTime();
				}

				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_DOWN) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_S) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_DOWN))
				{
					if (graphicTopDown == 2)
					{
						if (currentYTopButtons > 0) //Sigue en los botones de arriba
						{
							currentYTopButtons--;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							graphicTopDown--;
							currentYTopButtons = 0;
							currentXTopButtons = 0;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xExtraGraphic, yExtraGraphic); //Extra Button
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (graphicTopDown == 1)
					{
						graphicTopDown--;
						currentPosDownButtons = 1;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (graphicTopDown == 0)
					{
						graphicTopDown = 2;
						currentYTopButtons = tamGraphicTop - 1;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_UP) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_W) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_UP))
				{
					if (graphicTopDown == 2)
					{
						if (currentYTopButtons < tamGraphicTop - 1) //Sigue en los botones de arriba
						{
							currentYTopButtons++;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							graphicTopDown = 0;
							currentPosDownButtons = 1;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsGraphic[currentPosDownButtons], yGraphicBot);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (graphicTopDown == 1)
					{
						graphicTopDown--;
						currentYTopButtons = 0;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (graphicTopDown == 0)
					{
						graphicTopDown++;
						currentYTopButtons = 0;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xExtraGraphic, yExtraGraphic); //Extra Button
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
			}
			break;

			case 3: //Advanced Graphic Options
			{
				if (InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_B) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_ESCAPE))
				{
					MotorCasaPaco::getInstance()->revertAdvancedGraphicChanges();
					layoutLayer--;
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(3, true); //Opciones Graficos In
					MotorCasaPaco::getInstance()->getGUI_Manager()->setLayoutVisible(4, false); //Menu Avanzado Out
					currentXTopButtons = 0;
					currentYTopButtons = tamGraphicTop - 1;
					MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsGraphic[currentXTopButtons], positionsYGraphic[currentYTopButtons]);
					Event evt = Event("RESET_ADVANCED_GRAPHIC_INFO");
					EventManager::getInstance()->EmitEvent(evt);
				}

				if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_LEFT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_A) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_LEFT))
				{
					if (advancedTopDown == 1)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (advancedTopDown == 0)
					{
						if (currentPosDownButtons == 0)
							currentPosDownButtons = tamAdvancedTop - 1;
						else
							currentPosDownButtons--;

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsAdvanced[currentPosDownButtons], yAdvancedBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTX, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_RIGHT) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_D) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_RIGHT))
				{
					if (advancedTopDown == 1)
					{
						if (currentXTopButtons == 0)
						{
							currentXTopButtons++;
						}
						else
						{
							currentXTopButtons--;
						}

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
					else if (advancedTopDown == 0)
					{
						if (currentPosDownButtons < 2)
							currentPosDownButtons++;
						else
							currentPosDownButtons = 0;

						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsAdvanced[currentPosDownButtons], yAdvancedBot);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}

					//MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsX[currenPos], positionsY[currenPos]);
					//currentTime = MotorCasaPaco::getInstance()->getTime();
				}

				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) < -0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_DOWN) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_S) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_DOWN))
				{
					if (advancedTopDown == 1)
					{
						if (currentYTopButtons > 0) //Sigue en los botones de arriba
						{
							currentYTopButtons--;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							advancedTopDown = 0;
							currentYTopButtons = 0;
							currentXTopButtons = 0;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsAdvanced[currentPosDownButtons], yAdvancedBot);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (advancedTopDown == 0)
					{
						advancedTopDown = 1;
						currentYTopButtons = tamAdvancedTop - 1;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
				else if (InputManager::getInstance()->GameControllerGetAxisMovement(GameControllerAxis::CONTROLLER_AXIS_LEFTY, true) > 0.7 || InputManager::getInstance()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_DPAD_UP) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_W) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_UP))
				{
					if (advancedTopDown == 1)
					{
						if (currentYTopButtons < tamAdvancedTop - 1) //Sigue en los botones de arriba
						{
							currentYTopButtons++;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
						else //Baja a los botones de abajo
						{
							advancedTopDown = 0;
							currentPosDownButtons = 1;
							MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXBotButtonsAdvanced[currentPosDownButtons], yAdvancedBot);
							currentTime = MotorCasaPaco::getInstance()->getTime();
						}
					}
					else if (advancedTopDown == 0)
					{
						advancedTopDown++;
						currentYTopButtons = 0;
						currentXTopButtons = 0;
						MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(positionsXTopButtonsAdvanced[currentXTopButtons], positionsYAdvanced[currentYTopButtons]);
						currentTime = MotorCasaPaco::getInstance()->getTime();
					}
				}
			}
			break;
		}

		if (MotorCasaPaco::getInstance()->getInputManager()->GameControllerIsButtonDown(GameControllerButton::CONTROLLER_BUTTON_A) || InputManager::getInstance()->IsKeyDown(Scancode::SCANCODE_SPACE))
		{
			MotorCasaPaco::getInstance()->getInputManager()->injectLeftMouseButtonDown();
			currentTime = MotorCasaPaco::getInstance()->getTime();
		}
		else
		{
			MotorCasaPaco::getInstance()->getInputManager()->injectLeftMouseButtonUp();
		}
	}
}

bool PauseMenuComponent::optionsAssert(json& j)
{
	return (!j["basicOptionButtons"].is_null() && j["basicOptionButtons"].is_array() && !j["basicExtraButton"].is_null() && !j["basicOptionBotButtons"].is_null() && j["basicOptionBotButtons"].is_array()
		&& !j["graphicOptionDownButtons"].is_null() && j["graphicOptionDownButtons"].is_array()
		&& !j["graphicOptionTopButtons"].is_null() && j["graphicOptionTopButtons"].is_array() && !j["graphicOptionExtraButton"].is_null()
		&& !j["graphicOptionTexts"].is_null() && j["graphicOptionTexts"].is_array()
		&& !j["advancedGraphicOptionDownButtons"].is_null() && j["advancedGraphicOptionDownButtons"].is_array()
		&& !j["advancedOptionTexts"].is_null() && j["advancedOptionTexts"].is_array()
		&& !j["advancedGraphicOptionTopButtons"].is_null() && j["advancedGraphicOptionTopButtons"].is_array());
}

int PauseMenuComponent::getCurrentPosForRes(std::string currentFormat_, std::string currentRes_)
{
	if (currentFormat_ == "4 : 3")
	{
		if (currentRes_ == "640 x 480")
		{
			return 0;
		}
		else if (currentRes_ == "800 x 600")
		{
			return 1;
		}
		else if (currentRes_ == "832 x 624")
		{
			return 2;
		}
		else if (currentRes_ == "1024 x 768")
		{
			return 3;
		}
		else if (currentRes_ == "1152 x 872")
		{
			return 4;
		}
	}
	else if (currentFormat_ == "5 : 4")
	{
		return 0;
	}
	else if (currentFormat_ == "16 : 9")
	{
		if (currentRes_ == "1280 x 720")
		{
			return 0;
		}
		else if (currentRes_ == "1600 x 900")
		{
			return 1;
		}
		else if (currentRes_ == "1920 x 1080")
		{
			return 2;
		}
	}
	else if (currentFormat_ == "16 : 10")
	{
		if (currentRes_ == "1280 x 800")
		{
			return 0;
		}
		else if (currentRes_ == "1650 x 1050")
		{
			return 1;
		}
	}
}

int PauseMenuComponent::getShadowsPosition(std::string shadow)
{
	if (shadow == "No")
	{
		return 0;
	}
	else if (shadow == "Bajo")
	{
		return 1;
	}
	else if (shadow == "Medio")
	{
		return 2;
	}
	else if (shadow == "Alto")
	{
		return 3;
	}
}

int PauseMenuComponent::getFSAAPosition(std::string fsaa)
{
	if (fsaa == "0")
	{
		return 0;
	}
	else if (fsaa == "2")
	{
		return 1;
	}
	else if (fsaa == "4")
	{
		return 2;
	}
	else if (fsaa == "8")
	{
		return 3;
	}
}

void PauseMenuComponent::init(json& j)
{
	//Para que el componente funcione bien, hay que asegurarse de que todos estos datos est�n presentes en el componente. Si no, el componente fallar�.
	if (!optionsAssert(j) ||
		j["delayPause"].is_null() || j["delay"].is_null() || j["buttons"].is_null()
		|| (!j["buttons"].is_null() && !j["buttons"].is_array()) || j["MainMenu"].is_null())
		return;


	/////////////////////////////////////////////////////////
	//Pause Menu Stuff

	std::vector<std::string> vec = j["buttons"];

	int count = 0;
	for (std::string name : vec) {

		if (count == 0)
			xPause = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
			
		float y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

		positionsYPause.push_back(y);

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			auto helperFunction = std::bind(&PauseMenuComponent::functionPauseReturn, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction);
		}
		break;
		case 1:
		{
			auto helperFunction = std::bind(&PauseMenuComponent::functionPauseSettings, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction);
		}
		break;
		case 2:
		{
			auto helperFunction = std::bind(&PauseMenuComponent::functionPauseMainMenu, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction);
		}
		break;
		case 3:
		{
			auto helperFunction = std::bind(&PauseMenuComponent::functionPauseExit, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction);
		}
		break;
		}

		count++;
	}

	tamPause = count;

	//////////////////////////////////////////////////////////////
	//Basic Options Stuff

	std::vector<std::string> vec2 = j["basicOptionButtons"];

	count = 0;

	for (std::string name : vec2)
	{
		if (count < 2)
		{
			float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
			positionsXTopButtonsBasic.push_back(x);
		}

		if (count % 2 == 0)
		{
			float y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

			positionsYBasic.push_back(y);
		}

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			//auto helperFunction = std::bind(&PauseMenuComponent::functionReturn, this, std::placeholders::_1);
			//GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction);
		}
		break;
		case 1:
		{

		}
		break;
		case 2:
		{

		}
		break;
		case 3:
		{

		}
		break;
		case 4:
		{

		}
		break;
		case 5:
		{

		}
		break;
		case 6:
		{

		}
		break;
		case 7:
		{

		}
		break;
		case 8:
		{

		}
		break;
		case 9:
		{

		}
		break;
		}
			
		count++;
	}

	tamBasicTop = count / 2;

	std::string interm = j["basicExtraButton"];
	yBasicExtra = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(interm).getCenterPointYAbsolute();
	xBasicExtra = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(interm).getCenterPointYAbsolute();

	//Assign function

	auto helperFunction = std::bind(&PauseMenuComponent::functionBasicGraphicOptions, this, std::placeholders::_1);
	GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(interm), helperFunction);

	std::vector<std::string> vec7 = j["basicOptionBotButtons"];

	float y;
	count = 0;

	for (std::string name : vec7)
	{
		float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();

		if (count == 0)
			y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

		positionsXBotButtonsBasic.push_back(x);

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionBasicApply, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		case 1:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionBasicRevert, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		case 2:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionBasicBack, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		}

		count++;
	}

	yBasicBot = y;

	tamBasicBot = count;

	//////////////////////////////////////////////////////////////
	//Graphic Options Stuff

	std::vector<std::string> vec3 = j["graphicOptionDownButtons"];

	count = 0;

	for (std::string name : vec3)
	{
		float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
			
		if (count == 0)
			y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

		positionsXBotButtonsGraphic.push_back(x);

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionGraphicApply, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		case 1:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionGraphicRevert, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		case 2:
		{
			auto helperFunctionGraphicBot = std::bind(&PauseMenuComponent::functionGraphicBack, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionGraphicBot);
		}
		break;
		}

		count++;
	}

	yGraphicBot = y;

	tamGraphicDown = count;

	//Texts
	std::vector<std::string> vecText1 = j["graphicOptionTexts"];

	for (std::string name : vecText1)
	{
		graphicTexts.push_back(GUI_Manager::getInstance()->getStaticText(name));
	}

	std::vector<std::string> vec4 = j["graphicOptionTopButtons"];

	count = 0;

	for (std::string name : vec4)
	{
		if (count < 2)
		{
			float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
			positionsXTopButtonsGraphic.push_back(x);
		}

		if (count % 2 == 0)
		{
			y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

			positionsYGraphic.push_back(y);
		}

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicVSync, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);			
		}
		break;
		case 1:
		{
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicVSync, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);			
		}
		break;
		case 2:
		{
			//Resolucion
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicResolutionLess, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 3:
		{
			//Resolucion
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicResolutionMore, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 4:
		{
			//Formato de pantalla 
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicFormatLess, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 5:
		{
			//Formato de pantalla 
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicFormatMore, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 6:
		{
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicFullScreen, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 7:
		{
			auto helperFunctionTopGraphic = std::bind(&PauseMenuComponent::functionGraphicFullScreen, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopGraphic);
		}
		break;
		case 8:
		{
			//Configuraciones predefinidas
		}
		break;
		case 9:
		{
			//Configuraciones predefinidas
		}
		break;
		}

		count++;
	}

	tamGraphicTop = count / 2;
		
	std::string name = j["graphicOptionExtraButton"];
	xExtraGraphic = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
	yExtraGraphic = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

	auto helperFunction2 = std::bind(&PauseMenuComponent::functionGraphicAdvancedOptions, this, std::placeholders::_1);
	GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunction2);

	//////////////////////////////////////////////////////////////
	//Advanced Graphic Options Stuff

	std::vector<std::string> vec5 = j["advancedGraphicOptionDownButtons"];

	count = 0;

	for (std::string name : vec5)
	{
		float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();

		if (count == 0)
			y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

		positionsXBotButtonsAdvanced.push_back(x);

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{
			auto helperFunctionAdvancedBot = std::bind(&PauseMenuComponent::functionAdvancedApply, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionAdvancedBot);
		}
		break;
		case 1:
		{
			auto helperFunctionAdvancedBot = std::bind(&PauseMenuComponent::functionAdvancedRevert, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionAdvancedBot);
		}
		break;
		case 2:
		{
			auto helperFunctionAdvancedBot = std::bind(&PauseMenuComponent::functionAdvancedBack, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionAdvancedBot);
		}
		break;
		}

		count++;
	}

	yAdvancedBot = y;

	tamAdvancedDown = count;

	//Texts

	std::vector<std::string> vecText2 = j["advancedOptionTexts"];

	for (std::string name : vecText2)
	{
		advancedTexts.push_back(GUI_Manager::getInstance()->getStaticText(name));
	}

	std::vector<std::string> vec6 = j["advancedGraphicOptionTopButtons"];

	count = 0;

	for (std::string name : vec6) //Faltan funciones de los botones
	{
		if (count < 2)
		{
			float x = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointXAbsolute();
			positionsXTopButtonsAdvanced.push_back(x);
		}

		if (count % 2 == 0)
		{
			y = MotorCasaPaco::getInstance()->getGUI_Manager()->getRoot()->getChild(name).getCenterPointYAbsolute();

			positionsYAdvanced.push_back(y);
		}

		//Asignacion de funciones

		switch (count)
		{
		case 0:
		{

		}
		break;
		case 1:
		{

		}
		break;
		case 2:
		{

		}
		break;
		case 3:
		{

		}
		break;
		case 4:
		{

		}
		break;
		case 5:
		{

		}
		break;
		case 6:
		{

		}
		break;
		case 7:
		{

		}
		break;
		case 8:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedShadowsLess, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		case 9:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedShadowsMore, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		case 10:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedGamma, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		case 11:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedGamma, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		case 12:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedFSAALess, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		case 13:
		{
			auto helperFunctionTopAdvanced = std::bind(&PauseMenuComponent::functionAdvancedFSAAMore, this, std::placeholders::_1);
			GUI_Manager::getInstance()->setEvents(GUI_Manager::getInstance()->getPushButton(name), helperFunctionTopAdvanced);
		}
		break;
		}

		count++;
	}

	tamAdvancedTop = count / 2;

	//////////////////////////////////////////////////////////////

	std::string interm3 = j["MainMenu"];
	mainMenu = interm3;

	delay = j["delay"];
	delayPause = j["delayPause"];

	currenPos = 0;
	layoutLayer = 0;

	//Registrar listeners

	EventManager::getInstance()->RegisterListener(this, "RESET_GRAPHIC_INFO");
	EventManager::getInstance()->RegisterListener(this, "RESET_ADVANCED_GRAPHIC_INFO");

	//Cosas del motor

	currentTime = MotorCasaPaco::getInstance()->getTime();

	if (MotorCasaPaco::getInstance()->getFullScreen())
	{
		GUI_Manager::getInstance()->changeText(graphicTexts[1], "Si");
	}
	else
	{
		GUI_Manager::getInstance()->changeText(graphicTexts[1], "No");
	}


	if (MotorCasaPaco::getInstance()->getVSync())
	{
		GUI_Manager::getInstance()->changeText(graphicTexts[4], "Si");
	}
	else
	{
		GUI_Manager::getInstance()->changeText(graphicTexts[4], "No");
	}

	currentFormat = MotorCasaPaco::getInstance()->getScreenProportion();
	currentRes = MotorCasaPaco::getInstance()->getResolution();
	ForResPosition = getCurrentPosForRes(currentFormat, currentRes);
	GUI_Manager::getInstance()->changeText(graphicTexts[3], currentRes);
	GUI_Manager::getInstance()->changeText(graphicTexts[2], currentFormat);

	shadowsPos = getShadowsPosition(MotorCasaPaco::getInstance()->getShadows());
	GUI_Manager::getInstance()->changeText(advancedTexts[2], MotorCasaPaco::getInstance()->getShadows());

	if (MotorCasaPaco::getInstance()->getGamma())
	{
		GUI_Manager::getInstance()->changeText(advancedTexts[1], "Si");
	}
	else
	{
		GUI_Manager::getInstance()->changeText(advancedTexts[1], "No");
	}

	fsaaPos = getFSAAPosition(MotorCasaPaco::getInstance()->getFSAA());
	GUI_Manager::getInstance()->changeText(advancedTexts[0], "X " + MotorCasaPaco::getInstance()->getFSAA());

	MotorCasaPaco::getInstance()->getGUI_Manager()->injectPosition(xPause, positionsYPause[currenPos]);
}
