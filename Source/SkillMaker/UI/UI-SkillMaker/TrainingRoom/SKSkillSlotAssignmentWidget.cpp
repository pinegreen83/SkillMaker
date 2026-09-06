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
	SK_LOG(LogSkillMaker, Log, TEXT("슬롯 할당 위젯 Initialize 시작: Widget=%s, WidgetTree=%s"),
		*GetName(), WidgetTree ? TEXT("Valid") : TEXT("None"));
	if (!Super::Initialize())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("슬롯 할당 위젯 Super::Initialize 실패."));
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
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &USKSkillSlotAssignmentWidget::OnCancelClicked);
	}

	SK_LOG(LogSkillMaker, Log,
		TEXT("슬롯 할당 위젯 Initialize 완료: Root=%s, Q=%s, E=%s, R=%s, F=%s, Cancel=%s"),
		RootBorder ? TEXT("Valid") : TEXT("None"), SkillSlotQButton ? TEXT("Valid") : TEXT("None"),
		SkillSlotEButton ? TEXT("Valid") : TEXT("None"), SkillSlotRButton ? TEXT("Valid") : TEXT("None"),
		SkillSlotFButton ? TEXT("Valid") : TEXT("None"), CancelButton ? TEXT("Valid") : TEXT("None"));

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
	SelectedSkillText->SetText(NSLOCTEXT("SkillSlot", "NoSelectedSkill", "할당할 스킬을 선택해 주세요."));
	SelectedSkillText->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	if (UVerticalBoxSlot* SelectedSlot = SlotList->AddChildToVerticalBox(SelectedSkillText))
	{
		SelectedSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	}

	SkillSlotQButton = CreateSlotButton(TEXT("SkillSlotQButton"), TEXT("SkillSlotQText"),
		NSLOCTEXT("SkillSlot", "EmptyQ", "Q 키에 할당"), SkillSlotQText);
	SkillSlotEButton = CreateSlotButton(TEXT("SkillSlotEButton"), TEXT("SkillSlotEText"),
		NSLOCTEXT("SkillSlot", "EmptyE", "E 키에 할당"), SkillSlotEText);
	SkillSlotRButton = CreateSlotButton(TEXT("SkillSlotRButton"), TEXT("SkillSlotRText"),
		NSLOCTEXT("SkillSlot", "EmptyR", "R 키에 할당"), SkillSlotRText);
	SkillSlotFButton = CreateSlotButton(TEXT("SkillSlotFButton"), TEXT("SkillSlotFText"),
		NSLOCTEXT("SkillSlot", "EmptyF", "F 키에 할당"), SkillSlotFText);

	for (UButton* SlotButton : { SkillSlotQButton.Get(), SkillSlotEButton.Get(), SkillSlotRButton.Get(), SkillSlotFButton.Get() })
	{
		if (UVerticalBoxSlot* ButtonSlot = SlotList->AddChildToVerticalBox(SlotButton))
		{
			ButtonSlot->SetPadding(FMargin(0.0f, 4.0f));
		}
	}

	TObjectPtr<UTextBlock> CancelText;
	CancelButton = CreateSlotButton(TEXT("CancelButton"), TEXT("CancelText"),
		NSLOCTEXT("SkillSlot", "SelectSkill", "스킬 선택"), CancelText);
	if (UVerticalBoxSlot* CancelSlot = SlotList->AddChildToVerticalBox(CancelButton))
	{
		CancelSlot->SetPadding(FMargin(0.0f, 12.0f, 0.0f, 0.0f));
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
	SetSlotButtonsEnabled(true);
	SK_LOG(LogSkillMaker, Log, TEXT("슬롯 할당 대상 설정: SkillID=%s, SkillName=%s, TextWidget=%s"),
		*SelectedSkillID.ToString(), *SkillData.SkillName, SelectedSkillText ? TEXT("Valid") : TEXT("None"));
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
			const FString DisplayName = SkillName.IsEmpty() ? TEXT("비어 있음") : SkillName;
			SlotText->SetText(FText::FromString(FString::Printf(TEXT("%s : %s"), SlotKeys[SlotIndex], *DisplayName)));
			SK_LOG(LogSkillMaker, Log, TEXT("슬롯 현황 표시 갱신: Key=%s, SkillName=%s"),
				SlotKeys[SlotIndex], *DisplayName);
		}
	}
}

void USKSkillSlotAssignmentWidget::ShowAssignmentOverview()
{
	SelectedSkillID = NAME_None;
	SetSlotButtonsEnabled(false);
	if (SelectedSkillText)
	{
		SelectedSkillText->SetText(NSLOCTEXT(
			"SkillSlot", "AssignmentOverview", "현재 스킬 매핑\n변경하려면 스킬 선택을 눌러 주세요."));
	}
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 매핑 현황 모드 설정 완료: 슬롯 버튼 입력 비활성화."));
}

void USKSkillSlotAssignmentWidget::BroadcastSlotSelection(int32 SlotIndex)
{
	SK_LOG(LogSkillMaker, Log, TEXT("슬롯 버튼 입력 수신: SlotIndex=%d, SelectedSkillID=%s"),
		SlotIndex, *SelectedSkillID.ToString());
	if (SelectedSkillID.IsNone())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("슬롯 할당 전에 스킬을 선택해야 함."));
		return;
	}

	OnSkillSlotSelected.Broadcast(SlotIndex);
	SK_LOG(LogSkillMaker, Log, TEXT("슬롯 선택 델리게이트 전송 완료: SlotIndex=%d"), SlotIndex);
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

void USKSkillSlotAssignmentWidget::SetSlotButtonsEnabled(bool bEnabled)
{
	for (UButton* SlotButton :
		{ SkillSlotQButton.Get(), SkillSlotEButton.Get(), SkillSlotRButton.Get(), SkillSlotFButton.Get() })
	{
		if (SlotButton)
		{
			SlotButton->SetIsEnabled(bEnabled);
		}
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

void USKSkillSlotAssignmentWidget::OnCancelClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 선택 버튼 클릭."));
	OnAssignmentCancelled.Broadcast();
}
