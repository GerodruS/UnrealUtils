#include "StupidMenuScreen.h"

#include "StupidMenuButton.h"
#include "StupidMenuElement.h"
#include "StupidMenuState.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

constexpr size_t RowCount = 5;
constexpr size_t ColumnCount = 3;
constexpr size_t OptionButtonsCount = RowCount * ColumnCount;

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

void UStupidMenuScreen::RedrawElements(const size_t NewIndexOffset)
{
	if (StatesStack.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("There is no state!"));
		return;
	}

	IndexOffset = NewIndexOffset;
	const FStupidMenuState& State = StatesStack.Last();
	const TArray<FStupidMenuElement>& CheatElements = State.Elements;

	const size_t ButtonsCount = MenuButtons.Num();
	const size_t Count = FMath::Min(ButtonsCount, static_cast<size_t>(CheatElements.Num()) - NewIndexOffset);
	for (size_t i = 0; i < Count; ++i)
	{
		const FStupidMenuElement& Element = CheatElements[i + NewIndexOffset];
		if (Element.IsEmpty())
		{
			MenuButtons[i]->Hide();
		}
		else
		{
			const FText Title = Element.OnGetTitle.Execute();
			MenuButtons[i]->SetText(Title);
		}
	}
	for (size_t i = Count; i < ButtonsCount; ++i)
	{
		MenuButtons[i]->Hide();
	}

	MenuButtons[0]->Select();
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

			const int32 Row = i / ColumnCount;
			const int32 InColumn = i % ColumnCount;
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
	SetVisibility(ESlateVisibility::Visible);
}

FReply UStupidMenuScreen::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (const float CurrentTime = GetWorld()->GetTimeSeconds();
		LastWheelTime <= CurrentTime)
	{
		constexpr float WheelTimeDelay = 0.25f;
		LastWheelTime = CurrentTime + WheelTimeDelay;

		const float WheelDelta = -InMouseEvent.GetWheelDelta();
		const size_t NewIndexOffset = ColumnCount * FMath::Sign(WheelDelta) + IndexOffset;
		if (CheckIndex(NewIndexOffset))
		{
			RedrawElements(NewIndexOffset);
		}
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UStupidMenuScreen::OnButtonClick(const UStupidMenuButton* const Button)
{
	using SizeType = TArray<UStupidMenuButton*>::SizeType;

	if (const SizeType Index = MenuButtons.IndexOfByKey(Button); Index != INDEX_NONE)
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

bool UStupidMenuScreen::CheckIndex(const size_t Index)
{
	FStupidMenuState* CurrentState;
	return GetCurrentState(CurrentState) &&
		   CurrentState->Elements.IsValidIndex(Index);
}
