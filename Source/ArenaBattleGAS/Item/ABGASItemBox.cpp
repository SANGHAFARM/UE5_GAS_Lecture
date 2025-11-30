// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ABGASItemBox.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Physics/ABCollision.h"

AABGASItemBox::AABGASItemBox()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("ASC"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);
	
	Trigger->SetCollisionProfileName(CPROFILE_ABTRIGGER);
	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMeshRef(TEXT("/Game/ArenaBattle/Environment/Props/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (BoxMeshRef.Object)
	{
		Mesh->SetStaticMesh(BoxMeshRef.Object);
	}
	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}

UAbilitySystemComponent* AABGASItemBox::GetAbilitySystemComponent() const
{
	return ASC;
}

void AABGASItemBox::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	InvokeGameplayCue(OtherActor);
	ApplyEffectToTarget(OtherActor);
	
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	SetLifeSpan(2.0f);
}

void AABGASItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ASC->InitAbilityActorInfo(this, this);
}

void AABGASItemBox::ApplyEffectToTarget(AActor* Target)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	if (TargetASC)
	{
		// TargetASC의 EffectContextHandle 생성
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		// EffectContextHandle과 GameplayEffectClass로 EffectSpecHandle 생성
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1, EffectContextHandle);
		if (EffectSpecHandle.IsValid())
		{
			// 생성된 GameplayEffectSpec을 자기 자신에게 적용
			TargetASC->BP_ApplyGameplayEffectSpecToSelf(EffectSpecHandle);
		}
	}
}

void AABGASItemBox::InvokeGameplayCue(AActor* Target)
{
	FGameplayCueParameters CueParam;
	CueParam.SourceObject = this;
	CueParam.Instigator = Target;
	CueParam.Location = GetActorLocation();
	ASC->ExecuteGameplayCue(GameplayCueTag, CueParam);
}
