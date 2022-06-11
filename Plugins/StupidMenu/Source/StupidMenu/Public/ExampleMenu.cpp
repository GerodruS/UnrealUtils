#include "ExampleMenu.h"

#include "StupidMenuScreen.h"
#include "StupidMenuState.h"

void FExampleMenu::Open(UWorld* World)
{
	// Switching to GameAndUI InputMode
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	World->GetFirstPlayerController()->SetInputMode(InputMode);
	World->GetFirstPlayerController()->SetShowMouseCursor(true);

	// Construct MenuState
	FStupidMenuState State(FText::FromString(TEXT("Main Menu")));

	//   Creating delegates from lambdas
	{
		FStupidMenuElementTitleDelegate HostTitleDelegate;
		HostTitleDelegate.BindLambda([]
		{
			return FText::FromString(TEXT("Host"));
		});

		FStupidMenuElementCallbackDelegate HostCallbackDelegate;
		HostCallbackDelegate.BindLambda([]
		{
			UE_LOG(LogTemp, Error, TEXT("HOST"));
		});

		State.Elements.Add(FStupidMenuElement(
			HostTitleDelegate,
			HostCallbackDelegate));
	}

	//   Creating a empty element -- it's a gap
	State.Elements.Add(FStupidMenuElement());

	//   Creating delegates from static functions
	{
		FStupidMenuElementTitleDelegate HostTitleDelegate;
		HostTitleDelegate.BindStatic(FExampleMenu::GetTitle);

		FStupidMenuElementCallbackDelegate HostCallbackDelegate;
		HostCallbackDelegate.BindStatic(FExampleMenu::OnClick);

		State.Elements.Add(FStupidMenuElement(
			HostTitleDelegate,
			HostCallbackDelegate));
	}

	// Creating MenuScreen
	UStupidMenuScreen* StupidMenuScreen = CreateWidget<UStupidMenuScreen>(World, UStupidMenuScreen::StaticClass());
	StupidMenuScreen->PushNewState(State);
	StupidMenuScreen->AddToViewport();
}

FText FExampleMenu::GetTitle()
{
	return FText::FromString(TEXT("Host 2"));
}

void FExampleMenu::OnClick()
{
	UE_LOG(LogTemp, Error, TEXT("HOST 2"));
}
