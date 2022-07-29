// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCameraActor.h"
#include "Camera/CameraActor.h"
#include "Engine/World.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"



AMyCameraActor::AMyCameraActor()
	
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = SceneComponent;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SceneComponent);
	CameraComponent->FieldOfView = 90.f;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->AspectRatio = 1.777778f;
	CameraComponent->PostProcessBlendWeight = 1.0f;

}


void AMyCameraActor::PostLoadSubobjects(FObjectInstancingGraph * OuterInstanceGraph)
{

	USceneComponent* OldRoot = RootComponent;
	USceneComponent* OldAttachParent = OldRoot->GetAttachParent();
	const FName OldSocketName = OldRoot->GetAttachSocketName();

	Super::PostLoadSubobjects(OuterInstanceGraph);

	if (GetLinkerUE4Version() < VER_UE4_CAMERA_ACTOR_USING_CAMERA_COMPONENT)
	{
		CameraComponent->SetupAttachment(OldAttachParent, OldSocketName);
		OldRoot->SetupAttachment(nullptr);
	}

	if (GetLinkerUE4Version() < VER_UE4_CAMERA_COMPONENT_ATTACH_TO_ROOT)
	{

		RootComponent = SceneComponent;
		if (OldAttachParent != SceneComponent) {
			CameraComponent->SetupAttachment(RootComponent);
			RootComponent->SetupAttachment(OldAttachParent, OldSocketName);

		}
		
	}
}

USceneComponent * AMyCameraActor::GetDefaultAttachComponent() const
{
	return nullptr;
}

void AMyCameraActor::BeginPlay()
{
	if (AutoActivateForPlayer)// != EAutoReceiveInput::Disabled && GetNetMode() != NM_Client
	{
		const int PlayerIndex = GetAutoActivatePlayerIndex();

		GetWorld()->RegisterAutoActivateCamera(this, PlayerIndex);

		APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
		if (PC) 
		{
			PC->SetViewTarget(this);
			
		}
		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
	Super::BeginPlay();
	
}
