// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "Skill/SKProjectileActor.h"
#include "SKProjectileSelectionWidget.generated.h"

class UScrollBox;
class UTextBlock;
class UImage;
class USoundBase;
class AActor;
class UWidgetSwitcher;
class UButton;
class USKProjectileCardWidget;
class USKSoundCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileSelected, TSoftClassPtr<ASKProjectileActor>, SelectedProjectile);

UCLASS()
class SKILLMAKER_API USKProjectileSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	void SetProjectileCard(const TSoftClassPtr<ASKProjectileActor>& CurrentProjectile,
		FGameplayTag CurrentElementTag = FGameplayTag());
	
	UPROPERTY(BlueprintAssignable, Category = "Projectile Selection")
	FOnProjectileSelected OnProjectileSelected;

protected:
	/** 이펙트 리스트 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ProjectileListBox;

	/** 선택된 이펙트 프리뷰 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedEffectPreview;

	/** 선택된 이펙트 이름 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedEffectText;

	/** 카드 클릭 즉시 선택하므로 기존 확인 버튼은 표시하지 않는다. */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UButton> ConfirmButton;

	/** 개별 항목 UI 카드 위젯 클래스 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKProjectileCardWidget> WBP_ProjectileCard;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USKProjectileCardWidget>> ProjectileCards;

private:
	/** 현재 선택된 이펙트 & 사운드 */
	UPROPERTY()
	TSoftClassPtr<ASKProjectileActor> SelectedProjectileData;

	UFUNCTION()
	void SelectedProjectile(TSoftClassPtr<ASKProjectileActor> SelectedProjectile);
};
