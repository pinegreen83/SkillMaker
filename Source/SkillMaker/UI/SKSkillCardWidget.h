// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillCardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCardSelected, const FString&, SkillName);

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class SKILLMAKER_API USKSkillCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category = "Skill Selection")
	void SetSkillInfo(const FString& SkillName, UTexture2D* Thumbnail);

	UPROPERTY(BlueprintAssignable, Category = "Skill Selection")
	FOnSkillCardSelected OnSkillCardSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkillButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SkillThumbnail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SkillNameText;

	FString StoredSkillName;

	UFUNCTION()
	void HandleSkillSelected();
};
