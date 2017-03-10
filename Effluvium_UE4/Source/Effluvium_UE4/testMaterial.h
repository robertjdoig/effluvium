// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "testMaterial.generated.h"

UCLASS()
class EFFLUVIUM_UE4_API AtestMaterial : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AtestMaterial();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  UTexture2D* Texture; 

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  AActor* obj;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Array_Ob)
  TArray<UStaticMeshComponent*> m_plane;
  UMaterialInterface* mat_ref; 

  int SizeX, SizeY;
  int Level; 

  int DataSize; 
  void* Data;
  float NewData[40000]; 
  TArray<FColor> colors;
};
