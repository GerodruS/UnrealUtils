#pragma once

#include "StupidMenuElement.generated.h"

DECLARE_DELEGATE_RetVal(FText, FStupidMenuElementTitleDelegate)
DECLARE_DELEGATE_OneParam(FStupidMenuElementCallbackDelegate, class UStupidMenuScreen*);

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
		TFunctionRef<void (class UStupidMenuScreen*)> OnClick);

	FStupidMenuElement(
		FText Title,
		TFunctionRef<void (class UStupidMenuScreen*)> OnClick);

	bool IsEmpty() const;
};
