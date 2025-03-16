// Fill out your copyright notice in the Description page of Project Settings.


#include "SKAnimNotifySelectionWidget.h"
#include "SKAnimNotifyCardWidget.h"
#include "Components/ScrollBox.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/TextBlock.h"

bool USKAnimNotifySelectionWidget::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	return true;
}

void USKAnimNotifySelectionWidget::PopulateNotifyList(UAnimMontage* Montage)
{
	if (!NotifyListBox || !Montage || !WBP_AnimNotifyCard)
	{
		UE_LOG(LogTemp, Error, TEXT("몽타주 없음."));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("몽타주 이름 : %s"), *Montage->GetName());

	NotifyListBox->ClearChildren();
	AvailableNotifies.Empty();

	int32 NotifyIndex = 1;
	for (const FAnimNotifyEvent& NotifyEvent : Montage->Notifies)
	{
		FName NotifyName = NotifyEvent.NotifyName;
		float NotifyTime = NotifyEvent.GetTime();

		FString IndexedNotifyName = FString::Printf(TEXT("%d번 - %s (%.2f초)"), NotifyIndex, *NotifyName.ToString(), NotifyTime);
		NotifyIndex++;

		UE_LOG(LogTemp, Log, TEXT("애님 노티파이: %s"), *IndexedNotifyName);

		USKAnimNotifyCardWidget* NotifyButton = CreateWidget<USKAnimNotifyCardWidget>(this, WBP_AnimNotifyCard);
		if (!NotifyButton) continue;

		NotifyButton->SetNotifyInfo(NotifyName, NotifyTime); // 노티파이 이름과 시간 저장
		NotifyButton->OnNotifySelected.AddDynamic(this, &USKAnimNotifySelectionWidget::OnNotifyButtonSelected);

		NotifyListBox->AddChild(NotifyButton);
	}
}

void USKAnimNotifySelectionWidget::ExtractNotifiesFromMontage(UAnimMontage* Montage)
{
	if (!Montage) return;

	for (const FAnimNotifyEvent& NotifyEvent : Montage->Notifies)
	{
		if (NotifyEvent.NotifyName != NAME_None)
		{
			AvailableNotifies.AddUnique(NotifyEvent.NotifyName);
		}
	}
}

void USKAnimNotifySelectionWidget::OnNotifyButtonSelected(FName NotifyName, float NotifyTime)
{
	SelectedNotify = NotifyName;
	SelectedNotifyTime = NotifyTime;  

	SelectedNotifyText->SetText(FText::FromString(FString::Printf(TEXT("%s (%.2f초)"), *NotifyName.ToString(), NotifyTime)));
	OnAnimNotifySelected.Broadcast(SelectedNotify, SelectedNotifyTime);  // NotifyTime 추가
	UE_LOG(LogTemp, Log, TEXT("애님 노티파이 선택됨: %s (%.2f초)"), *NotifyName.ToString(), NotifyTime);
}
