// Fill out your copyright notice in the Description page of Project Settings.

#include "Effluvium_UE4.h"
#include "Fluid_Array.h"
#include "Effluvium_UE4Projectile.h"


// Sets default values
AFluid_Array::AFluid_Array()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFluid_Array::BeginPlay()
{
	Super::BeginPlay();

  // try and fire a projectile
  if (ProjectileClass != NULL)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      const FRotator SpawnRotation = FRotator(0, 0, 0);
      const FVector SpawnLocation = FVector(0, 0, 300);
      World->SpawnActor<AEffluvium_UE4Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
    }
  }
	
}

// Called every frame
void AFluid_Array::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );


  if (counter >= 59) {
    // try and fire a projectile
    if (ProjectileClass != NULL)
    {
      UWorld* const World = GetWorld();
      if (World != NULL)
      {
        const FRotator SpawnRotation = FRotator(0, 0, 0);
        const FVector SpawnLocation = FVector(0, 0, 300);
        World->SpawnActor<AEffluvium_UE4Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
      }
    }
  }

  counter++;
  counter %= 60;

}

