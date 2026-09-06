#include "SKSkillSlotAssignmentWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Logging/SKLogSkillMakerMacro.h"
#include "Skill/SKSkillData.h"

bool USKSkillSlotAssignmentWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	BuildNativeWidget();

	if (SkillSlotQButton)
	{
		SkillSlotQButton->OnClicked.AddDynamic(this, &USKSkillSlotAssignmentWidget::OnQSlotClicked);
	}
	if (SkillSlotEButton)
	{
		SkillSlotEButton->OnClicked.AddDynamic(this, &USKSkillSlotAssignmentWidget::OnESlotClicked);
	}
	if (SkillSlotRButton)
	{
		SkillSlotRButton->OnClicked.AddDynamic(this, &USKSkillSlotAssignmentWidget::OnRSlotClicked);
	}
	if (SkillSlotFButton)
	{
		SkillSlotFButton->OnClicked.AddDynamic(this, &USKSkillSlotAssignmentWidget::OnFSlotClicked);
	}

	return true;
}

void USKSkillSlotAssignmentWidget::BuildNativeWidget()
{
	RootBorder = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RootBorder"));
	RootBorder->SetBrushColor(FLinearColor(0.03f, 0.03f, 0.03f, 0.92f));
	RootBorder->SetPadding(FMargin(16.0f));

	UVerticalBox* SlotList = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SlotList"));
	RootBorder->SetContent(SlotList);
	WidgetTree->RootWidget = RootBorder;

	SelectedSkillText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("SelectedSkillText"));
	SelectedSkillText->SetText(NSLOCTEXT("SkillSlot", "NoSelectedSkill", "선택한 스킬: 없음"));
	SelectedSkillText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	if (UVerticalBoxSlot* SelectedSlot = SlotList->AddChildToVerticalBox(SelectedSkillText))
	{
		SelectedSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	SkillSlotQButton = CreateSlotButton(TEXT("SkillSlotQButton"), TEXT("SkillSlotQText"),
		NSLOCTEXT("SkillSlot", "EmptyQ", "Q : 비어 있음"), SkillSlotQText);
	SkillSlotEButton = CreateSlotButton(TEXT("SkillSlotEButton"), TEXT("SkillSlotEText"),
		NSLOCTEXT("SkillSlot", "EmptyE", "E : 비어 있음"), SkillSlotEText);
	SkillSlotRButton = CreateSlotButton(TEXT("SkillSlotRButton"), TEXT("SkillSlotRText"),
		NSLOCTEXT("SkillSlot", "EmptyR", "R : 비어 있음"), SkillSlotRText);
	SkillSlotFButton = CreateSlotButton(TEXT("SkillSlotFButton"), TEXT("SkillSlotFText"),
		NSLOCTEXT("SkillSlot", "EmptyF", "F : 비어 있음"), SkillSlotFText);

	for (UButton* SlotButton : { SkillSlotQButton.Get(), SkillSlotEButton.Get(), SkillSlotRButton.Get(), SkillSlotFButton.Get() })
	{
		if (UVerticalBoxSlot* ButtonSlot = SlotList->AddChildToVerticalBox(SlotButton))
		{
			ButtonSlot->SetPadding(FMargin(0.0f, 4.0f));
		}
	}
}

UButton* USKSkillSlotAssignmentWidget::CreateSlotButton(const FName& ButtonName, const FName& TextName,
	const FText& InitialText, TObjectPtr<UTextBlock>& OutTextBlock)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), ButtonName);
	OutTextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TextName);
	OutTextBlock->SetText(InitialText);
	OutTextBlock->SetJustification(ETextJustify::Center);
	Button->AddChild(OutTextBlock);
	return Button;
}

void USKSkillSlotAssignmentWidget::SetSelectedSkill(const FSKSkillData& SkillData)
{
	SelectedSkillID = SkillData.SkillID;
	if (SelectedSkillText)
	{
		SelectedSkillText->SetText(FText::Format(
			NSLOCTEXT("SkillSlot", "SelectedSkill", "선택한 스킬: {0}"),
			FText::FromString(SkillData.SkillName)));
	}
}

void USKSkillSlotAssignmentWidget::SetAssignedSkill(int32 SlotIndex, const FString& SkillName)
{
	if (UTextBlock* SlotText = GetSlotText(SlotIndex))
	{
		static const TCHAR* SlotKeys[] = { TEXT("Q"), TEXT("E"), TEXT("R"), TEXT("F") };
		if (SlotIndex >= 0 && SlotIndex < UE_ARRAY_COUNT(SlotKeys))
		{
			SlotText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), SlotKeys[SlotIndex], *SkillName)));
		}
	}
}

void USKSkillSlotAssignmentWidget::BroadcastSlotSelection(int32 SlotIndex)
{
	if (SelectedSkillID.IsNone())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("슬롯 할당 전에 스킬을 선택해야 함."));
		return;
	}

	OnSkillSlotSelected.Broadcast(SlotIndex);
}

UTextBlock* USKSkillSlotAssignmentWidget::GetSlotText(int32 SlotIndex) const
{
	switch (SlotIndex)
	{
	case 0:
		return SkillSlotQText;
	case 1:
		return SkillSlotEText;
	case 2:
		return SkillSlotRText;
	case 3:
		return SkillSlotFText;
	default:
		return nullptr;
	}
}

void USKSkillSlotAssignmentWidget::OnQSlotClicked()
{
	BroadcastSlotSelection(0);
}

void USKSkillSlotAssignmentWidget::OnESlotClicked()
{
	BroadcastSlotSelection(1);
}

void USKSkillSlotAssignmentWidget::OnRSlotClicked()
{
	BroadcastSlotSelection(2);
}

void USKSkillSlotAssignmentWidget::OnFSlotClicked()
{
	BroadcastSlotSelection(3);
}
