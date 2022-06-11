#include "StupidMenuState.h"

FStupidMenuState::FStupidMenuState()
	: Title()
	, Elements()
{
}

FStupidMenuState::FStupidMenuState(const FText& Title)
	: Title(Title)
	, Elements()
{
}
