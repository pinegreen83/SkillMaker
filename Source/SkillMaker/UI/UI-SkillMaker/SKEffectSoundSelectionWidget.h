// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKEffectSoundSelectionWidget.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class UImage;
class USoundBase;
class AActor;
class UWidgetSwitcher;
class USKEffectCardWidget;
class USKSoundCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectSoundSelected, const FEffectSoundData&, SelectedEffectSound);

UCLASS()
class SKILLMAKER_API USKEffectSoundSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	void PopulateEffectList();
	void PopulateSoundList();
	
	UPROPERTY(BlueprintAssignable, Category = "EffectSound Selection")
	FOnEffectSoundSelected OnEffectSoundSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> TabSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EffectTabButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SoundTabButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FinalTabButton;
	
	/** 이펙트 리스트 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> EffectListBox;

	/** 사운드 리스트 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> SoundListBox;

	/** 선택된 이펙트 프리뷰 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedEffectPreview;

	/** 선택된 이펙트 이름 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedEffectText;

	/** 선택된 사운드 이름 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedSoundText;

	/** 적용 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	/** 개별 항목 UI 카드 위젯 클래스 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKEffectCardWidget> WBP_EffectCard;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKSoundCardWidget> WBP_SoundCard;

private:
	/** 현재 선택된 이펙트 & 사운드 */
	FEffectSoundData SelectedEffectSoundData;
	
	UFUNCTION()
	void OnEffectTabClicked();

	UFUNCTION()
	void OnSoundTabClicked();
	
	UFUNCTION()
	void OnFinalTabClicked();
	
	/** 이펙트 선택 시 호출 */
	UFUNCTION()
	void OnEffectSelected(TSubclassOf<AActor> SelectedEffect);

	/** 사운드 선택 시 호출 */
	UFUNCTION()
	void OnSoundSelected(USoundBase* SelectedSound);

	/** 선택한 데이터를 SkillDetail로 전달 */
	UFUNCTION()
	void OnConfirmSelection();
};
