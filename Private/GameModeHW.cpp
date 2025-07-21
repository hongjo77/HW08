#include "GameModeHW.h"
#include "CharacterHW.h"
#include "PlayerControllerHW.h"
#include "GameStateHW.h"

AGameModeHW::AGameModeHW()
{
	DefaultPawnClass = ACharacterHW::StaticClass();
	PlayerControllerClass = APlayerControllerHW::StaticClass();
	GameStateClass = AGameStateHW::StaticClass();
}

