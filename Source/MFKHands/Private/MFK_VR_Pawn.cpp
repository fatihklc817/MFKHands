// Fill out your copyright notice in the Description page of Project Settings.


#include "MFK_VR_Pawn.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MFKGrabComponent.h"
#include "MFKHandAnimInstance.h"
#include "MotionControllerComponent.h"
#include "NavigationSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMFK_VR_Pawn::AMFK_VR_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>("RootComp");
	RootComponent = RootComp;

	MotionControllerComponentRight = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerRight");
	MotionControllerComponentRight->SetupAttachment(RootComponent);

	HandSkeletonRight = CreateDefaultSubobject<USkeletalMeshComponent>("HandSkeletonRight");
	HandSkeletonRight->SetupAttachment(MotionControllerComponentRight);
	
	MotionControllerComponentLeft = CreateDefaultSubobject<UMotionControllerComponent>("MotionControllerLeft");
	MotionControllerComponentLeft->SetupAttachment(RootComponent);

	HandSkeletonLeft = CreateDefaultSubobject<USkeletalMeshComponent>("HandSkeletonLeft");
	HandSkeletonLeft->SetupAttachment(MotionControllerComponentLeft);

	VrCamera = CreateDefaultSubobject<UCameraComponent>("Vr Camera");
	VrCamera->SetupAttachment(RootComponent);

	HeadMountedDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>("Head Mounted Display Mesh");
	HeadMountedDisplayMesh->SetupAttachment(VrCamera);

	NiagaraParticleSystem = CreateDefaultSubobject<UNiagaraComponent>("Niagara Particle System");
	NiagaraParticleSystem -> SetupAttachment(RootComponent);
	NiagaraParticleSystem->SetVisibility(false);
	
	NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
}

// Called when the game starts or when spawned
void AMFK_VR_Pawn::BeginPlay()
{
	Super::BeginPlay();

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
		Cast<APlayerController>(GetController())->ConsoleCommand("vr.PixelDensity 1.0");

		K2_TeleportTo(FVector(GetActorLocation().X,GetActorLocation().Y,TeleportZHeight),GetActorRotation());
		
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subystem->AddMappingContext(DefaultMappingContext,0);
		}
	}
	
}

// Called every frame
void AMFK_VR_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMFK_VR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_RightGrip,ETriggerEvent::Started,this,&AMFK_VR_Pawn::RightGripStarted);
		EnhancedInputComponent->BindAction(IA_RightGrip,ETriggerEvent::Completed,this,&AMFK_VR_Pawn::RightGripEnded);
		
		EnhancedInputComponent->BindAction(IA_LeftGrip,ETriggerEvent::Started,this,&AMFK_VR_Pawn::LeftGripStarted);
		EnhancedInputComponent->BindAction(IA_LeftGrip,ETriggerEvent::Completed,this,&AMFK_VR_Pawn::LeftGripEnded);

		EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Started,this,&AMFK_VR_Pawn::MoveStarted);
		EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,this,&AMFK_VR_Pawn::MoveTrigger);
		EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Completed,this,&AMFK_VR_Pawn::MoveEnd);
		
	}

}

