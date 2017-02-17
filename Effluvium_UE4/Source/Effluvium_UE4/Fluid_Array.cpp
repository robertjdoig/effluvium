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
      for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
          AActor* t_ref= World->SpawnActor<AActor>(ProjectileClass, FVector(100, 100+(i*100), 400+(j*100)), FRotator::ZeroRotator);
          refs.Add(t_ref);
        }
      }
    
    }
  }
	
}

// Called every frame
void AFluid_Array::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

  for (int i = 0; i < col; i++) {
    for (int j = 0; j < row; j++) {
      //refs[i + (col*j)]->SetActorLocation(FVector(DeltaTime * 100,0,500));
      refs[i + (col*j)]->SetActorRelativeRotation(FRotator(DeltaTime * 100, 0,0));
    }
  }
}

