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
class UScrollBox;
class UWidgetSwitcher;
class USKStatusEffectCardWidget;
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

	/** 탭 버튼들 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GeneralTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StatusEffectTabButton;

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

	/** 상태 이상 효과 리스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> StatusEffectListBox;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKStatusEffectCardWidget> WBP_SKStatusEffectCard;

	/** 발사체 선택 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKProjectileSelectionWidget> ProjectileSelectionWidget;

	/** 애님 노티파이 선택 위젯 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKAnimNotifySelectionWidget> AnimNotifySelectionWidget;
	
	// /** 투사체 위치 조정 버튼 */
	// UPROPERTY(meta = (BindWidget))
	// TObjectPtr<UButton> AdjustProjectilePositionButton;

	/* Legacy: 프로토타입 세부사항 개편 전 범위 설정 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MinRangeSlider;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MaxRangeSlider;
	*/

	/** 미리보기 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PreviewSkillButton;

private:
	
	/** UI 요소 -> HUD 데이터로 초기화 */
	void PopularSkillDetails();

	/** 상태이상 선택 창 초기화 */
	void PopulateStatusEffectList();

	/** 애님 노티파이 리스트 초기화 */
	void PopulateAnimNotifyList();
	
	/** 탭 버튼 클릭 이벤트 */
	UFUNCTION()
	void OnGeneralTabClicked();

	UFUNCTION()
	void OnStatusEffectTabClicked();

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

	/** 상태 이상 선택 / 해제 시 호출 */
	UFUNCTION()
	void OnStatusEffectToggled(const FStatusEffectData& EffectData, bool bIsChecked);

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
	
};
