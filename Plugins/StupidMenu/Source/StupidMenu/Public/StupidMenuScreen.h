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
	void RedrawElements(const size_t NewIndexOffset = 0);
	void Close();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY()
	class UUniformGridPanel* ButtonsGripPanel;

	UPROPERTY()
	TArray<class UStupidMenuButton*> MenuButtons;

	UPROPERTY()
	TArray<struct FStupidMenuState> StatesStack;

	size_t IndexOffset = 0;
	float LastWheelTime = 0.0f;

private:
	UFUNCTION()
	void OnButtonClick(const class UStupidMenuButton* const Button);

	bool GetCurrentState(FStupidMenuState*& CurrentState);
	bool CheckIndex(const size_t Index);
};
