// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillMakerTrainMainWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillDataFromSaveGame);

class UWidgetSwitcher;
class UButton;
class USKWeaponSelectionWidget;
class USKAnimationSelectionWidget;
class USKSkillSelectionWidget;
class USKSkillDetailWidget;
class UEditableTextBox;
class ASKSkillMakerTrainHUD;
class ASKPlayerCharacter;

UENUM(BlueprintType)
enum class ESKSkillMakerTrainState : uint8
{
	ChooseAction,
	ChooseSkillSet,
	ChooseSkill,
	ChooseAnimation,
	SkillDetail,
	SaveSkill
};

UCLASS()
class SKILLMAKER_API USKSkillMakerTrainMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	virtual bool Initialize() override;

	void StartSkillMaker(ASKPlayerCharacter* InPlayerCharacter);

	void SetHUDReference(ASKSkillMakerTrainHUD* InHUD);
	
	void SetSkillMakerState(ESKSkillMakerTrainState NewState, bool bFromBackNavigation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	FOnSkillDataFromSaveGame OnSkillDataFromSaveGame;

protected:
	// UI 위젯들을 소유하며 변경.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> SkillMakerSwitcher;

	// 스킬셋을 변경할 것인지 스킬셋을 새로 만들것인지, 어떤 스킬을 바꿀것인지 만들것인지 선택.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKSkillSelectionWidget> SkillSelectionWidget;

	// 스킬에 사용될 애니메이션을 선택.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKAnimationSelectionWidget> AnimationSelectionWidget;

	// 스킬의 디테일을 수정.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USKSkillDetailWidget> SkillDetailWidget;

	// 스킬 변경 시 누르는 버튼.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ModifySkillButton;

	// 스킬 생성 시 누르는 버튼.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CreateSkillButton;

	// 스킬의 수정이 완료되었을 때 누르는 버튼.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> FinishEditingSkillButton;
	
	// 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SaveSkillButton;

	// 스킬 이름을 작성하는 텍스트 박스.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> SkillNameInput;

	// 뒤로가기 버튼.
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

	UPROPERTY()
	TObjectPtr<ASKSkillMakerTrainHUD> HUDReference;

	UPROPERTY()
	TObjectPtr<ASKPlayerCharacter> PlayerCharacter;

	FString SelectedWeaponType;

private:
	void GoBackToPreviousState();
	
	UFUNCTION()
	void OnModifySkillSetClicked();

	UFUNCTION()
	void OnCreateSkillSetClicked();
	
	UFUNCTION()
	void OnCreateSkillClicked();

	UFUNCTION()
	void OnSkillSelected(const FName& SkillID);

	UFUNCTION()
	void OnAnimationSelected(UAnimMontage* AnimationMontage);

	UFUNCTION()
	void OnFinishSkillEditing();
	
	UFUNCTION()
	void OnSaveSkillClicked();

	UFUNCTION()
	void OnBackClicked();

	ESKSkillMakerTrainState CurrentState;
	TArray<ESKSkillMakerTrainState> PreviousStates;
};
