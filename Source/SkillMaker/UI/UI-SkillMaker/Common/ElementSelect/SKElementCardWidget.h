#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "SKElementCardWidget.generated.h"

class UButton;
class UCheckBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnElementCardSelectionChanged, FGameplayTag, ElementTag, bool, bIsSelected);

UCLASS()
class SKILLMAKER_API USKElementCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	void SetElementInfo(FGameplayTag InElementTag, const FText& InDisplayName,
		const FText& InDescription, const FLinearColor& InDisplayColor, bool bIsSelected);
	void SetSelected(bool bIsSelected);
	FGameplayTag GetElementTag() const;

	UPROPERTY(BlueprintAssignable, Category = "Element Selection")
	FOnElementCardSelectionChanged OnElementSelectionChanged;

private:
	void BuildDefaultWidgetTree();

	UFUNCTION()
	void OnElementButtonClicked();

	UPROPERTY(Transient)
	TObjectPtr<UButton> ElementButton;

	UPROPERTY(Transient)
	TObjectPtr<UCheckBox> ElementCheckBox;

	UPROPERTY(Transient)
	TObjectPtr<UTextBlock> ElementNameText;

	FGameplayTag ElementTag;
	FLinearColor ElementColor = FLinearColor::White;
	bool bUpdatingSelection = false;
};
