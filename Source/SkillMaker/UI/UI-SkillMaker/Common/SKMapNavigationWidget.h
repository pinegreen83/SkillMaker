#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKMapNavigationWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class SKILLMAKER_API USKMapNavigationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;
	void Configure(const FText& ButtonText, FName InDestinationMap);

private:
	UFUNCTION()
	void OnNavigationButtonClicked();

	UPROPERTY()
	TObjectPtr<UButton> NavigationButton;

	UPROPERTY()
	TObjectPtr<UTextBlock> NavigationButtonText;

	FName DestinationMap;
};
