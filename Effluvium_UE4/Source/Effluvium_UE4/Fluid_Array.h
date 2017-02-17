// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Fluid_Array.generated.h"

UCLASS()
class EFFLUVIUM_UE4_API AFluid_Array : public AActor
{
	GENERATED_BODY()
	
public:	
  
  int counter;
	
  // Sets default values for this actor's properties
	AFluid_Array();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = Projectile)
    TSubclassOf<class AEffluvium_UE4Projectile> ProjectileClass;
	

};
