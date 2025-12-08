// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ABCharacterPlayer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbilityTypes.h"

#include "ABGASCharacterPlayer.generated.h"

class UABGASWidgetComponent;
class UAbilitySystemComponent;
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class ARENABATTLEGAS_API AABGASCharacterPlayer : public AABCharacterPlayer, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AABGASCharacterPlayer();
	
	FORCEINLINE virtual UAnimMontage* GetSkillActionMontage() const { return SkillActionMontage; }
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Character의 ASC에 PlayerState의 ASC를 대입하고 어빌리티를 추가하는 함수
	virtual void PossessedBy(AController* NewController) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
protected:
	// EnhancedInputComponent를 활용하여 입력 바인딩하는 함수
	void SetupGASInputComponent();
	
	void GASInputPressed(int32 InputId);
	void GASInputReleased(int32 InputId);
	
	UFUNCTION()
	virtual void OnOutOfHealth();
	
	void EquipWeapon(const FGameplayEventData* EventData);
	void UnEquipWeapon(const FGameplayEventData* EventData);
	
protected:
	// 어빌리티 시스템 컴포넌트
	UPROPERTY(EditAnywhere, Category = GAS)
	TObjectPtr<UAbilitySystemComponent> ASC;
	
	// 시작할 때 발동시킬 어빌리티들에 대한 정보를 묶은 TArray
	UPROPERTY(EditAnywhere, Category = GAS)
	TArray<TSubclassOf<UGameplayAbility>> StartAbilities;
	
	// 입력에 대한 어빌리티와 Id 목록
	UPROPERTY(EditAnywhere, Category = GAS)
	TMap<int32, TSubclassOf<UGameplayAbility>> StartInputAbilities;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UABGASWidgetComponent> HpBar;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	TObjectPtr<USkeletalMesh> WeaponMesh;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponRange;
	
	UPROPERTY(EditAnywhere, Category = Weapon)
	float WeaponAttackRate;
	
	UPROPERTY(EditAnywhere, Category = GAS)
	TSubclassOf<UGameplayAbility> SkillAbilityClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<UAnimMontage> SkillActionMontage;
};
