// Fill out your copyright notice in the Description page of Project Settings.


#include "SKSkillDetailWidget.h"
#include "UI/UI-SkillMaker/Common/ProjectileSelect/SKProjectileSelectionWidget.h"
#include "UI/UI-SkillMaker/Common/AnimationSelect/SKAnimNotifySelectionWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UI-SkillMaker/UEEditor/SKSkillMakerEditorHUD.h"
#include "UI/UI-SkillMaker/TrainingRoom/SKSkillMakerTrainHUD.h"
#include "UI/UI-SkillMaker/Common/ElementSelect/SKElementCardWidget.h"
#include "Data/SKElementData.h"
#include "Components/ScrollBox.h"
#include "Components/ScrollBoxSlot.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Logging/SKLogSkillMakerMacro.h"

namespace
{
FSKElementData MakeElementData(const TCHAR* TagName, const FText& DisplayName,
	const FText& Description, const FLinearColor& DisplayColor)
{
	FSKElementData Result;
	Result.ElementTag = FGameplayTag::RequestGameplayTag(FName(TagName), false);
	Result.DisplayName = DisplayName;
	Result.Description = Description;
	Result.DisplayColor = DisplayColor;
	return Result;
}

TArray<FSKElementData> GetPrototypeElementList()
{
	return {
		MakeElementData(TEXT("Element.Fire"), NSLOCTEXT("SkillElement", "FireName", "화염"),
			NSLOCTEXT("SkillElement", "FireDescription", "열과 연소의 성질을 가진 속성입니다."), FLinearColor(0.9f, 0.15f, 0.05f, 1.0f)),
		MakeElementData(TEXT("Element.Water"), NSLOCTEXT("SkillElement", "WaterName", "물"),
			NSLOCTEXT("SkillElement", "WaterDescription", "흐름과 유동의 성질을 가진 속성입니다."), FLinearColor(0.05f, 0.35f, 0.95f, 1.0f)),
		MakeElementData(TEXT("Element.Wind"), NSLOCTEXT("SkillElement", "WindName", "바람"),
			NSLOCTEXT("SkillElement", "WindDescription", "속도와 밀어내기의 성질을 가진 속성입니다."), FLinearColor(0.2f, 0.8f, 0.55f, 1.0f)),
		MakeElementData(TEXT("Element.Earth"), NSLOCTEXT("SkillElement", "EarthName", "대지"),
			NSLOCTEXT("SkillElement", "EarthDescription", "무게와 충격의 성질을 가진 속성입니다."), FLinearColor(0.55f, 0.3f, 0.1f, 1.0f))
	};
}

FString GetTabSwitcherChildrenDescription(const UWidgetSwitcher* TabSwitcher)
{
	if (!TabSwitcher)
	{
		return TEXT("TabSwitcher=null");
	}

	TArray<FString> ChildDescriptions;
	for (int32 Index = 0; Index < TabSwitcher->GetNumWidgets(); ++Index)
	{
		ChildDescriptions.Add(FString::Printf(TEXT("%d:%s"), Index, *GetNameSafe(TabSwitcher->GetWidgetAtIndex(Index))));
	}
	return FString::Join(ChildDescriptions, TEXT(", "));
}
}

