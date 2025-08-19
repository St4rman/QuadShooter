// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../QuadShooterProjectile.h"
#include "QuadShooter/QuadShooterPickUpComponent.h"
#include "QWeapon.generated.h"

UCLASS()
class QUADSHOOTER_API AQWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	
	AQWeapon();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	UQuadShooterPickUpComponent* Pickup;
	
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

	UFUNCTION(BlueprintCallable, Category="Weapon")
	bool AttachWeapon(AQuadShooterCharacter* TargetCharacter);
	
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire(FVector MuzzleLocation, FRotator MuzzleRotation);
	bool ServerFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);
	void ServerFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
protected:


private:
	/** The Character holding this weapon*/
	AQuadShooterCharacter* Character;
};
