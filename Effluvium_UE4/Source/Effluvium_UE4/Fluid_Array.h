// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Fluid_Array.generated.h"

UCLASS()
class EFFLUVIUM_UE4_API AFluid_Array : public AActor
{
	GENERATED_BODY()
	
public:	
 	
  // Sets default values for this actor's properties
	AFluid_Array();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

  /** Projectile class to spawn */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
    TSubclassOf<class AActor> ProjectileClass;
	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<AActor*> refs;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  FVector dimensions;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> density;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> prev_density;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> vx;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> vy;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> prev_vx;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<float> prev_vy;

  void set_boundary(int N, int b, float * x);
  void gauss_siedel(int N, int b, float * x, float * x0, float a, float c);
  void diffuse(int N, int b, float * x, float * x0, float diff, float dt);
  void advect(int N, int b, float * d, float * d0, float * u, float * v, float dt);
  void project(int N, float * u, float * v, float * p, float * div);
  void density_step(int N, float * x, float * x0, float * u, float * v, float diff, float dt);
  void velocity_step(int N, float * u, float * v, float * u0, float * v0, float visc, float dt);

  int IX(int i, int j, int N);
};