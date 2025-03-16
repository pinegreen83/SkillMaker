// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Animation/AnimMontage.h"
#include "SKAnimNotifySelectionWidget.generated.h"

class UScrollBox;
class UTextBlock;
class USKAnimNotifyCardWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimNotifySelected, FName, NotifyName, float, NotifyTime);

UCLASS()
class SKILLMAKER_API USKAnimNotifySelectionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	/** 애님 몽타주에서 애님 노티파이 목록을 가져와 UI를 채운다 */
	void PopulateNotifyList(UAnimMontage* Montage);

	/** 애님 노티파이 선택 시 브로드캐스트할 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "AnimNotify Selection")
	FOnAnimNotifySelected OnAnimNotifySelected;

protected:
	/** 애님 노티파이 버튼 목록 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> NotifyListBox;

	/** 선택된 노티파이 이름 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SelectedNotifyText;
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<USKAnimNotifyCardWidget> WBP_AnimNotifyCard;

private:
	/** 사용 가능한 애님 노티파이 목록 */
	TArray<FName> AvailableNotifies;

	/** 선택된 애님 노티파이 */
	FName SelectedNotify;
	float SelectedNotifyTime;

	/** UI 요소에 표시할 애님 노티파이 목록을 초기화 */
	void ExtractNotifiesFromMontage(UAnimMontage* Montage);

	/** 리스트에서 특정 애님 노티파이를 선택했을 때 호출 */
	UFUNCTION()
	void OnNotifyButtonSelected(FName NotifyName, float NotifyTime);
};
