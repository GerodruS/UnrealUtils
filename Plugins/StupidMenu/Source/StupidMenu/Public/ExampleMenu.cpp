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

	//   Creating an element from delegates
	FStupidMenuElementTitleDelegate HostTitleDelegate;
	HostTitleDelegate.BindLambda([] {
		return FText::FromString(TEXT("Delegates Option Name"));
	});
	FStupidMenuElementCallbackDelegate HostCallbackDelegate;
	HostCallbackDelegate.BindLambda([] {
		UE_LOG(LogTemp, Error, TEXT("Delegates Callback"));
	});
	State.Elements.Add(FStupidMenuElement(HostTitleDelegate, HostCallbackDelegate));

	//   Adding an empty element -- it's a gap
	State.Elements.Add(FStupidMenuElement::Empty);

	//   Creating an element from static functions references
	State.Elements.Add(FStupidMenuElement(
		FExampleMenu::GetTitle,
		FExampleMenu::OnClick));

	//   Creating an element from lambdas
	State.Elements.Add(FStupidMenuElement(
		[] {
			return FText::FromString(TEXT("Lambdas Option Name"));
		},
		[] {
			UE_LOG(LogTemp, Error, TEXT("Lambdas Callback"));
		}));

	//   Creating an element from a FText instance and a lambda callback
	State.Elements.Add(FStupidMenuElement(
		FText::FromString(TEXT("FText instance Option Name")),
		[]
		{
			UE_LOG(LogTemp, Error, TEXT("FText instance Callback"));
		}));

	// Creating MenuScreen
	UStupidMenuScreen* StupidMenuScreen = CreateWidget<UStupidMenuScreen>(World, UStupidMenuScreen::StaticClass());
	StupidMenuScreen->AddToViewport();
	StupidMenuScreen->PushNewState(State);
}

FText FExampleMenu::GetTitle()
{
	return FText::FromString(TEXT("Static Functions Option Name"));
}

void FExampleMenu::OnClick()
{
	UE_LOG(LogTemp, Error, TEXT("Static Functions Callback"));
}
