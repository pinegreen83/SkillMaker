// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKStatusEffectCardWidget.generated.h"

class UCheckBox;
class UEditableTextBox;
class USlider;
class UTextBlock;

UCLASS()
class SKILLMAKER_API USKStatusEffectCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void InitializeEffectEntry(EStatusEffect EffectType);

	/** 기존 상태이상 데이터를 UI에 반영 */
	void SetStatusEffectData(const FStatusEffectData& EffectData);
	
	/** 현재 입력된 상태 이상 데이터를 반환 */
	FStatusEffectData GetCurrentStatusEffectData() const;

protected:
	/** 상태 이상 이름 표시 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectNameText;
	
	/** 상태 이상 체크박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> EffectCheckBox;

	/** 지속 시간 입력 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> DurationTextBox;

	/** 초당 피해량 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USlider> DOTSlider;

	/** 최대 스택 개수 */
	TObjectPtr<UEditableTextBox> StackCountTextBox;

	/** 현재 상태이상 타입 */
	EStatusEffect CurrentEffectType;
	float CurrentDuration;
	float CurrentDOT;
	int32 CurrentStackCount;

	/** 상태 변경 이벤트 */
	UFUNCTION()
	void OnEffectToggled(bool bIsChecked);

	UFUNCTION()
	void OnDurationChanged(const FText& Text, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void OnDOTValueChanged(float Value);

	UFUNCTION()
	void OnStackCountChanged(const FText& Text, ETextCommit::Type CommitMethod);
};