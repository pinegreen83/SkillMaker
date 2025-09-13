// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SKAnimNotifyCardWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyButtonClicked, FName, NotifyName);

UCLASS()
class SKILLMAKER_API USKAnimNotifyCardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	/** 노티파이 정보를 UI에 설정 */
	void SetNotifyInfo(FName InNotifyType, FName InNotifyName, float InNotifyTime);

	/** 노티파이 선택 시 브로드캐스트할 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "AnimNotify Selection")
	FOnNotifyButtonClicked OnNotifySelected;

protected:
	/** 노티파이 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NotifyButton;

	/** 노티파이 이름 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NotifyText;

	/** 저장된 노티파이 정보 */
	FName NotifyName;
	float NotifyTime;

	/** 버튼 클릭 시 호출 */
	UFUNCTION()
	void HandleButtonClicked();
};
