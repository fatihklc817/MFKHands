// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrapType.h"
#include "Components/ActorComponent.h"
#include "MFKGrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MFKHANDS_API UMFKGrabComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	EGrabType GrabType;

	UPROPERTY(EditAnywhere)
	FName SocketNameRight;

	UPROPERTY(EditAnywhere)
	FName SocketNameLeft;

	UPROPERTY(EditAnywhere)
	class UMotionControllerComponent* HoldingHandMotionController;

	UPROPERTY()
	bool bIsObjectGrabbed;

	UPROPERTY()
	FTransform ObjectStartTransform;

	UPROPERTY()
	USkeletalMeshComponent* HoldingSkeletalMeshComponent;
public:	
	
	UMFKGrabComponent();

protected:
	
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void Grab(USkeletalMeshComponent* HandSkeleton);

	UFUNCTION()
	FName GetRightAttachmentSocketName();

	UFUNCTION()
	FName GetLeftAttachmentSocketName();

	UFUNCTION()
	EGrabType GetGrabType();

	UFUNCTION()
	void SetHoldingHandMotionController(UMotionControllerComponent* GrabHandMotionController);

	UFUNCTION(BlueprintCallable)
	UMotionControllerComponent* GetHoldingMotionController();

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetHoldingSkeletalMeshComponent();

	UFUNCTION()
	void SetHoldingSkeletalMeshComp(USkeletalMeshComponent* InSkeletalMesh);

	UFUNCTION()
	void SetIsObjectGrabbed(bool BoolValue);

	UFUNCTION()
	bool GetIsObjectGrabbed();

	UFUNCTION()
	void ResetObjectToStartPos();
};