void USKSkillDetailWidget::NativeConstruct()
{
	Super::NativeConstruct();
	ConfigureGeneralTabScrolling();

	if (GeneralTabButton)
	{
		GeneralTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnGeneralTabClicked);
	}

	if (StatusEffectTabButton)
	{
		StatusEffectTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnElementTabClicked);
	}
	if (StatusEffectTabButtonText)
	{
		StatusEffectTabButtonText->SetText(NSLOCTEXT("SkillDetail", "ElementTabButton", "속성"));
	}
	if (StatusEffectTabText)
	{
		StatusEffectTabText->SetText(NSLOCTEXT("SkillDetail", "ElementTabTitle", "스킬 속성 선택"));
	}

	if (EffectSoundTabButton)
	{
		EffectSoundTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnEffectSoundTabClicked);
	}

	if (AnimNotifyTabButton)
	{
		AnimNotifyTabButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnAnimNotifyTabClicked);
	}

	if(SkillTypeComboBox)
	{
		SkillTypeComboBox->OnSelectionChanged.AddDynamic(this, &USKSkillDetailWidget::OnSkillTypeChanged);
	}

	if (DamageTextBox)
	{
		DamageTextBox->OnTextChanged.AddUniqueDynamic(this, &USKSkillDetailWidget::OnDamageTextChanged);
		SK_LOG(LogSkillMaker, Log, TEXT("데미지 입력 바인딩 완료: Widget=%s / CurrentText=%s"),
			*DamageTextBox->GetName(), *DamageTextBox->GetText().ToString());
	}
	else
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("데미지 입력 바인딩 생략: DamageTextBox가 위젯에 없음."));
	}

	if (MinRangeSlider) MinRangeSlider->SetVisibility(ESlateVisibility::Collapsed);
	if (MaxRangeSlider) MaxRangeSlider->SetVisibility(ESlateVisibility::Collapsed);
	if (MinRangeText) MinRangeText->SetVisibility(ESlateVisibility::Collapsed);
	if (MaxRangeText) MaxRangeText->SetVisibility(ESlateVisibility::Collapsed);

	if (PreviewSkillButton)
	{
		PreviewSkillButton->OnClicked.AddDynamic(this, &USKSkillDetailWidget::OnPreviewSkillClicked);
	}

	if (ProjectileSelectionWidget)
	{
		ProjectileSelectionWidget->OnProjectileSelected.AddDynamic(this, &USKSkillDetailWidget::OnProjectileSelected);
	}

	if (AnimNotifySelectionWidget)
	{
		AnimNotifySelectionWidget->OnAnimNotifySelected.AddDynamic(this, &USKSkillDetailWidget::OnNotifySelected);
	}

	PopulateElementList();
	SK_LOG(LogSkillMaker, Log,
		TEXT("탭 초기화: General=%s / Element=%s / EffectSound=%s / Trigger=%s / 자식=[%s]"),
		*GetNameSafe(GeneralTabPanel),
		*GetNameSafe(StatusEffectTabPanel),
		*GetNameSafe(ProjectileTabPanel),
		*GetNameSafe(AnimNotifyPanel),
		*GetTabSwitcherChildrenDescription(TabSwitcher));
	SelectTab(GeneralTabPanel, GeneralTabButton, TEXT("기본(초기화)"));
}

void USKSkillDetailWidget::ConfigureGeneralTabScrolling()
{
	if (!TabSwitcher || !GeneralTabPanel || !GeneralTabScrollBox)
	{
		SK_LOG(LogSkillMaker, Warning,
			TEXT("기본 탭 스크롤 구성 실패: TabSwitcher=%s / GeneralTabPanel=%s / GeneralTabScrollBox=%s"),
			*GetNameSafe(TabSwitcher),
			*GetNameSafe(GeneralTabPanel),
			*GetNameSafe(GeneralTabScrollBox));
		return;
	}

	if (GeneralTabScrollBox->GetParent() != GeneralTabPanel)
	{
		SK_LOG(LogSkillMaker, Warning,
			TEXT("기본 탭 스크롤 계층 불일치: ScrollBox 부모=%s / 기대 부모=%s"),
			*GetNameSafe(GeneralTabScrollBox->GetParent()),
			*GetNameSafe(GeneralTabPanel));
		return;
	}

	if (GeneralTabScrollBox->GetChildrenCount() == 0)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("기본 탭 스크롤 구성 실패: GeneralTabScrollBox 안에 콘텐츠가 없음."));
		return;
	}

	GeneralTabScrollBox->SetOrientation(Orient_Vertical);
	GeneralTabScrollBox->SetScrollBarVisibility(ESlateVisibility::Visible);
	GeneralTabScrollBox->SetAlwaysShowScrollbar(true);
	GeneralTabScrollBox->SetAnimateWheelScrolling(true);
	GeneralTabScrollBox->SetScrollbarThickness(FVector2D(8.0f, 8.0f));

	UWidget* GeneralTabContent = GeneralTabScrollBox->GetChildAt(0);
	if (UScrollBoxSlot* GeneralTabContentSlot = Cast<UScrollBoxSlot>(GeneralTabContent->Slot))
	{
		GeneralTabContentSlot->SetPadding(FMargin(0.0f, 24.0f, 12.0f, 20.0f));
		GeneralTabContentSlot->SetHorizontalAlignment(HAlign_Fill);
	}

	SK_LOG(LogSkillMaker, Log,
		TEXT("기본 탭 스크롤 구성 완료: Panel=%s / ScrollBox=%s / Content=%s / 탭 자식=[%s]"),
		*GetNameSafe(GeneralTabPanel),
		*GetNameSafe(GeneralTabScrollBox),
		*GetNameSafe(GeneralTabContent),
		*GetTabSwitcherChildrenDescription(TabSwitcher));
}

