// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SKWeaponSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSelected, FGameplayTag, WeaponTag);

class UUniformGridPanel;
class USKWeaponCardWidget;
struct FSKWeaponRow;

UCLASS()
class SKILLMAKER_API USKWeaponSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon Selection")
	void LoadWeaponList(FGameplayTag CurrentWeaponTag);
	
	UPROPERTY(BlueprintAssignable, Category = "Weapon Selection")
	FOnWeaponSelected OnWeaponSelected;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> WeaponGridPanel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKWeaponCardWidget> WBP_SKWeaponCard;

	void CreateWeaponCard(const FSKWeaponRow& WeaponRow, const int32 WeaponIndex, FGameplayTag CurrentWeaponTag);

	UFUNCTION()
	void WeaponSelected(FGameplayTag WeaponTag);
};
