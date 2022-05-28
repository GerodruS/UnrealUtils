#include "StupidMenuScreen.h"

#include "StupidMenuButton.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

constexpr int32 RowCount = 3;
constexpr int32 ColumnCount = 5;
constexpr int32 OptionButtonsCount = 15;

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
	if (const int32 Index = MenuButtons.IndexOfByKey(Button);
		Index != INDEX_NONE)
	{
		UE_LOG(LogTemp, Log, TEXT("[TEST] UStupidMenuScreen::OnButtonClick button#%d Clicked!"), Index);
	}
}
