#pragma once

#include "CoreMinimal.h"

class STUPIDMENU_API FExampleMenu
{
public:
	static void Open(UWorld* World);

private:
	static FText GetTitle();
	static void OnClick(class UStupidMenuScreen* MenuScreen);

};
