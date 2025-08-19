#include "QuadShooterWeaponComponent.h"
#include "QuadShooterCharacter.h"
#include "QuadShooterProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"


UQuadShooterWeaponComponent::UQuadShooterWeaponComponent()
{
	
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}


void UQuadShooterWeaponComponent::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Firing On Client"));
	
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
 
	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			World->SpawnActor<AQuadShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			ServerFire(SpawnLocation, SpawnRotation);
		}
	}
	
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}
	
	if (FireAnimation != nullptr)
	{
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

//validates can fire
bool UQuadShooterWeaponComponent::ServerFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	return false;
	 
}

void UQuadShooterWeaponComponent::ServerFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerFireImplementation called"));
}

bool UQuadShooterWeaponComponent::AttachWeapon(AQuadShooterCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UQuadShooterWeaponComponent>())
	{
		return false;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UQuadShooterWeaponComponent::Fire);
		}
	}

	return true;
}

void UQuadShooterWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character != nullptr)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(FireMappingContext);
			}
		}
	}

	Super::EndPlay(EndPlayReason);
}