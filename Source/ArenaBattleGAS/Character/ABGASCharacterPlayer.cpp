// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ABGASCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "Player/ABGASPlayerState.h"
#include "UI/ABGASWidgetComponent.h"
#include "Attribute/ABGASCharacterAttributeSet.h"
#include "Tag/ABGameplayTag.h"

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
	
	HpBar = CreateDefaultSubobject<UABGASWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/ArenaBattleGAS/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(200.0f, 20.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshRef(TEXT("/Game/InfinityBladeWeapons/Weapons/Blunt/Blunt_Hellhammer/SK_Blunt_HellHammer.SK_Blunt_HellHammer"));
	if (WeaponMeshRef.Object)
	{
		WeaponMesh = WeaponMeshRef.Object;
	}
	
	WeaponRange = 75.0f;
	WeaponAttackRate = 100.0f;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SkillActionMontageRef(TEXT("/Game/ArenaBattleGAS/Animation/AM_SkillAttack.AM_SkillAttack"));
	if (SkillActionMontageRef.Object)
	{
		SkillActionMontage = SkillActionMontageRef.Object;
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
		
		// ASC 내부의 TMap에서 ABTAG_EVENT_CHARACTER_WEAPONEQUIP 키를 가진 delegate를 찾고, 없으면 새로 생성 후 해당 delegate에 EquipWeapon을 바인딩
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONEQUIP).AddUObject(this, &AABGASCharacterPlayer::EquipWeapon);
		// ASC 내부의 TMap에서 ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP 키를 가진 delegate를 찾고, 없으면 새로 생성 후 해당 delegate에 UnEquipWeapon을 바인딩
		ASC->GenericGameplayEventCallbacks.FindOrAdd(ABTAG_EVENT_CHARACTER_WEAPONUNEQUIP).AddUObject(this, &AABGASCharacterPlayer::UnEquipWeapon);
		
		// AttributeSet은 PlayerState에 있기 때문에 ASC를 통해 불러와서 사용
		const UABGASCharacterAttributeSet* CurrentAttributeSet = ASC->GetSet<UABGASCharacterAttributeSet>();
		CurrentAttributeSet->OnOutOfHealth.AddDynamic(this, &AABGASCharacterPlayer::OnOutOfHealth);
		
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
		EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Triggered, this, &AABGASCharacterPlayer::GASInputPressed, 2);
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

void AABGASCharacterPlayer::OnOutOfHealth()
{
	SetDead();
}

void AABGASCharacterPlayer::EquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		Weapon->SetSkeletalMesh(WeaponMesh);
		
		FGameplayAbilitySpec NewSkillSpec(SkillAbilityClass);
		NewSkillSpec.InputID = 2;
		// ASC에 해당 어빌리티가 없으면 추가
		if (!ASC->FindAbilitySpecFromClass(SkillAbilityClass))
		{
			ASC->GiveAbility(NewSkillSpec);
		}
		
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRateAttribute());
		
		// 현재 상태 + 무기 능력치로 Set
		ASC->SetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange + WeaponRange);
		ASC->SetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate + WeaponAttackRate);
	}
}

void AABGASCharacterPlayer::UnEquipWeapon(const FGameplayEventData* EventData)
{
	if (Weapon)
	{
		const float CurrentAttackRange = ASC->GetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRangeAttribute());
		const float CurrentAttackRate = ASC->GetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRateAttribute());
		
		// 현재 상태 - 무기 능력치로 Set
		ASC->SetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRangeAttribute(), CurrentAttackRange - WeaponRange);
		ASC->SetNumericAttributeBase(UABGASCharacterAttributeSet::GetAttackRateAttribute(), CurrentAttackRate - WeaponAttackRate);
		
		// ASC에서 해당 어빌리티를 찾고, 존재하면 제거
		FGameplayAbilitySpec* SkillAbilitySpec = ASC->FindAbilitySpecFromClass(SkillAbilityClass);
		if (SkillAbilitySpec)
		{
			ASC->ClearAbility(SkillAbilitySpec->Handle);
		}
		
		Weapon->SetSkeletalMesh(nullptr);
	}
}
