#include "StupidMenuButton.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"

void UStupidMenuButton::SetText(FText InText)
{
	Text->SetText(InText);
}

TSharedRef<SWidget> UStupidMenuButton::RebuildWidget()
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
		Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass());
		RootWidget->AddChild(Button);

		if (UCanvasPanelSlot* const ButtonPanelSlot = Cast<UCanvasPanelSlot>(Button->Slot);
			ButtonPanelSlot != nullptr)
		{
			ButtonPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			ButtonPanelSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}

		Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Text->SetJustification(ETextJustify::Center);
		Text->SetVisibility(ESlateVisibility::HitTestInvisible);
		RootWidget->AddChild(Text);

		if (UCanvasPanelSlot* const TextPanelSlot = Cast<UCanvasPanelSlot>(Text->Slot);
			TextPanelSlot != nullptr)
		{
			TextPanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
			TextPanelSlot->SetOffsets(FMargin(0.0f, 0.0f));
		}

		Text->SetText(FText::FromString(TEXT("TEST")));
	}

	return Widget;
}

void UStupidMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UStupidMenuButton::OnClick);
}

void UStupidMenuButton::OnClick()
{
	OnClicked.Execute(this);
}
