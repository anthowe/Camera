// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Thing1.generated.h"

UCLASS()
class BLANKPROJECT_API AThing1 : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AThing1();

	class UStaticMeshComponent* StaticMesh;

	class UStaticMeshComponent* OtherMesh;

	class APlayerController* PC;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Pawn Setup")
	class USphereComponent* SphereComponent;

	class UThing1PawnMovementComponent* OurMovementComponent;

	UMaterial* StoredMaterial;
	UMaterial* StoredMaterial1;

	UMaterialInstanceDynamic* DynamicMaterialInst;
	UMaterialInstanceDynamic* DynamicMaterialInst1;

	UParticleSystemComponent* Thing1Particles;

	float BaseTurnRate;
	float LookUpAtRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void MoveForward(float Val);

	void MoveRight(float Val);

	void MoveUp(float Val);
	
	

	void Turn(float Val);

	void LookUp(float Val);
};
