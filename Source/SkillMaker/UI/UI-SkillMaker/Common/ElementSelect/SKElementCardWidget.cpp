#include "SKElementCardWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/ButtonSlot.h"
#include "Components/CheckBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"

bool USKElementCardWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	BuildDefaultWidgetTree();
	if (ElementButton)
	{
		ElementButton->OnClicked.AddUniqueDynamic(this, &USKElementCardWidget::OnElementButtonClicked);
	}

	return true;
}

void USKElementCardWidget::BuildDefaultWidgetTree()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	ElementButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("ElementButton"));
	WidgetTree->RootWidget = ElementButton;

	UHorizontalBox* Layout = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ElementLayout"));
	Layout->SetVisibility(ESlateVisibility::HitTestInvisible);
	if (UButtonSlot* ButtonSlot = Cast<UButtonSlot>(ElementButton->AddChild(Layout)))
	{
		ButtonSlot->SetPadding(FMargin(12.0f, 8.0f));
		ButtonSlot->SetHorizontalAlignment(HAlign_Fill);
	}

	ElementCheckBox = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass(), TEXT("ElementCheckBox"));
	Layout->AddChildToHorizontalBox(ElementCheckBox);

	ElementNameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ElementNameText"));
	ElementNameText->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	ElementNameText->SetJustification(ETextJustify::Left);
	if (UHorizontalBoxSlot* NameSlot = Layout->AddChildToHorizontalBox(ElementNameText))
	{
		NameSlot->SetPadding(FMargin(10.0f, 0.0f, 0.0f, 0.0f));
		NameSlot->SetVerticalAlignment(VAlign_Center);
	}
}

void USKElementCardWidget::SetElementInfo(FGameplayTag InElementTag, const FText& InDisplayName,
	const FText& InDescription, const FLinearColor& InDisplayColor, bool bIsSelected)
{
	ElementTag = InElementTag;
	ElementColor = InDisplayColor;

	if (ElementNameText)
	{
		ElementNameText->SetText(InDisplayName);
	}
	if (WidgetTree)
	{
		UBorder* ToolTipBackground = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("ElementToolTipBackground"));
		ToolTipBackground->SetPadding(FMargin(12.0f, 8.0f));
		ToolTipBackground->SetBrushColor(FLinearColor(0.03f, 0.03f, 0.03f, 0.95f));

		UTextBlock* ToolTipText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ElementToolTipText"));
		ToolTipText->SetText(InDescription);
		ToolTipText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		ToolTipText->SetAutoWrapText(true);
		ToolTipText->SetWrapTextAt(420.0f);
		FSlateFontInfo ToolTipFont = ToolTipText->GetFont();
		ToolTipFont.Size = 15;
		ToolTipText->SetFont(ToolTipFont);

		ToolTipBackground->SetContent(ToolTipText);
		SetToolTip(ToolTipBackground);
	}
	SetSelected(bIsSelected);
}

void USKElementCardWidget::SetSelected(bool bIsSelected)
{
	bUpdatingSelection = true;
	if (ElementCheckBox)
	{
		ElementCheckBox->SetIsChecked(bIsSelected);
	}
	if (ElementButton)
	{
		ElementButton->SetBackgroundColor(bIsSelected ? ElementColor : FLinearColor::White);
	}
	bUpdatingSelection = false;
}

FGameplayTag USKElementCardWidget::GetElementTag() const
{
	return ElementTag;
}

void USKElementCardWidget::OnElementButtonClicked()
{
	if (!bUpdatingSelection && ElementTag.IsValid() && ElementCheckBox)
	{
		OnElementSelectionChanged.Broadcast(ElementTag, !ElementCheckBox->IsChecked());
	}
}
