// Weichao Qiu @ 2017
// Attach two lit sensor and two depth sensor components

#include "StereoCameraActor.h"

AStereoCameraActor::AStereoCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), BaseLineDistance(80)
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Make the scene component the root component
	RootComponent = SceneComponent;

	LeftSensor = CreateDefaultSubobject<UFusionCamSensor>(TEXT("LeftSensor"));
	LeftSensor->SetupAttachment(SceneComponent);

	RightSensor = CreateDefaultSubobject<UFusionCamSensor>(TEXT("RightSensor"));
	RightSensor->SetupAttachment(SceneComponent);

	LeftSensor->SetRelativeLocation (FVector(0, - BaseLineDistance / 2, 0));
	RightSensor->SetRelativeLocation(FVector(0, + BaseLineDistance / 2, 0));
}

void AStereoCameraActor::BeginPlay()
{
	Super::BeginPlay(); // Enable ticking actor
}

TArray<FString> AStereoCameraActor::GetSensorNames()
{
	return { 
		this->GetName() + TEXT("_") + LeftSensor->GetName(),
		this->GetName() + TEXT("_") + RightSensor->GetName(),
	};
}

TArray<UFusionCamSensor*> AStereoCameraActor::GetSensors()
{
	return {
		this->LeftSensor,
		this->RightSensor,
	};
}