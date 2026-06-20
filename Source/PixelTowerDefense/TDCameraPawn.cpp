// TDCameraPawn.cpp
#include "TDCameraPawn.h"
#include "TDPlacementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"

ATDCameraPawn::ATDCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 1500.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->SetRelativeRotation(FRotator(CameraPitch, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	PlacementComponent = CreateDefaultSubobject<UTDPlacementComponent>(TEXT("PlacementComponent"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ATDCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ATDCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATDCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ZoomAction)
		{
			EnhancedInput->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ATDCameraPawn::Zoom);
		}
		if (PanAction)
		{
			EnhancedInput->BindAction(PanAction, ETriggerEvent::Triggered, this, &ATDCameraPawn::Pan);
		}
		if (PanActivateAction)
		{
			EnhancedInput->BindAction(PanActivateAction, ETriggerEvent::Started, this, &ATDCameraPawn::PanStart);
			EnhancedInput->BindAction(PanActivateAction, ETriggerEvent::Completed, this, &ATDCameraPawn::PanEnd);
		}
		if (PlaceConfirmAction)
		{
			EnhancedInput->BindAction(PlaceConfirmAction, ETriggerEvent::Started, this, &ATDCameraPawn::PlaceConfirm);
		}
		if (PlaceCancelAction)
		{
			EnhancedInput->BindAction(PlaceCancelAction, ETriggerEvent::Started, this, &ATDCameraPawn::PlaceCancel);
		}
	}
}

void ATDCameraPawn::PlaceConfirm(const FInputActionValue& Value)
{
	if (PlacementComponent && PlacementComponent->IsInPlacementMode())
	{
		PlacementComponent->ConfirmPlacement();
	}
}

void ATDCameraPawn::PlaceCancel(const FInputActionValue& Value)
{
	if (PlacementComponent && PlacementComponent->IsInPlacementMode())
	{
		PlacementComponent->CancelPlacement();
	}
}

void ATDCameraPawn::Zoom(const FInputActionValue& Value)
{
	const float ZoomValue = Value.Get<float>();

	if (ZoomValue != 0.f && SpringArm)
	{
		float NewLength = SpringArm->TargetArmLength - (ZoomValue * ZoomSpeed);
		SpringArm->TargetArmLength = FMath::Clamp(NewLength, MinZoom, MaxZoom);
	}
}

void ATDCameraPawn::PanStart(const FInputActionValue& Value)
{
	bIsPanning = true;
}

void ATDCameraPawn::PanEnd(const FInputActionValue& Value)
{
	bIsPanning = false;
}

void ATDCameraPawn::Pan(const FInputActionValue& Value)
{
	if (!bIsPanning)
	{
		return;
	}

	const FVector2D PanVector = Value.Get<FVector2D>();

	// ���콺�� ���������� �巡���ϸ� ī�޶�� �������� �̵� (�ڿ������� �巡�� �д�)
	AddMovementInput(FVector(0.f, 1.f, 0.f), -PanVector.X * PanSpeed);
	AddMovementInput(FVector(1.f, 0.f, 0.f), -PanVector.Y * PanSpeed);
}