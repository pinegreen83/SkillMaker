// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKWeaponCardWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponCardSelected, const FString&, WeaponType);

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class SKILLMAKER_API USKWeaponCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon Selection")
	void SetWeaponInfo(const FString& WeaponName, UTexture2D* Thumbnail, const FString& InWeaponType);

	UPROPERTY(BlueprintAssignable, Category = "Weapon Selection")
	FOnWeaponCardSelected OnWeaponCardSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> WeaponButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> WeaponThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponNameText;
	
	FString WeaponType;

	UFUNCTION()
	void HandleWeaponSelected();
};