void USKSkillDetailWidget::SelectTab(UWidget* TabContent, UButton* SelectedTabButton, const TCHAR* TabDisplayName)
{
	if (!TabSwitcher || !TabContent)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("탭 선택 실패: %s / TabSwitcher=%s / TabContent=%s"),
			TabDisplayName,
			TabSwitcher ? TEXT("Valid") : TEXT("Null"),
			TabContent ? TEXT("Valid") : TEXT("Null"));
		return;
	}

	UWidget* ActiveContent = TabContent;

	SK_LOG(LogSkillMaker, Log,
		TEXT("탭 전환 요청: %s / 이전 활성=%s(%d) / 요청 위젯=%s / 자식=[%s]"),
		TabDisplayName,
		*GetNameSafe(TabSwitcher->GetActiveWidget()),
		TabSwitcher->GetActiveWidgetIndex(),
		*GetNameSafe(ActiveContent),
		*GetTabSwitcherChildrenDescription(TabSwitcher));

	TabSwitcher->SetActiveWidget(ActiveContent);
	UpdateTabButtonSelection(SelectedTabButton);
	LogTabState(TEXT("C++ 전환 직후"), TabDisplayName, ActiveContent);

	if (UWorld* World = GetWorld())
	{
		const FString RequestedTabName(TabDisplayName);
		const TWeakObjectPtr<UWidget> RequestedContent(ActiveContent);
		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateWeakLambda(this,
			[this, RequestedTabName, RequestedContent]()
			{
				LogTabState(TEXT("다음 프레임 최종"), RequestedTabName, RequestedContent.Get());
			}));
	}
}

void USKSkillDetailWidget::LogTabState(const TCHAR* Phase, const FString& RequestedTabName,
	const UWidget* RequestedContent) const
{
	if (!TabSwitcher)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("탭 상태 확인 실패: %s / %s / TabSwitcher=null"),
			Phase, *RequestedTabName);
		return;
	}

	const UWidget* CurrentActiveWidget = TabSwitcher->GetActiveWidget();
	const bool bMatchesRequest = CurrentActiveWidget == RequestedContent;
	if (bMatchesRequest)
	{
		SK_LOG(LogSkillMaker, Log,
			TEXT("탭 상태 정상: %s / 요청=%s / 활성=%s / 인덱스=%d"),
			Phase, *RequestedTabName, *GetNameSafe(CurrentActiveWidget), TabSwitcher->GetActiveWidgetIndex());
	}
	else
	{
		SK_LOG(LogSkillMaker, Warning,
			TEXT("탭 상태 불일치: %s / 요청=%s(%s) / 실제=%s / 인덱스=%d"),
			Phase,
			*RequestedTabName,
			*GetNameSafe(RequestedContent),
			*GetNameSafe(CurrentActiveWidget),
			TabSwitcher->GetActiveWidgetIndex());
	}
}

void USKSkillDetailWidget::UpdateTabButtonSelection(UButton* SelectedTabButton)
{
	const FLinearColor SelectedColor(0.15f, 0.55f, 1.0f, 1.0f);
	const FLinearColor UnselectedColor = FLinearColor::White;
	const TArray<UButton*> TabButtons = {
		GeneralTabButton,
		StatusEffectTabButton,
		EffectSoundTabButton,
		AnimNotifyTabButton
	};

	for (UButton* TabButton : TabButtons)
	{
		if (TabButton)
		{
			TabButton->SetBackgroundColor(TabButton == SelectedTabButton ? SelectedColor : UnselectedColor);
		}
	}
}

void USKSkillDetailWidget::SetSkillMakerEditorHUD(ASKSkillMakerEditorHUD* InHUD)
{
	if (SkillMakerEditorHUDReference)
	{
		OnSkillDetailChanged.RemoveDynamic(SkillMakerEditorHUDReference, &ASKSkillMakerEditorHUD::SetCurrentSkillData);
	}

	SkillMakerEditorHUDReference = InHUD;

	if(SkillMakerEditorHUDReference)
	{
		OnSkillDetailChanged.AddUniqueDynamic(SkillMakerEditorHUDReference, &ASKSkillMakerEditorHUD::SetCurrentSkillData);
		InitializeFromSkillData();
	}
}

void USKSkillDetailWidget::OnGeneralTabClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("기본 탭 버튼 클릭 / 클릭 직전 활성=%s(%d)"),
		*GetNameSafe(TabSwitcher ? TabSwitcher->GetActiveWidget() : nullptr),
		TabSwitcher ? TabSwitcher->GetActiveWidgetIndex() : INDEX_NONE);
	SelectTab(GeneralTabPanel, GeneralTabButton, TEXT("기본"));
}

