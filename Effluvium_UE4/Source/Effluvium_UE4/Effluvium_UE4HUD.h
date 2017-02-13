// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "Effluvium_UE4HUD.generated.h"

UCLASS()
class AEffluvium_UE4HUD : public AHUD
{
	GENERATED_BODY()

public:
	AEffluvium_UE4HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

