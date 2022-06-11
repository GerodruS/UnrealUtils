#pragma once

#include "StupidMenuElement.generated.h"

DECLARE_DELEGATE_RetVal(FText, FStupidMenuElementTitleDelegate)
DECLARE_DELEGATE(FStupidMenuElementCallbackDelegate);

USTRUCT()
struct STUPIDMENU_API FStupidMenuElement
{
	GENERATED_BODY()

	static const FStupidMenuElement Empty;

	FStupidMenuElementTitleDelegate OnGetTitle;
	FStupidMenuElementCallbackDelegate OnClick;

	FStupidMenuElement();

	FStupidMenuElement(
		const FStupidMenuElementTitleDelegate& OnGetTitle,
		const FStupidMenuElementCallbackDelegate& OnClick);

	FStupidMenuElement(
		TFunctionRef<FText ()> OnGetTitle,
		TFunctionRef<void ()> OnClick);

	FStupidMenuElement(
		FText Title,
		TFunctionRef<void ()> OnClick);

	bool IsEmpty() const;
};
