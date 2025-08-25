#include "QWeapon.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AQWeapon::AQWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComponent);

	Pickup = CreateDefaultSubobject<UQuadShooterPickUpComponent>("Pickup");
	Pickup->SetupAttachment(WeaponMesh);
	
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	
}

// Called when the game starts or when spawned
void AQWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AQWeapon::AttachWeapon(AQuadShooterCharacter* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr ||Character->GetInstanceComponents().FindItemByClass<AQWeapon>())
	{
		return false;
	}
	
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	WeaponMesh->AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		SetOwner(PlayerController);
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
		
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AQWeapon::Fire);
		}
	}
	
	return true;
}

void AQWeapon::Fire()
{
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
			const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			World->SpawnActor<AQuadShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			if (!HasAuthority())
			{
				ServerFire(SpawnLocation, SpawnRotation);
			}
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

bool AQWeapon::ServerFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerFire Validation"));
	return true;
}

void AQWeapon::ServerFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	UE_LOG(LogTemp, Warning, TEXT("ServerFire implementation called"));

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
	GetWorld()->SpawnActor<AQuadShooterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);

}

// Called every frame
void AQWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

