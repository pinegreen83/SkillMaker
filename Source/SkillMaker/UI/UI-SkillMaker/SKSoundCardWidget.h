// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKSoundCardWidget.generated.h"

class UButton;
class UTextBlock;
class USoundBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundCardSelected, USoundBase*, SelectedSound);

UCLASS()
class SKILLMAKER_API USKSoundCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	/** 사운드 정보를 설정 */
	void SetSoundInfo(USoundBase* InSound);

	/** 사운드 선택 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Sound Selection")
	FOnSoundCardSelected OnSoundSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SoundSelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SoundNameText;

private:
	UPROPERTY()
	TObjectPtr<USoundBase> Sound;

	/** 버튼 클릭 시 사운드 선택 이벤트 발생 */
	UFUNCTION()
	void OnSoundButtonClicked();
};
