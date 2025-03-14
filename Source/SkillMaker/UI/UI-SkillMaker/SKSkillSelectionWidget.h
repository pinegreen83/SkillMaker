// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillSelected, const FName&, SkillID);

class UUniformGridPanel;
class USKSkillCardWidget;
struct FSKSkillData;

UCLASS()
class SKILLMAKER_API USKSkillSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Skill Selection")
	void LoadSkillList();

	UPROPERTY(BlueprintCallable, Category = "Skill Selection")
	FOnSkillSelected OnSkillSelected;

protected:
	UPROPERTY()
	TArray<FSKSkillData> SkillList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> SkillGridPanel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKSkillCardWidget> WBP_SKSkillCard;

	void CreateSkillCard(const FName& SkillID, const FString& SkillName, UTexture2D* Thumbnail, const int32 SkillIndex);

	UFUNCTION()
	void SelectSkill(const FName& SkillID);
};
