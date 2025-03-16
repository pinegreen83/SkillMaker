// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKEffectCardWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectCardSelected, TSubclassOf<AActor>, SelectedEffect);

UCLASS()
class SKILLMAKER_API USKEffectCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	/** 이펙트 정보를 설정 */
	void SetEffectInfo(TSubclassOf<AActor> InEffectClass);

	/** 이펙트 선택 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Effect Selection")
	FOnEffectCardSelected OnEffectSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EffectSelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectNameText;

private:
	TSubclassOf<AActor> EffectClass;

	/** 버튼 클릭 시 이펙트 선택 이벤트 발생 */
	UFUNCTION()
	void OnEffectButtonClicked();
};
