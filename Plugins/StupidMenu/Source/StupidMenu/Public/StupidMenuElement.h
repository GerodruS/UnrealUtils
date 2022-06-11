#pragma once

#include "StupidMenuElement.generated.h"

DECLARE_DELEGATE_RetVal(FText, FStupidMenuElementTitleDelegate)
DECLARE_DELEGATE(FStupidMenuElementCallbackDelegate);

USTRUCT()
struct STUPIDMENU_API FStupidMenuElement
{
	GENERATED_BODY()

	FStupidMenuElementTitleDelegate OnGetTitle;
	FStupidMenuElementCallbackDelegate OnClick;

	FStupidMenuElement();

	FStupidMenuElement(
		const FStupidMenuElementTitleDelegate& OnGetTitle,
		const FStupidMenuElementCallbackDelegate& OnClick);

	bool IsEmpty() const;
};
