// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/ABGASPlayerState.h"

AABGASCharacterPlayer::AABGASCharacterPlayer()
{
	// 플레이어가 빙의하는 캐릭터의 경우
	// ASC를 또 생성하게 되면 데이터를 관리하는 주체가 2개(State, Character)가 되어버리기 때문에
	// 하나의 캐릭터로 관리하기 위해서 ASC를 우선 null로 설정하고, 나중에 PlayerState의 ASC를 대입
	ASC = nullptr;
	
	// 참조할 몽타주 애셋 경로 지정
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattleGAS/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}
}

UAbilitySystemComponent* AABGASCharacterPlayer::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// PlayerState의 ASC를 불러와서 Character의 ASC에 대입
	AABGASPlayerState* GASPS = GetPlayerState<AABGASPlayerState>();
	if (GASPS)
	{
		ASC = GASPS->GetAbilitySystemComponent();
		// OwnerActor와 AvatarActor가 결정됐기 때문에 초기화 실행
		ASC->InitAbilityActorInfo(GASPS, this);
		
		// StartAbilities를 순회하면서 Spec 생성 후 어빌리티 추가
		for (const auto& StartAbility : StartAbilities)
		{
			FGameplayAbilitySpec StartSpec(StartAbility);
			
			// ASC에 어빌리티 추가
			ASC->GiveAbility(StartSpec);
		}
		
		// StartInputAbilities를 순회하면서 Spec 생성 후 어빌리티 추가
		for (const auto& StartInputAbility : StartInputAbilities)
		{
			// StartInputAbility.Value로 어빌리티 지정
			FGameplayAbilitySpec StartSpec(StartInputAbility.Value);
			// StartInputAbility.Key로 InputID 지정
			StartSpec.InputID = StartInputAbility.Key;
			
			// ASC에 어빌리티 추가
			ASC->GiveAbility(StartSpec);
		}
		
		// PossessedBy 함수는 서버에서만 호출
		SetupGASInputComponent();
		
		APlayerController* PlayerController = CastChecked<APlayerController>(NewController);
		PlayerController->ConsoleCommand(TEXT("showdebug abilitysystem"));
	}
}

void AABGASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	SetupGASInputComponent();
}

void AABGASCharacterPlayer::SetupGASInputComponent()
{
	if (IsValid(ASC) && IsValid(InputComponent))
	{
		UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
		
		// 매개변수가 있는 함수와 바인딩 가능
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 0);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AABGASCharacterPlayer::GASInputReleased, 0);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 1);
	}
}

void AABGASCharacterPlayer::GASInputPressed(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = true;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void AABGASCharacterPlayer::GASInputReleased(int32 InputId)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputId);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}
