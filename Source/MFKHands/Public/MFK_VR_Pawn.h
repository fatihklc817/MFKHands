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

	
	//input
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_RightGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_LeftGrip;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Move;
	//
	
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* NiagaraParticleSystem;

	UPROPERTY()
	class UNavigationSystemV1* NavSystem;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> SpawnTeleportNiagaraActor;

	UPROPERTY()
	AActor* TeleportVisualizerReference;

	UPROPERTY()
	TArray<FVector> TeleportTracePathPosition;

	UPROPERTY()
	FVector TeleportPoint;
	bool bIsValidTeleportPoint;

	UPROPERTY(EditAnywhere,Category="vr")
	float TeleportZHeight;

	
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
	void MoveStarted();

	UFUNCTION()
	void MoveTrigger();

	UFUNCTION()
	void MoveEnd();
	
	UFUNCTION()
	AActor* CheckForNearestGrabActor(UMotionControllerComponent* SelectedMotionController);
};
