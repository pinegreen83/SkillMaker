// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKSkillDetailWidget.generated.h"

class ASKSkillMakerHUD;
class UComboBoxString;
class UEditableTextBox;
class USlider;
class UButton;
class UVerticalBox;
class UWidgetSwitcher;
class USKStatusEffectCardWidget;

UCLASS()
class SKILLMAKER_API USKSkillDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** HUD 레퍼런스 설정 */
	void SetHUDReference(ASKSkillMakerHUD* InHUD);

	/** UI에서 수정된 데이터를 HUD에 저장 */
	void SaveSkillData();
	
	/** UI를 현재 스킬 데이터로 초기화 */
	void InitializeFromSkillData();

protected:
	/** HUD 참조 */
	UPROPERTY()
	TObjectPtr<ASKSkillMakerHUD> HUDReference;

	/** 현재 수정 중인 스킬 데이터 */	
	TOptional<FSKSkillData> EditingSkillData;

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
	TObjectPtr<UButton> ProjectileTabButton;
	
	/** 스킬 타입 선택 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> SkillTypeComboBox;

	/** 데미지 값 입력 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> DamageTextBox;

	/** 상태 이상 효과 리스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatusEffectListBox;

	/** 투사체 위치 조정 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AdjustProjectilePositionButton;

	/** 최소 사거리 슬라이더 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MinRangeSlider;

	/** 최대 사거리 슬라이더 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> MaxRangeSlider;

	/** 저장 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveSkillButton;

private:
	/** UI 요소 -> HUD 데이터로 초기화 */
	void PopularSkillDetails();

	/** 상태이상 선택 창 초기화 */
	void PopulateStatusEffectList();

	/** UI 값 변경 -> HUD 데이터에 저장 */
	void SaveSkillDetails();

	/** 탭 버튼 클릭 이벤트 */
	UFUNCTION()
	void OnGeneralTabClicked();

	UFUNCTION()
	void OnStatusEffectTabClicked();

	UFUNCTION()
	void OnEffectSoundTabClicked();

	UFUNCTION()
	void OnProjectileTabClicked();

	/** 콤보박스 선택 시(스킬 타입 변경) */
	UFUNCTION()
	void OnSkillTypeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);

	/** 데미지 값 변경 시 */
	UFUNCTION()
	void OnDamageChanged(const FText& Text, ETextCommit::Type CommitMethod);

	/** 상태 이상 선택 / 해제 시 호출 */
	UFUNCTION()
	void OnStatusEffectToggled(EStatusEffect EffectType, bool bIsChecked, float Duration, float DamageOverTime);

	/** 최소 사거리 변경 */
	UFUNCTION()
	void OnMinRangeChanged(float Value);

	/** 최대 사거리 변경 */
	UFUNCTION()
	void OnMaxRangeChanged(float Value);
	
	/** 저장 버튼 클릭 */
	UFUNCTION()
	void OnSaveSkillClicked();
};
