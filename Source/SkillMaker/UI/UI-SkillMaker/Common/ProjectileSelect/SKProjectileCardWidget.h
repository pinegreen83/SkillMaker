// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKSkillData.h"
#include "SKProjectileCardWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileCardSelected, TSubclassOf<ASKProjectileActor>, SelectedEffect);

UCLASS()
class SKILLMAKER_API USKProjectileCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	/** 발사체 정보를 설정 */
	void SetProjectileInfo(const TSubclassOf<ASKProjectileActor> InProjectileClass);

	/** 발사체 선택 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Projectile Selection")
	FOnProjectileCardSelected OnProjectileCardSelected;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ProjectileSelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProjectileNameText;

private:
	TSubclassOf<ASKProjectileActor> ProjectileClass;

	/** 버튼 클릭 시 발사체 선택 이벤트 발생 */
	UFUNCTION()
	void OnProjectileButtonClicked();
};
