// Fill out your copyright notice in the Description page of Project Settings.


#include "MFKGrabComponent.h"

#include "ComponentUtils.h"
#include "MFK_VR_Pawn.h"
#include "MotionControllerComponent.h"


UMFKGrabComponent::UMFKGrabComponent()
{

	PrimaryComponentTick.bCanEverTick = false;

	bIsObjectGrabbed = false;
}



void UMFKGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	//ObjectStartTransform = GetOwner()->GetActorTransform();

	
}

void UMFKGrabComponent::Grab(USkeletalMeshComponent* HandSkeleton)
{
	//attach this to grabhand parameter skeletal
	UE_LOG(LogTemp, Warning, TEXT(" GrabComponentLog"));
	
	
	
	//Cast<AActor>(ComponentUtils::GetAttachedParent());
	
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		// Get the root component of the owner actor
		UMeshComponent* MeshComponent = Cast<UMeshComponent>(OwnerActor->GetComponentByClass(UMeshComponent::StaticClass()));
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
		
		
	}
	
	
	
	// //GetOwner()->K2_AttachRootComponentTo(GrabHand,SocketName,EAttachLocation::KeepWorldPosition);
	// // GetOwner()->AttachToActor()
	// GetOwner()->attach
	// GetOwner()->AttachToComponent(GrabHand,FAttachmentTransformRules::SnapToTargetIncludingScale,SocketName);

	
	
}

FName UMFKGrabComponent::GetRightAttachmentSocketName()
{
	return SocketNameRight;
}

FName UMFKGrabComponent::GetLeftAttachmentSocketName()
{
	return SocketNameLeft;
}

EGrabType UMFKGrabComponent::GetGrabType()
{
	return GrabType;
}

void UMFKGrabComponent::SetHoldingHandMotionController(UMotionControllerComponent* GrabHandMotionController)
{
	HoldingHandMotionController = GrabHandMotionController;
}

UMotionControllerComponent* UMFKGrabComponent::GetHoldingMotionController()
{
	return HoldingHandMotionController;
}

USkeletalMeshComponent* UMFKGrabComponent::GetHoldingSkeletalMeshComponent()
{
	if (HoldingSkeletalMeshComponent)
	{
		return HoldingSkeletalMeshComponent;
	}
	
	return nullptr;
}

void UMFKGrabComponent::SetHoldingSkeletalMeshComp(USkeletalMeshComponent* InSkeletalMesh)
{
	HoldingSkeletalMeshComponent = InSkeletalMesh;
}

void UMFKGrabComponent::SetIsObjectGrabbed(bool BoolValue)
{
	bIsObjectGrabbed = BoolValue;
}

bool UMFKGrabComponent::GetIsObjectGrabbed()
{
	return bIsObjectGrabbed;
}

void UMFKGrabComponent::ResetObjectToStartPos()
{
	AActor* OwnerActor = GetOwner();
	OwnerActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//OwnerActor->SetActorTransform(ObjectStartTransform);
	
	if (UMeshComponent* MeshComponent = Cast<UMeshComponent>(OwnerActor->GetComponentByClass(UMeshComponent::StaticClass())))
	{
		MeshComponent->SetSimulatePhysics(true);
	
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		SetIsObjectGrabbed(false);
		
	},
	1.5f,
	false
	);
	
	
}







