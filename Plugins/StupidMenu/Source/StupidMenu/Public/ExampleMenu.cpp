﻿#include "ExampleMenu.h"

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
	FStupidMenuState MainMenuState(FText::FromString(TEXT("Main Menu")));

	//   Creating an element from delegates
	FStupidMenuElementTitleDelegate HostTitleDelegate;
	HostTitleDelegate.BindLambda([]
	{
		return FText::FromString(TEXT("New Game"));
	});
	FStupidMenuElementCallbackDelegate HostCallbackDelegate;
	HostCallbackDelegate.BindLambda([] (UStupidMenuScreen* MenuScreen)
	{
		FStupidMenuState NewGameState(FText::FromString(TEXT("New Game")));
		NewGameState.Elements.Add(FStupidMenuElement(
				FText::FromString(TEXT("Easy")),
				[] (UStupidMenuScreen* _)
				{
					UE_LOG(LogTemp, Log, TEXT("'Easy' Button Callback"));
				}));
		NewGameState.Elements.Add(FStupidMenuElement(
				FText::FromString(TEXT("Medium")),
				[] (UStupidMenuScreen* _)
				{
					UE_LOG(LogTemp, Log, TEXT("'Medium' Button Callback"));
				}));
		NewGameState.Elements.Add(FStupidMenuElement(
				FText::FromString(TEXT("Rare (:")),
				[] (UStupidMenuScreen* _)
				{
					UE_LOG(LogTemp, Log, TEXT("'Rare' Button Callback"));
				}));
		NewGameState.Elements.Add(FStupidMenuElement::Empty);
		NewGameState.Elements.Add(FStupidMenuElement(
				FText::FromString(TEXT("Back")),
				[] (UStupidMenuScreen* MenuScreen)
				{
					MenuScreen->PopState();
				}));

		MenuScreen->PushState(NewGameState);
	});
	MainMenuState.Elements.Add(FStupidMenuElement(HostTitleDelegate, HostCallbackDelegate));

	//   Adding an empty element -- it's a gap
	MainMenuState.Elements.Add(FStupidMenuElement::Empty);
	MainMenuState.Elements.Add(FStupidMenuElement::Empty);

	//   Creating an element from static functions references
	MainMenuState.Elements.Add(FStupidMenuElement(
		FExampleMenu::GetTitle,
		FExampleMenu::OnClick));

	//   Creating an element from lambdas
	MainMenuState.Elements.Add(FStupidMenuElement(
		[] {
			return FText::FromString(TEXT("Options"));
		},
		[] (UStupidMenuScreen* MenuScreen){
			UE_LOG(LogTemp, Error, TEXT("Options Callback"));
		}));

	//   Creating an element from a FText instance and a lambda callback
	MainMenuState.Elements.Add(FStupidMenuElement(
		FText::FromString(TEXT("Exit")),
		[] (UStupidMenuScreen* MenuScreen)
		{
			UE_LOG(LogTemp, Error, TEXT("Quitting the Game..."));
			// TODO: implementation
			// UKismetSystemLibrary::QuitGame();
		}));

	// Creating MenuScreen
	UStupidMenuScreen* StupidMenuScreen = CreateWidget<UStupidMenuScreen>(World, UStupidMenuScreen::StaticClass());
	StupidMenuScreen->AddToViewport();
	StupidMenuScreen->PushState(MainMenuState);
}

FText FExampleMenu::GetTitle()
{
	return FText::FromString(TEXT("Static Functions Option Name"));
}

void FExampleMenu::OnClick(UStupidMenuScreen* MenuScreen)
{
	UE_LOG(LogTemp, Error, TEXT("Static Functions Callback"));
}
