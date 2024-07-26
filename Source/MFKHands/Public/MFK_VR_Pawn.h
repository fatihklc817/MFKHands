// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MFK_VR_Pawn.generated.h"

UCLASS()
class MFKHANDS_API AMFK_VR_Pawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootComp;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerComponentRight;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USkeletalMeshComponent* HandSkeletonRight;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UMotionControllerComponent* MotionControllerComponentLeft;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	class USkeletalMeshComponent* HandSkeletonLeft;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* VrCamera;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* HeadMountedDisplayMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_RightGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_LeftGrip;

	// UPROPERTY(EditDefaultsOnly)
	// class UNiagaraComponent* NiagaraParticleSystem;

	UPROPERTY(EditDefaultsOnly)
	float GrabSphereRadius;

	UPROPERTY()
	AActor* CurrentRighHandGrabActor;

	UPROPERTY()
	AActor* CurrentLeftHandGrabActor;
	
public:
	// Sets default values for this pawn's properties
	AMFK_VR_Pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UFUNCTION()
	void RightGripStarted();

	UFUNCTION()
	void RightGripEnded();

	UFUNCTION()
	void LeftGripStarted();
	
	UFUNCTION()
	void LeftGripEnded();

	UFUNCTION()
	AActor* CheckForNearestGrabActor(UMotionControllerComponent* SelectedMotionController);
};
