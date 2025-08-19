// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "QuadShooterWeaponComponent.generated.h"

class AQuadShooterCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUADSHOOTER_API UQuadShooterWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AQuadShooterProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	
	UQuadShooterWeaponComponent();


	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AQuadShooterCharacter* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(FVector MuzzleLocation, FRotator MuzzleRotation);
	bool ServerFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);
	void ServerFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

protected:
	
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AQuadShooterCharacter* Character;
};
