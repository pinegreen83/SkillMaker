#include "SKMapNavigationWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Logging/SKLogSkillMakerMacro.h"

bool USKMapNavigationWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	NavigationButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("NavigationButton"));
	NavigationButtonText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NavigationButtonText"));
	NavigationButtonText->SetJustification(ETextJustify::Center);
	NavigationButton->AddChild(NavigationButtonText);
	NavigationButton->OnClicked.AddDynamic(this, &USKMapNavigationWidget::OnNavigationButtonClicked);
	WidgetTree->RootWidget = NavigationButton;

	return true;
}

void USKMapNavigationWidget::Configure(const FText& ButtonText, FName InDestinationMap)
{
	DestinationMap = InDestinationMap;
	if (NavigationButtonText)
	{
		NavigationButtonText->SetText(ButtonText);
	}
}

void USKMapNavigationWidget::OnNavigationButtonClicked()
{
	if (DestinationMap.IsNone())
	{
		SK_LOG(LogSkillMaker, Error, TEXT("이동할 맵이 설정되지 않음."));
		return;
	}

	SK_LOG(LogSkillMaker, Log, TEXT("맵 이동: %s"), *DestinationMap.ToString());
	UGameplayStatics::OpenLevel(this, DestinationMap);
}
