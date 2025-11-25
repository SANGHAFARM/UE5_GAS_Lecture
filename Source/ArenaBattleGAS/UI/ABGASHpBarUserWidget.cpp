// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ABGASHpBarUserWidget.h"

#include "AbilitySystemComponent.h"
#include "Attribute/ABGASCharacterAttributeSet.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Tag/ABGameplayTag.h"

void UABGASHpBarUserWidget::SetAbilitySystemComponent(AActor* InOwner)
{
	Super::SetAbilitySystemComponent(InOwner);
	
	if (ASC)
	{
		// 특정 어트리뷰트가 변화하면 함수를 호출해주는 델리게이트에 함수 바인딩
		ASC->GetGameplayAttributeValueChangeDelegate(UABGASCharacterAttributeSet::GetHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnHealthChanged);
		ASC->GetGameplayAttributeValueChangeDelegate(UABGASCharacterAttributeSet::GetMaxHealthAttribute()).AddUObject(this, &UABGASHpBarUserWidget::OnMaxHealthChanged);
		
		// 특정 게임플레이 태그가 부착되거나 떨어질 때 브로드캐스팅되는 델리게이트에 함수 바인딩
		ASC->RegisterGameplayTagEvent(ABTAG_CHARACTER_INVINCIBLE, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UABGASHpBarUserWidget::OnInvincibleTagChanged);
		
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		
		// InOwner의 ASC로부터 AttributeSet 불러오기
		const UABGASCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UABGASCharacterAttributeSet>();
		if (CurrentAttributeSet)
		{
			CurrentHealth = CurrentAttributeSet->GetHealth();
			CurrentMaxHealth = CurrentAttributeSet->GetMaxHealth();
			
			if (CurrentMaxHealth > 0.0f)
			{
				UpdateHpBar();
			}
		}
	}
}

void UABGASHpBarUserWidget::OnHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	CurrentMaxHealth = ChangeData.NewValue;
	UpdateHpBar();
}

void UABGASHpBarUserWidget::OnInvincibleTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	// CallbackTag는 어떤 태그가 변경되었는지, NewCount는 해당 태그의 누적 개수
	if (NewCount > 0)
	{
		// NewCount가 0보다 크면 Callback 태그(무적 태그)가 부착되어 있다는 의미
		PbHpBar->SetFillColorAndOpacity(InvincibleColor);
		PbHpBar->SetPercent(1.0f);
	}
	else
	{
		// NewCount가 0 이하라면 Callback 태그(무적 태그)가 제거되었다는 의미
		PbHpBar->SetFillColorAndOpacity(HealthColor);
		UpdateHpBar();
	}
}

void UABGASHpBarUserWidget::UpdateHpBar()
{
	if (PbHpBar)
	{
		PbHpBar->SetPercent(CurrentHealth / CurrentMaxHealth);
	}
	
	if (TxtHpStat)
	{
		TxtHpStat->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, CurrentMaxHealth)));
	}
}