void USKSkillDetailWidget::OnElementTabClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("속성 탭 버튼 클릭 / 클릭 직전 활성=%s(%d)"),
		*GetNameSafe(TabSwitcher ? TabSwitcher->GetActiveWidget() : nullptr),
		TabSwitcher ? TabSwitcher->GetActiveWidgetIndex() : INDEX_NONE);
	SelectTab(StatusEffectTabPanel, StatusEffectTabButton, TEXT("속성"));
}

void USKSkillDetailWidget::OnEffectSoundTabClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("이펙트-사운드 탭 버튼 클릭 / 클릭 직전 활성=%s(%d)"),
		*GetNameSafe(TabSwitcher ? TabSwitcher->GetActiveWidget() : nullptr),
		TabSwitcher ? TabSwitcher->GetActiveWidgetIndex() : INDEX_NONE);
	SelectTab(ProjectileTabPanel, EffectSoundTabButton, TEXT("이펙트-사운드"));
}

void USKSkillDetailWidget::OnAnimNotifyTabClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("트리거 탭 버튼 클릭 / 클릭 직전 활성=%s(%d)"),
		*GetNameSafe(TabSwitcher ? TabSwitcher->GetActiveWidget() : nullptr),
		TabSwitcher ? TabSwitcher->GetActiveWidgetIndex() : INDEX_NONE);
	SelectTab(AnimNotifyPanel, AnimNotifyTabButton, TEXT("트리거"));
	PopulateAnimNotifyList();
}

void USKSkillDetailWidget::InitializeFromSkillData()
{
	if(!SkillMakerEditorHUDReference)
		return;

	PopularSkillDetails();
	PopulateElementList();

	if (ProjectileSelectionWidget)
	{
		const FSKSkillData& SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
		ProjectileSelectionWidget->SetProjectileCard(SkillData.ProjectileActor, SkillData.ElementTag);
	}
}

void USKSkillDetailWidget::PopularSkillDetails()
{
	if (!SkillMakerEditorHUDReference)
		return;

	const FSKSkillData& SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();

	if(SkillTypeComboBox)
	{
		switch (SkillData.SkillType)
		{
		case ESkillType::Attack:
			SkillTypeComboBox->SetSelectedOption("Attack");
			break;
		case ESkillType::Buff:
			SkillTypeComboBox->SetSelectedOption("Buff");
			break;
		case ESkillType::Debuff:
			SkillTypeComboBox->SetSelectedOption("Debuff");
			break;
		}
	}

	if (DamageTextBox)
	{
		DamageTextBox->SetText(FText::AsNumber(SkillData.DamageValue));
		SK_LOG(LogSkillMaker, Log, TEXT("데미지 입력값 복원: SkillID=%s / DamageValue=%.2f"),
			*SkillData.SkillID.ToString(), SkillData.DamageValue);
	}
}

void USKSkillDetailWidget::PopulateElementList()
{
	if (!StatusEffectListBox)
		return;

	StatusEffectListBox->ClearChildren();
	ElementCards.Reset();
	const FGameplayTag CurrentElement = SkillMakerEditorHUDReference
		? SkillMakerEditorHUDReference->GetCurrentSkillData().ElementTag
		: FGameplayTag();

	for (const FSKElementData& ElementData : GetPrototypeElementList())
	{
		if (!ElementData.ElementTag.IsValid())
		{
			continue;
		}

		USKElementCardWidget* ElementCard = CreateWidget<USKElementCardWidget>(this, USKElementCardWidget::StaticClass());
		if (!ElementCard) continue;

		ElementCard->SetElementInfo(ElementData.ElementTag, ElementData.DisplayName, ElementData.Description,
			ElementData.DisplayColor, ElementData.ElementTag == CurrentElement);
		ElementCard->OnElementSelectionChanged.AddDynamic(this, &USKSkillDetailWidget::OnElementSelectionChanged);
		ElementCards.Add(ElementCard);
		StatusEffectListBox->AddChild(ElementCard);
	}
}

