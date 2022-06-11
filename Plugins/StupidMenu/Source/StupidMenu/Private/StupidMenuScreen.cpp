#include "StupidMenuScreen.h"

#include "StupidMenuButton.h"
#include "StupidMenuElement.h"
#include "StupidMenuState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

constexpr int32 RowCount = 3;
constexpr int32 ColumnCount = 5;
constexpr int32 OptionButtonsCount = 15;

void UStupidMenuScreen::PushState(const FStupidMenuState& State)
{
	StatesStack.Add(State);

	// // TODO: whole path, not only title
	// PathText->SetText(State.Title);

	RedrawElements();
}

void UStupidMenuScreen::PopState()
{
	if (StatesStack.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("There is no state!"));
		return;
	}

	using SizeType = TArray<FStupidMenuState>::SizeType;

	const SizeType LastIndex = StatesStack.Num() - 1;
	StatesStack.RemoveAt(LastIndex, 1, false);

	if (StatesStack.IsEmpty())
	{
		Close();
	}
	else
	{
		RedrawElements();
	}
}

void UStupidMenuScreen::Close()
{
	// TODO: implementation
	UE_LOG(LogTemp, Error, TEXT("UStupidMenuScreen::Close"));
}

void UStupidMenuScreen::RedrawElements()
{
	if (StatesStack.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("There is no state!"));
		return;
	}

	const FStupidMenuState& State = StatesStack.Last();
	const TArray<FStupidMenuElement>& CheatElements = State.Elements;

	const int32 ButtonsCount = MenuButtons.Num();
	const int32 Count = FMath::Min(ButtonsCount, CheatElements.Num());
	for (int32 i = 0; i < Count; ++i)
	{
		const FStupidMenuElement& Element = CheatElements[i];
		UStupidMenuButton* const MenuButton = MenuButtons[i];
		if (Element.IsEmpty())
		{
			MenuButton->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			const FText Title = Element.OnGetTitle.Execute();
			MenuButton->SetText(Title);
			MenuButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
	for (int32 i = Count; i < ButtonsCount; ++i)
	{
		UStupidMenuButton* const MenuButton = MenuButtons[i];
		MenuButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

TSharedRef<SWidget> UStupidMenuScreen::RebuildWidget()
{
	UPanelWidget* RootWidget = Cast<UPanelWidget>(GetRootWidget());
	if (RootWidget == nullptr)
	{
		RootWidget = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass());
		WidgetTree->RootWidget = RootWidget;
	}

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget != nullptr)
	{
		ButtonsGripPanel = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass());
		RootWidget->AddChild(ButtonsGripPanel);
		if (UCanvasPanelSlot* const GridPanelSlot = Cast<UCanvasPanelSlot>(ButtonsGripPanel->Slot);
			GridPanelSlot != nullptr)
		{
			GridPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			GridPanelSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}

		MenuButtons.Reserve(OptionButtonsCount);
		for (int32 i = 0; i < OptionButtonsCount; ++i)
		{
			UStupidMenuButton* const ButtonWithText = CreateWidget<UStupidMenuButton>(this, UStupidMenuButton::StaticClass());
			ButtonWithText->OnClicked.BindUObject(this, &UStupidMenuScreen::OnButtonClick);

			MenuButtons.Add(ButtonWithText);

			const int32 Row = i / RowCount;
			const int32 InColumn = i % RowCount;
			UUniformGridSlot* const GridSlot = ButtonsGripPanel->AddChildToUniformGrid(ButtonWithText, Row, InColumn);
			GridSlot->SetHorizontalAlignment(HAlign_Fill);
			GridSlot->SetVerticalAlignment(VAlign_Fill);
		}
	}

	return Widget;
}

void UStupidMenuScreen::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStupidMenuScreen::OnButtonClick(const UStupidMenuButton* const Button)
{
	if (const int32 Index = MenuButtons.IndexOfByKey(Button); Index != INDEX_NONE)
	{
		if (FStupidMenuState* CurrentState; GetCurrentState(CurrentState))
		{
			if (CurrentState->Elements.IsValidIndex(Index))
			{
				if (const FStupidMenuElement& Element = CurrentState->Elements[Index]; Element.OnClick.IsBound())
				{
					Element.OnClick.Execute(this);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Element OnClick delegate has no bound!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Invalid element click!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("There is no state!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Button not found!"));
	}
}

bool UStupidMenuScreen::GetCurrentState(FStupidMenuState*& CurrentState)
{
	if (StatesStack.IsEmpty())
	{
		CurrentState = nullptr;
		return false;
	}
	else
	{
		CurrentState = &StatesStack.Last();
		return true;
	}
}
