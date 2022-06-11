#pragma once

#include "CoreMinimal.h"
// #include "ExampleMenu.generated.h"

class STUPIDMENU_API FExampleMenu
{
public:
	static void Open(UWorld* World);

private:
	static FText GetTitle();
	static void OnClick();
};
