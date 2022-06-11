#pragma once

#include "StupidMenuElement.h"

#include "StupidMenuState.generated.h"

USTRUCT()
struct STUPIDMENU_API FStupidMenuState
{
	GENERATED_BODY()

	FStupidMenuState();
	explicit FStupidMenuState(const FText& Title);

	UPROPERTY()
	FText Title;

	UPROPERTY()
	TArray<FStupidMenuElement> Elements;
};