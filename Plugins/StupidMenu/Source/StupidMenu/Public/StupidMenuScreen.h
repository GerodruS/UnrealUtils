#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StupidMenuScreen.generated.h"

UCLASS()
class STUPIDMENU_API UStupidMenuScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	void PushState(const struct FStupidMenuState& State);
	void PopState();
	void RedrawElements();
	void Close();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UUniformGridPanel* ButtonsGripPanel;

	UPROPERTY()
	TArray<class UStupidMenuButton*> MenuButtons;

	UPROPERTY()
	TArray<struct FStupidMenuState> StatesStack;

private:
	UFUNCTION()
	void OnButtonClick(const class UStupidMenuButton* const Button);

	bool GetCurrentState(FStupidMenuState*& CurrentState);
};
