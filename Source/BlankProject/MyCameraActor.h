// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "MyCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class BLANKPROJECT_API AMyCameraActor : public ACameraActor
{
	GENERATED_BODY()

		AMyCameraActor();

private:
		
	TEnumAsByte<EAutoReceiveInput::Type> AutoActivateForPlayer;

private:

	class UCameraComponent* CameraComponent;

	class USceneComponent* SceneComponent;
	

	virtual void PostLoadSubobjects(FObjectInstancingGraph* OuterInstanceGraph) override;

	virtual class USceneComponent* GetDefaultAttachComponent() const override;

protected:

	virtual void BeginPlay() override;

public:
	/** Returns CameraComponent subobject **/
	class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	/**
	 * Called to notify that this camera was cut to, so it can update things like interpolation if necessary.
	 * Typically called by the camera component.
	 */
	virtual void NotifyCameraCut() {};
};
