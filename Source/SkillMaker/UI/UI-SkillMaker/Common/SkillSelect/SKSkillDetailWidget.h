// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKSkillDetailWidget.generated.h"

class ASKSkillMakerTrainHUD;
class ASKSkillMakerEditorHUD;
class UComboBoxString;
class UEditableTextBox;
class USlider;
class UButton;
class UPanelWidget;
class UScrollBox;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;
class USKElementCardWidget;
class USKProjectileSelectionWidget;
class USKAnimNotifySelectionWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillDetailChanged, const FSKSkillData&, SkillData);

UENUM(BlueprintType)
enum class ESKHUDClass : uint8
{
	SkillMakerEditor,
	SkillMakerTrain,
};

UCLASS()
class SKILLMAKER_API USKSkillDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** HUD 레퍼런스 설정 */
	void SetSkillMakerEditorHUD(ASKSkillMakerEditorHUD* InHUD);
	void SetSkillMakerTrainHUD(ASKSkillMakerTrainHUD* InHUD);

	/** UI를 현재 스킬 데이터로 초기화 */
	void InitializeFromSkillData();

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillDetailChanged OnSkillDetailChanged;

protected:
	/** HUD 참조 */
	UPROPERTY()
	TObjectPtr<ASKSkillMakerEditorHUD> SkillMakerEditorHUDReference;

	UPROPERTY()
	TObjectPtr<ASKSkillMakerTrainHUD> SkillMakerTrainHUDReference;

	/** 탭 UI 전환을 위한 WidgetSwitcher */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> TabSwitcher;

	/** WidgetSwitcher의 기본 탭 직접 자식 */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UPanelWidget> GeneralTabPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> StatusEffectTabPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> ProjectileTabPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UWidget> AnimNotifyPanel;

	/** 탭 버튼들 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GeneralTabButton;

	/** 기존 WBP 이름을 유지하지만 프로토타입에서는 속성 탭으로 사용 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StatusEffectTabButton;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StatusEffectTabButtonText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StatusEffectTabText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EffectSoundTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AnimNotifyTabButton;
	
	/** 스킬 타입 선택 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> SkillTypeComboBox;

	/* Legacy: 프로토타입 세부사항 개편 전 데미지 입력 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> DamageTextBox;
	*/

	/** 기존 WBP 이름을 유지하지만 프로토타입에서는 속성 목록으로 사용 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> StatusEffectListBox;

	/** 발사체 선택 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKProjectileSelectionWidget> ProjectileSelectionWidget;

	/** 애님 노티파이 선택 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKAnimNotifySelectionWidget> AnimNotifySelectionWidget;
	
	// /** 투사체 위치 조정 버튼 */
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UButton> AdjustProjectilePositionButton;

	/** 논타겟 프로토타입에서는 기존 사거리 UI를 숨긴다. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USlider> MinRangeSlider;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<USlider> MaxRangeSlider;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> MinRangeText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> MaxRangeText;

	/** 미리보기 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PreviewSkillButton;

private:
	
	/** UI 요소 -> HUD 데이터로 초기화 */
	void PopularSkillDetails();
	void ConfigureGeneralTabScrolling();
	void SelectTab(UWidget* TabContent, UButton* SelectedTabButton, const TCHAR* TabDisplayName);
	void UpdateTabButtonSelection(UButton* SelectedTabButton);
	void LogTabState(const TCHAR* Phase, const FString& RequestedTabName, const UWidget* RequestedContent) const;

	/** 단일 속성 선택 목록 초기화 */
	void PopulateElementList();

	/** 애님 노티파이 리스트 초기화 */
	void PopulateAnimNotifyList();
	
	/** 탭 버튼 클릭 이벤트 */
	UFUNCTION()
	void OnGeneralTabClicked();

	UFUNCTION()
	void OnElementTabClicked();

	UFUNCTION()
	void OnEffectSoundTabClicked();

	UFUNCTION()
	void OnAnimNotifyTabClicked();

	/** 콤보박스 선택 시(스킬 타입 변경) */
	UFUNCTION()
	void OnSkillTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	/* Legacy: 프로토타입 세부사항 개편 전 데미지 입력 이벤트
	UFUNCTION()
	void OnDamageChanged(const FText& Text, ETextCommit::Type CommitMethod);
	*/

	/** 속성 선택 / 해제 시 호출 */
	UFUNCTION()
	void OnElementSelectionChanged(FGameplayTag ElementTag, bool bIsSelected);

	/* Legacy: 프로토타입 세부사항 개편 전 범위 설정 이벤트
	UFUNCTION()
	void OnMinRangeChanged(float Value);

	UFUNCTION()
	void OnMaxRangeChanged(float Value);
	*/

	/** 발사체 선택 완료 이벤트 */
	UFUNCTION()
	void OnProjectileSelected(TSoftClassPtr<ASKProjectileActor> SelectedProjectileClass);

	/** 애님 노티파이 선택 이벤트 */
	UFUNCTION()
	void OnNotifySelected(FName NotifyName);
	
	/** 미리보기 실행 */
	UFUNCTION()
	void OnPreviewSkillClicked();

	UPROPERTY(Transient)
	TArray<TObjectPtr<USKElementCardWidget>> ElementCards;

	/** GeneralTabPanel 내부에서 GeneralTabContent를 감싸는 블루프린트 ScrollBox */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UScrollBox> GeneralTabScrollBox;
};
