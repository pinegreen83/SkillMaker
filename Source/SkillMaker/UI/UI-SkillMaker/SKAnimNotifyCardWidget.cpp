// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimNotifyCardWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

bool USKAnimNotifyCardWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (NotifyButton)
	{
		NotifyButton->OnClicked.AddDynamic(this, &USKAnimNotifyCardWidget::HandleButtonClicked);
	}

	return true;
}

void USKAnimNotifyCardWidget::SetNotifyInfo(FName InNotifyName, float InNotifyTime)
{
	NotifyName = InNotifyName;
	NotifyTime = InNotifyTime;

	if (NotifyText)
	{
		NotifyText->SetText(FText::FromString(FString::Printf(TEXT("%s (%.2f초)"), *NotifyName.ToString(), NotifyTime)));
	}
}

void USKAnimNotifyCardWidget::HandleButtonClicked()
{
	OnNotifySelected.Broadcast(NotifyName, NotifyTime);
	UE_LOG(LogTemp, Log, TEXT("노티파이 버튼 클릭됨: %s, %f"), *NotifyName.ToString(), NotifyTime);
}