void AMFK_VR_Pawn::RightGripStarted()
{
	if (CurrentRighHandGrabActor != nullptr)
	{
		return;
	}
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("RightGrip"));	

	if (AActor* NearestGrabActor = CheckForNearestGrabActor(MotionControllerComponentRight))
	{
		UMFKGrabComponent* GrabComponent = Cast<UMFKGrabComponent>(NearestGrabActor->GetComponentByClass(UMFKGrabComponent::StaticClass()));
		if (GrabComponent)
		{
			UMeshComponent* MeshComponent = Cast<UMeshComponent>(NearestGrabActor->GetComponentByClass(UMeshComponent::StaticClass()));
			if (MeshComponent)
			{
				// Disable physics simulation
				MeshComponent->SetSimulatePhysics(false);
				UE_LOG(LogTemp, Warning, TEXT("Physics simulation disabled for the owner actor."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Root component is not a primitive component."));
			}
			bool bSucces = NearestGrabActor->AttachToComponent(HandSkeletonRight,FAttachmentTransformRules::SnapToTargetNotIncludingScale,GrabComponent->GetRightAttachmentSocketName());
			if (bSucces)
			{
				 CurrentRighHandGrabActor = NearestGrabActor  ;
				GrabComponent->SetHoldingHandMotionController(MotionControllerComponentRight);
			}
			UE_LOG(LogTemp, Warning, TEXT("socket name %s"),*GrabComponent->GetRightAttachmentSocketName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("is attached work %d"),bSucces);
		}

		
	UMFKHandAnimInstance* AnimInstanceRight =  Cast<UMFKHandAnimInstance>(HandSkeletonRight->GetAnimInstance());
		if (AnimInstanceRight)
		{
			AnimInstanceRight->SetGrabType(GrabComponent->GetGrabType());
		}
		
	}


	//check for near grab actors
	//get the closest grab actor
	//call its grab func. // send yourselft as a owner
	
	//right hand animation to grip to the specific actor
	
}

void AMFK_VR_Pawn::RightGripEnded()
{
	//Dettach grabbed actor
	//reset animation
	if (CurrentRighHandGrabActor)
	{
		CurrentRighHandGrabActor -> DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			UMeshComponent* MeshComponent = Cast<UMeshComponent>(CurrentRighHandGrabActor->GetComponentByClass(UMeshComponent::StaticClass()));
		MeshComponent->SetSimulatePhysics(true);
		CurrentRighHandGrabActor = nullptr;

		UMFKHandAnimInstance* AnimInstanceRight =  Cast<UMFKHandAnimInstance>(HandSkeletonRight->GetAnimInstance());
		if (AnimInstanceRight)
		{
			AnimInstanceRight->SetGrabType(EGrabType::Free);
		}
	}
}


void AMFK_VR_Pawn::LeftGripStarted()
{
	if (CurrentLeftHandGrabActor != nullptr)
	{
		return;
	}
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("RightGrip"));	

	if (AActor* NearestGrabActor = CheckForNearestGrabActor(MotionControllerComponentLeft))
	{
		UMFKGrabComponent* GrabComponent = Cast<UMFKGrabComponent>(NearestGrabActor->GetComponentByClass(UMFKGrabComponent::StaticClass()));
		if (GrabComponent)
		{
			UMeshComponent* MeshComponent = Cast<UMeshComponent>(NearestGrabActor->GetComponentByClass(UMeshComponent::StaticClass()));
			if (MeshComponent)
			{
				// Disable physics simulation
				MeshComponent->SetSimulatePhysics(false);
				UE_LOG(LogTemp, Warning, TEXT("Physics simulation disabled for the owner actor."));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Root component is not a primitive component."));
			}
			bool bSucces = NearestGrabActor->AttachToComponent(HandSkeletonLeft,FAttachmentTransformRules::SnapToTargetNotIncludingScale,GrabComponent->GetLeftAttachmentSocketName());
			if (bSucces)
			{
				CurrentLeftHandGrabActor = NearestGrabActor  ;
				GrabComponent->SetHoldingHandMotionController(MotionControllerComponentLeft);
			}
			UE_LOG(LogTemp, Warning, TEXT("socket name %s"),*GrabComponent->GetLeftAttachmentSocketName().ToString());
			UE_LOG(LogTemp, Warning, TEXT("is attached work %d"),bSucces);
		}

		
		UMFKHandAnimInstance* AnimInstanceLeft =  Cast<UMFKHandAnimInstance>(HandSkeletonLeft->GetAnimInstance());
		if (AnimInstanceLeft)
		{
			AnimInstanceLeft->SetGrabType(GrabComponent->GetGrabType());
		}
		
	}


	//check for near grab actors
	//get the closest grab actor
	//call its grab func. // send yourselft as a owner
	
	//right hand animation to grip to the specific actor
	
	
}

void AMFK_VR_Pawn::LeftGripEnded()
{
	//Dettach grabbed actor
	//reset animation
	if (CurrentLeftHandGrabActor)
	{
		CurrentLeftHandGrabActor -> DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		UMeshComponent* MeshComponent = Cast<UMeshComponent>(CurrentLeftHandGrabActor->GetComponentByClass(UMeshComponent::StaticClass()));
		MeshComponent->SetSimulatePhysics(true);
		CurrentLeftHandGrabActor = nullptr;

		UMFKHandAnimInstance* AnimInstanceLeft =  Cast<UMFKHandAnimInstance>(HandSkeletonLeft->GetAnimInstance());
		if (AnimInstanceLeft)
		{
			AnimInstanceLeft->SetGrabType(EGrabType::Free);
		}
	}
}

