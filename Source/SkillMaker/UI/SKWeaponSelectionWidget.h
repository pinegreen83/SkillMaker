// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKWeaponSelectionWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSelected, const FString&, WeaponName);

class UUniformGridPanel;
class USKWeaponCardWidget;
struct FSKWeaponData;

UCLASS()
class SKILLMAKER_API USKWeaponSelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon Selection")
	void LoadWeaponList();
	
	UPROPERTY(BlueprintAssignable, Category = "Weapon Selection")
	FOnWeaponSelected OnWeaponSelected;
	
protected:
	UPROPERTY()
	TArray<FSKWeaponData> WeaponList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> WeaponGridPanel;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKWeaponCardWidget> WBP_SKWeaponCard;

	void CreateWeaponCard(const FString& WeaponName, UTexture2D* Thumbnail, const FString& WeaponType, const int32 WeaponIndex);

	UFUNCTION()
	void SelectWeapon(const FString& WeaponName);
};