void USKSkillDetailWidget::PopulateAnimNotifyList()
{
	if (!AnimNotifySelectionWidget || !SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	UAnimMontage* Montage = SkillData.SkillMontage.LoadSynchronous();
	if (Montage)
	{
		SkillData.SkillDuration = Montage->GetPlayLength();
		OnSkillDetailChanged.Broadcast(SkillData);
	}
	AnimNotifySelectionWidget->PopulateNotifyList(Montage, SkillData.NotifyName);
}

void USKSkillDetailWidget::SetSkillMakerTrainHUD(ASKSkillMakerTrainHUD* InHUD)
{
	SkillMakerTrainHUDReference = InHUD;

	if(SkillMakerTrainHUDReference)
	{
		InitializeFromSkillData();
	}
}

void USKSkillDetailWidget::OnSkillTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (!SkillMakerEditorHUDReference)
		return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();

	if (SelectedItem == "Attack")
	{
		SkillData.SkillType = ESkillType::Attack;
	}
	else if (SelectedItem == "Buff")
	{
		SkillData.SkillType = ESkillType::Buff;
	}
	else if (SelectedItem == "Debuff")
	{
		SkillData.SkillType = ESkillType::Debuff;
	}

	OnSkillDetailChanged.Broadcast(SkillData);
}

void USKSkillDetailWidget::OnDamageTextChanged(const FText& Text)
{
	const FString InputValue = Text.ToString().TrimStartAndEnd();
	SK_LOG(LogSkillMaker, Log, TEXT("데미지 입력 변경 수신: RawValue=%s / HUD=%s"),
		*InputValue, SkillMakerEditorHUDReference ? TEXT("Valid") : TEXT("None"));

	if (!SkillMakerEditorHUDReference)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("데미지 입력 반영 실패: 제작 HUD 없음."));
		return;
	}

	if (InputValue.IsEmpty() || !InputValue.IsNumeric())
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("데미지 입력 반영 보류: 숫자가 아닌 값. RawValue=%s"), *InputValue);
		return;
	}

	const float ParsedDamage = FCString::Atof(*InputValue);
	if (!FMath::IsFinite(ParsedDamage) || ParsedDamage < 0.0f)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("데미지 입력 거부: 0 이상의 유한한 값이 아님. RawValue=%s"),
			*InputValue);
		return;
	}

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	const float PreviousDamage = SkillData.DamageValue;
	SkillData.DamageValue = ParsedDamage;
	OnSkillDetailChanged.Broadcast(SkillData);
	SK_LOG(LogSkillMaker, Log,
		TEXT("데미지 HUD 반영 완료: SkillID=%s / PreviousDamage=%.2f / DamageValue=%.2f"),
		*SkillData.SkillID.ToString(), PreviousDamage, SkillData.DamageValue);
}

void USKSkillDetailWidget::OnElementSelectionChanged(FGameplayTag ElementTag, bool bIsSelected)
{
	if (!SkillMakerEditorHUDReference)
		return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	SkillData.ElementTag = bIsSelected ? ElementTag : FGameplayTag();
	for (USKElementCardWidget* ElementCard : ElementCards)
	{
		if (ElementCard)
		{
			ElementCard->SetSelected(bIsSelected && ElementCard->GetElementTag() == ElementTag);
		}
	}

	OnSkillDetailChanged.Broadcast(SkillData);
	if (ProjectileSelectionWidget)
	{
		ProjectileSelectionWidget->SetProjectileCard(SkillData.ProjectileActor, SkillData.ElementTag);
	}
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 속성 업데이트: %s"),
		SkillData.ElementTag.IsValid() ? *SkillData.ElementTag.ToString() : TEXT("None"));
}

void USKSkillDetailWidget::OnProjectileSelected(TSoftClassPtr<ASKProjectileActor> SelectedProjectileClass)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	SkillData.ProjectileActor = SelectedProjectileClass;
	OnSkillDetailChanged.Broadcast(SkillData);

	SK_LOG(LogSkillMaker, Log, TEXT("ProjectileSelected : %s"), *SelectedProjectileClass.ToSoftObjectPath().ToString());
}

void USKSkillDetailWidget::OnNotifySelected(FName NotifyName)
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference) return;

	FSKSkillData SkillData = SkillMakerEditorHUDReference->GetCurrentSkillData();
	SkillData.NotifyName = NotifyName;
	OnSkillDetailChanged.Broadcast(SkillData);

	SK_LOG(LogSkillMaker, Log, TEXT("애님 노티파이 선택됨: %s"), *NotifyName.ToString());
}

void USKSkillDetailWidget::OnPreviewSkillClicked()
{
	SK_LOG(LogSkillMaker, Log, TEXT("Begin"));

	if (!SkillMakerEditorHUDReference)
	{
		SK_LOG(LogSkillMaker, Warning, TEXT("스킬 미리보기 불가: HUD 없음."));
		return;
	}

	SkillMakerEditorHUDReference->PreviewSkillEffect(SkillMakerEditorHUDReference->GetCurrentSkillData());
	SK_LOG(LogSkillMaker, Log, TEXT("스킬 미리보기 실행"));
}
