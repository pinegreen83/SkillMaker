// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Skill/SKProjectileActor.h"
#include "SKProjectileSelectionWidget.generated.h"

class UButton;
class UScrollBox;
class UTextBlock;
class UImage;
class USoundBase;
class AActor;
class UWidgetSwitcher;
class USKProjectileCardWidget;
class USKSoundCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileSelected, TSubclassOf<ASKProjectileActor>, SelectedProjectile);

UCLASS()
class SKILLMAKER_API USKProjectileSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;

	void SetProjectileCard();
	
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

	/** 적용 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	/** 개별 항목 UI 카드 위젯 클래스 */
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKProjectileCardWidget> WBP_ProjectileCard;

private:
	/** 현재 선택된 이펙트 & 사운드 */
	TSubclassOf<ASKProjectileActor> SelectedProjectileData;

	UFUNCTION()
	void SelectedProjectile(TSubclassOf<ASKProjectileActor> SelectedProjectile);
	
	/** 선택한 데이터를 SkillDetail로 전달 */
	UFUNCTION()
	void OnConfirmSelection();
};
