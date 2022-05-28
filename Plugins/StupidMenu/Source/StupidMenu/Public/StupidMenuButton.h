#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StupidMenuButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnStupidMenuButtonClickedEvent, const class UStupidMenuButton* const);

UCLASS()
class STUPIDMENU_API UStupidMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnStupidMenuButtonClickedEvent OnClicked;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UButton* Button;

	UPROPERTY()
	class UTextBlock* Text;

private:
	UFUNCTION()
	void OnClick();

};