void AMFK_VR_Pawn::MoveStarted()
{
	if (!NiagaraParticleSystem->IsVisible())
	{
		NiagaraParticleSystem->SetVisibility(true);
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	FVector SpawnLoc = FVector(0,0,0);
	FRotator SpawnRot = FRotator(0,0,0);
	TeleportVisualizerReference = GetWorld()->SpawnActor<AActor>(SpawnTeleportNiagaraActor,SpawnLoc,SpawnRot,SpawnParameters);
}

void AMFK_VR_Pawn::MoveTrigger()
{
	FPredictProjectilePathParams PathParams;
	PathParams.StartLocation = HandSkeletonRight->GetSocketLocation("IndexSocket");
	PathParams.ProjectileRadius = 3.6;
	PathParams.bTraceWithChannel = true;
	PathParams.bTraceWithCollision = true;
	PathParams.bTraceComplex = true; 
	PathParams.TraceChannel = ECC_WorldStatic;
	PathParams.bTraceWithChannel = true;
	PathParams.DrawDebugType = EDrawDebugTrace::Type::None;
	PathParams.SimFrequency = 15;
	PathParams.MaxSimTime = 2;
			
	PathParams.LaunchVelocity = HandSkeletonRight->GetSocketTransform("IndexSocket").GetRotation().GetForwardVector() * 500;
			
			
	FPredictProjectilePathResult PathResult;
	UGameplayStatics::PredictProjectilePath(this,PathParams,PathResult);

	TeleportTracePathPosition.Empty(); // Clear previous positions

	for (const FPredictProjectilePathPointData& PointData : PathResult.PathData)
	{
		TeleportTracePathPosition.Add(PointData.Location);
	}

	TeleportTracePathPosition.Insert(HandSkeletonRight->GetSocketLocation("IndexSocket"),0);

	FNavLocation OutLocation;
	bool bValid =	NavSystem->ProjectPointToNavigation(PathResult.HitResult.Location,OutLocation,FVector(500,500,500));

	if (bValid)
	{
		TeleportPoint = OutLocation;
		bIsValidTeleportPoint = true;

		TeleportVisualizerReference->GetRootComponent()->SetVisibility(bIsValidTeleportPoint,true);
		TeleportVisualizerReference->SetActorLocation(TeleportPoint);

		UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(NiagaraParticleSystem,"User.PointArray",TeleportTracePathPosition);
		
	}
}

void AMFK_VR_Pawn::MoveEnd()
{
	if (TeleportVisualizerReference)
	{
		TeleportVisualizerReference->Destroy();
	}

	NiagaraParticleSystem->SetVisibility(false);

	if (bIsValidTeleportPoint)
	{
		bIsValidTeleportPoint  = false;

		K2_TeleportTo(FVector(TeleportPoint.X,TeleportPoint.Y,TeleportZHeight),GetActorRotation());
		UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(TeleportZHeight,EOrientPositionSelector::Position);
	}
	
}

AActor* AMFK_VR_Pawn::CheckForNearestGrabActor(UMotionControllerComponent* SelectedMotionController)
{
	FVector StartLocation = SelectedMotionController->GetComponentLocation();
	FVector EndLocation = StartLocation;

	TArray<TEnumAsByte<EObjectTypeQuery>> GrabObjectTypes;
	GrabObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_PhysicsBody));
	TArray<FHitResult> HitResults;
	TArray<AActor*> ActorsToIgnore;
	float Distance = 99999999999;
	AActor* NearestGrabActor = nullptr;
	
	if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),StartLocation,EndLocation,GrabSphereRadius,GrabObjectTypes,false,ActorsToIgnore,EDrawDebugTrace::Type::None,HitResults,true))
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("Grab Draww"));

		for (FHitResult hitresult : HitResults)
		{
			AActor* HitActor = hitresult.GetActor();
			FString HitActorName = HitActor ? HitActor->GetHumanReadableName() : TEXT("None");
			FString DebugMessage = FString::Printf(TEXT("Hit: %s"), *HitActorName);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, DebugMessage);

			if (UMFKGrabComponent* GrabComponent = Cast<UMFKGrabComponent>(HitActor->GetComponentByClass(UMFKGrabComponent::StaticClass())))
			{
				float currentDistance = (HitActor->GetActorLocation() - this->GetActorLocation()) . Length();
				if (currentDistance < Distance)
				{
					NearestGrabActor = HitActor;
				}
			}
		 	
		}

		if (NearestGrabActor)
		{
			return NearestGrabActor;
		}
	}
	
	return nullptr;
}

