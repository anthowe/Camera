// Fill out your copyright notice in the Description page of Project Settings.


#include "Thing1.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Public/UObject/ConstructorHelpers.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Thing1PawnMovementComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AThing1::AThing1()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->InitSphereRadius(1.f);
	SphereComponent->bVisible = true;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual Representation"));
	RootComponent = StaticMesh;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));

	

	if (SphereVisualAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(SphereVisualAsset.Object);
		StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		//StaticMesh->IsSimulatingPhysics();
		StaticMesh->SetWorldScale3D(FVector(.8f));
	}

	OtherMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Other Mesh"));
	OtherMesh->SetupAttachment(StaticMesh);
	//OtherMesh->AddLocalOffset(FVector(0.f, 0.f, 10.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Pyramid(TEXT("/Game/StarterContent/Shapes/Shape_QuadPyramid.Shape_QuadPyramid"));

	if (Pyramid.Succeeded())
	{
		OtherMesh->SetStaticMesh(Pyramid.Object);
		OtherMesh->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
		OtherMesh->SetWorldScale3D(FVector(.5f));
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> MatObj(TEXT("/Game/M_Thing1.M_Thing1"));
	static ConstructorHelpers::FObjectFinder<UMaterial> MatObj1(TEXT("/Game/M_Thing2.M_Thing2"));
	
	if (MatObj.Succeeded()) 
	{
		StoredMaterial = MatObj.Object;
	}

	DynamicMaterialInst = UMaterialInstanceDynamic::Create(StoredMaterial, StaticMesh);

	if (MatObj1.Succeeded())
	{
		StoredMaterial1 = MatObj1.Object;
	}

	DynamicMaterialInst1 = UMaterialInstanceDynamic::Create(StoredMaterial1, OtherMesh);

	StaticMesh->SetMaterial(0, DynamicMaterialInst);
	OtherMesh->SetMaterial(0, DynamicMaterialInst1);

	
	PC = CreateDefaultSubobject<APlayerController>(TEXT("Player Controller"));
	PC->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

	//UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(MatObj.Object, this);



	
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	OurMovementComponent = CreateDefaultSubobject<UThing1PawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	Thing1Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Thing 1 Particles"));
	Thing1Particles->SetupAttachment(RootComponent);
	Thing1Particles->SetWorldScale3D(FVector(1.f));


	static ConstructorHelpers::FObjectFinder<UParticleSystem>ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Steam_Lit.P_Steam_Lit"));

	if (ParticleAsset.Succeeded()) 
	{
		Thing1Particles->SetTemplate(ParticleAsset.Object);
	}

	BaseTurnRate = 20.f;
	LookUpAtRate = 45.f;
	
}

// Called when the game starts or when spawned
void AThing1::BeginPlay()
{
	Super::BeginPlay();
	

		

	
	
	
}

// Called every frame
void AThing1::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThing1::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThing1::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThing1::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AThing1::MoveUp);
	PlayerInputComponent->BindAxis("Turn", this, &AThing1::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AThing1::LookUp);
}

void AThing1::MoveForward(float Val)
{
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	
	if (Val != 0) 
	{
		AddMovementInput(Direction * Val);
		UE_LOG(LogTemp, Warning, TEXT("W pressed"));


	}

	
}

void AThing1::MoveRight(float Val)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	

	if (Val != 0)
	{
		AddMovementInput(Direction, (Val));
		UE_LOG(LogTemp, Warning, TEXT("A pressed"));


	}
}

void AThing1::MoveUp(float Val)
{
	
	if (Val != 0) 
	{
		AddMovementInput(GetActorUpVector() * Val );
	}
	
}

void AThing1::Turn(float Val)
{
	

	if (Val != 0) 
	{

		AddControllerYawInput(BaseTurnRate * Val * GetWorld()->GetDeltaSeconds());

		UE_LOG(LogTemp, Warning, TEXT("Turn pressed"));

	}
	
}

void AThing1::LookUp(float Val)

{
	if (Val != 0)
	{
		AddControllerPitchInput(LookUpAtRate * Val * GetWorld()->GetDeltaSeconds());

		UE_LOG(LogTemp, Warning, TEXT("Lookup pressed"));
	}
	
}


