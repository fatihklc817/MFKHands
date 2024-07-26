// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrapType.h"
#include "Animation/AnimInstance.h"
#include "MFKHandAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MFKHANDS_API UMFKHandAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EGrabType CurrentGrabType;


public:
	UFUNCTION()
	void SetGrabType(EGrabType TargetGrabType);
	
};
