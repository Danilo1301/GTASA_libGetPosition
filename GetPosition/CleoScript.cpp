#include "CleoScript.h"

#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include <string>

#include "IMultiRemap.h"
extern IMultiRemap* multiRemap;

void CleoScript::OnLoad()
{ 
}

void CleoScript::OnFirstUpdate()
{
}

void CleoScript::OnUpdate()
{
    if(menuVSL)
    {
        if(menuVSL->GetGlobalIntVariable("show_menu_getPosition") == 1)
        {
            menuVSL->SetGlobalIntVariable("show_menu_getPosition", 0);

            CreateMenu();
        }
    }
}

void CleoScript::CreateMenu()
{
    auto window = menuVSL->AddWindow();
    window->m_Title = "Get position";

    auto playerPosition = window->AddText("", CRGBA(255, 255, 255));
    playerPosition->onUpdate = [playerPosition]() {
        auto position = GetPlayerPosition();

        playerPosition->m_Text = "Player position: " + std::to_string((int)position.x) + ", " + std::to_string((int)position.y) + ", " + std::to_string((int)position.z);
    };

    auto vehiclePosition = window->AddText("", CRGBA(255, 255, 255));
    vehiclePosition->onUpdate = [vehiclePosition]() {
        auto position = CVector(0, 0, 0);
        auto playerActor = GetPlayerActor();
        auto vehicleHandle = GetVehiclePedIsUsing(playerActor);

        if(vehicleHandle)
        {
            position = GetCarPosition(vehicleHandle);
        }

        vehiclePosition->m_Text = "Vehicle position: " + std::to_string((int)position.x) + ", " + std::to_string((int)position.y) + ", " + std::to_string((int)position.z);
    };

    window->AddText("- Position will be saved at:", CRGBA(255, 255, 255));
    window->AddText("- /configs/getPosition/", CRGBA(255, 255, 255));

    auto savePlayerPos = window->AddButton("> ~y~Save player position");
    savePlayerPos->onClick = [window]() {
        SavePlayerPos();
        window->SetToBeRemoved();
    };

    auto saveVehiclePos = window->AddButton("> ~y~Save vehicle position");
    saveVehiclePos->onClick = [window]() {
        SaveVehiclePos();
        window->SetToBeRemoved();
    };

    auto close = window->AddButton("> ~r~Close");
    close->onClick = [window]() {
        window->SetToBeRemoved();
    };
}

void CleoScript::SavePlayerPos()
{
    auto position = GetPlayerPosition();

    char path[512];
	sprintf(path, "%s/getPosition/savedpositions.txt", aml->GetConfigPath());

    std::ofstream file(path, std::ios_base::app);
    if (file)
    {
        file << position.x << " " << position.y << " " << position.z << std::endl;
    }

    Log::Level(eLogLevel::LOG_BOTH) << "---- SAVED POSITION ----" << std::endl;
    Log::Level(eLogLevel::LOG_BOTH) << position.x << " " << position.y << " " << position.z << std::endl;
    Log::Level(eLogLevel::LOG_BOTH) << "------------------------" << std::endl;
}

void CleoScript::SaveVehiclePos()
{
    char path[512];
	sprintf(path, "%s/getPosition/savedpositions.txt", aml->GetConfigPath());

    auto position = CVector(0, 0, 0);
    auto playerActor = GetPlayerActor();
    auto vehicleHandle = GetVehiclePedIsUsing(playerActor);

    if(vehicleHandle)
    {
        position = GetCarPosition(vehicleHandle);
    }

    auto angle = GET_CAR_Z_ANGLE(vehicleHandle);
    auto modelId = GET_CAR_MODEL(vehicleHandle);
    std::string remap = "";
    if(multiRemap) remap = multiRemap->GetVehicleRemap(vehicleHandle);
    if(remap.size() == 0) remap = "random";

    std::ofstream file(path, std::ios_base::app);
    if (file)
    {
        file << modelId << " " << position.x << " " << position.y << " " << position.z << " " << angle << " " << remap << std::endl;
    }

    Log::Level(eLogLevel::LOG_BOTH) << "---- SAVED POSITION ----" << std::endl;
    Log::Level(eLogLevel::LOG_BOTH) << modelId << " " << position.x << " " << position.y << " " << position.z << " " << angle << " " << remap << std::endl;
    Log::Level(eLogLevel::LOG_BOTH) << "------------------------" << std::endl;
}