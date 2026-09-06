#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKSkillSlotAssignmentWidget.generated.h"

class UButton;
class UBorder;
class UTextBlock;
struct FSKSkillData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillSlotSelected, int32, SlotIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillSlotAssignmentCancelled);

UCLASS()
class SKILLMAKER_API USKSkillSlotAssignmentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetSelectedSkill(const FSKSkillData& SkillData);
	void SetAssignedSkill(int32 SlotIndex, const FString& SkillName);
	void ShowAssignmentOverview();

	UPROPERTY(BlueprintAssignable, Category = "Skill Slot")
	FOnSkillSlotSelected OnSkillSlotSelected;

	UPROPERTY(BlueprintAssignable, Category = "Skill Slot")
	FOnSkillSlotAssignmentCancelled OnAssignmentCancelled;

private:
	void BuildNativeWidget();
	UButton* CreateSlotButton(const FName& ButtonName, const FName& TextName, const FText& InitialText,
		TObjectPtr<UTextBlock>& OutTextBlock);
	void BroadcastSlotSelection(int32 SlotIndex);
	UTextBlock* GetSlotText(int32 SlotIndex) const;
	void SetSlotButtonsEnabled(bool bEnabled);

	UPROPERTY()
	TObjectPtr<UBorder> RootBorder;

	UPROPERTY()
	TObjectPtr<UTextBlock> SelectedSkillText;

	UPROPERTY()
	TObjectPtr<UButton> SkillSlotQButton;

	UPROPERTY()
	TObjectPtr<UButton> SkillSlotEButton;

	UPROPERTY()
	TObjectPtr<UButton> SkillSlotRButton;

	UPROPERTY()
	TObjectPtr<UButton> SkillSlotFButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> SkillSlotQText;

	UPROPERTY()
	TObjectPtr<UTextBlock> SkillSlotEText;

	UPROPERTY()
	TObjectPtr<UTextBlock> SkillSlotRText;

	UPROPERTY()
	TObjectPtr<UTextBlock> SkillSlotFText;

	UPROPERTY()
	TObjectPtr<UButton> CancelButton;

	UFUNCTION()
	void OnQSlotClicked();

	UFUNCTION()
	void OnESlotClicked();

	UFUNCTION()
	void OnRSlotClicked();

	UFUNCTION()
	void OnFSlotClicked();

	UFUNCTION()
	void OnCancelClicked();

	FName SelectedSkillID;
};
