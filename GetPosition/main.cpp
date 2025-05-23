#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>

#include "utils.h"

#include "Log.h"
#include "CleoScript.h"

// ---------------------------------------

MYMODCFG(net.danilo1301.getPosition, Get Position, 1.0.1, Danilo1301)

// ---------------------------------------

// CLEO 2.0.1.5
#include "cleo.h"
cleo_ifs_t* cleo = NULL;

// Script Commands
#include "IScriptCommands.h"
IScriptCommands* scriptCommands = NULL;

// Menu VSL
#include "menu/IMenuVSL.h"
IMenuVSL* menuVSL = NULL;

#include "IMultiRemap.h"
IMultiRemap* multiRemap = NULL;

// ---------------------------------------

uintptr_t pGameLib = 0;
void* pGameHandle = NULL;

//---------------------------------------------------------------------------------------------------

extern "C" void OnModPreLoad()
{
    char logPath[512];
	sprintf(logPath, "%s/getPosition/", aml->GetConfigPath());

    CreateFolder(logPath);

    Log::Open(logPath, "getPosition");

    Log::Level(eLogLevel::LOG_BOTH) << "Preload()" << std::endl;
    Log::Level(eLogLevel::LOG_BOTH) << "AML headers: 1.0.3.1" << std::endl;

    logger->SetTag("GetPosition");

    logger->Info("Preload");

    Log::Level(eLogLevel::LOG_BOTH) << "Preload() END" << std::endl;
}

extern "C" void OnModLoad()
{
    Log::Level(eLogLevel::LOG_BOTH) << "Load()" << std::endl;

    cfg->Bind("Author", "", "About")->SetString("Danilo1301"); cfg->ClearLast();
    cfg->Bind("Discord", "", "About")->SetString("https://discord.gg/mkCDRf4zJA"); cfg->ClearLast();
    cfg->Bind("GitHub", "", "About")->SetString("https://github.com/Danilo1301"); cfg->ClearLast();
    cfg->Save();

    Log::Level(eLogLevel::LOG_BOTH) << "Loading interfaces..." << std::endl;

    LoadInterface(&cleo, "CLEO");
    LoadInterface(&scriptCommands, "ScriptCommands");
    LoadInterface(&menuVSL, "MenuVSL");
    LoadInterface(&multiRemap, "MultiRemap");

    if(!menuVSL)
    {
        Log::Level(eLogLevel::LOG_BOTH) << "Error: MenuVSL not found!" << std::endl;
        return;
    }

    if(!scriptCommands)
    {
        Log::Level(eLogLevel::LOG_BOTH) << "Error: ScriptCommands not found!" << std::endl;
        return;
    }

    Log::Level(eLogLevel::LOG_BOTH) << "Loading addresses..." << std::endl;
    
    pGameHandle = dlopen("libGTASA.so", RTLD_LAZY);
    uintptr_t gameAddr = (uintptr_t)(cleo->GetMainLibraryLoadAddress());

    Log::Level(eLogLevel::LOG_BOTH) << "Load() END" << std::endl;

    scriptCommands->AddOnFirstUpdateGame(CleoScript::OnFirstUpdate);
    scriptCommands->AddOnUpdateGame(CleoScript::OnUpdate);

    CleoScript::OnLoad();
}
