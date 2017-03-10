// Fill out your copyright notice in the Description page of Project Settings.

#include "Effluvium_UE4.h"
#include "testMaterial.h"


// Sets default values
AtestMaterial::AtestMaterial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AtestMaterial::BeginPlay()
{
	Super::BeginPlay();
	
  SizeX = 100;
  SizeY = 100;
  Level = 0; 
  DataSize = SizeX*SizeY;
  

  Texture = UTexture2D::CreateTransient(SizeX, SizeY);
  FTexture2DMipMap& Mip = Texture->PlatformData->Mips[Level];
  Data = Mip.BulkData.Lock(LOCK_READ_WRITE);
  
  
  //NewData[0] = 255; // Set the Blue channel in pixel nr. 0 to 255
  //NewData[1] = 120; // Set the Green channel in pixel nr. 0 to 120
  //NewData[2] = 120; // Set the Red channel in pixel nr. 0 to 120
  //NewData[3] = 120; // Set the Alpha channel in pixel nr. 0 to 120
  //NewData[4] = 120; // Set the Blue channel in pixel nr. 1 to 120 
  
  for( int i = 0; i < DataSize; i++){
    NewData[i] = 125; 
  }

  //TArray<FColor> rawData;
  for (int32 i = 0; i < DataSize; i++)
  {
    colors.Add(FColor(255, 0, 0, 255));
  }


  FMemory::Memcpy(Data, colors.GetData(), DataSize*4);
  
  Mip.BulkData.Unlock();
  Texture->UpdateResource();

  obj->GetComponents<UStaticMeshComponent>(m_plane);
  mat_ref = m_plane[0]->GetMaterial(0);
  UMaterialInstanceDynamic* Mat = m_plane[0]->CreateDynamicMaterialInstance(0);

  Mat->SetTextureParameterValue("TestTex", Texture);


  //mat_ref->SetMaterial(Texture);
}

// Called every frame
void AtestMaterial::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

    FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
    Data = Mip.BulkData.Lock(LOCK_READ_WRITE);

    for (int i = 0; i < DataSize; ++i)
    {   
      colors[i] = FColor(255,0,0,255);
    }
    int x = 20, y = 20, w = 50, h = 50;
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h; j++) {
        colors[i +(SizeX * j)] = FColor(255, 255, 255, 255);

      }

    }

    FMemory::Memcpy(Data, colors.GetData(), DataSize*4);

    Mip.BulkData.Unlock();
    Texture->UpdateResource();

    obj->GetComponents<UStaticMeshComponent>(m_plane);
    mat_ref = m_plane[0]->GetMaterial(0);
    UMaterialInstanceDynamic* Mat = m_plane[0]->CreateDynamicMaterialInstance(0);

    Mat->SetTextureParameterValue("TestTex", Texture);
 // }
}
