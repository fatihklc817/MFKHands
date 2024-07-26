// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MFKGrabbableActor.generated.h"

UCLASS()
class MFKHANDS_API AMFKGrabbableActor : public AActor
{
	GENERATED_BODY()

	
public:	
	AMFKGrabbableActor();

protected:

	virtual void BeginPlay() override;



};
