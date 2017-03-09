// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "R3D_Fluid.generated.h"

UCLASS()
class EFFLUVIUM_UE4_API AR3D_Fluid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AR3D_Fluid();

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
    TArray<UMaterialInstanceDynamic*> materials;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
    FTransform world_trans;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
    FVector dimensions;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
    FVector object_size;

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

  void set_boundary(FVector dim, int b, float * x);
  void gauss_siedel(FVector dim, int b, float * x, float * x0, float a, float c);
  void diffuse(FVector dim, int b, float * x, float * x0, float diff, float dt);
  void advect(FVector dim, int b, float * d, float * d0, float * u, float * v, float dt);
  void project(FVector dim, float * u, float * v, float * p, float * div);
  void density_step(FVector dim, float * x, float * x0, float * u, float * v, float diff, float dt);
  void velocity_step(FVector dim, float * u, float * v, float * u0, float * v0, float visc, float dt);

  int IX(int i, int j, FVector dim);
	
  /** UI TESTING */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
  float vx_speed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
  float vy_speed;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
  float density_amount;


};
