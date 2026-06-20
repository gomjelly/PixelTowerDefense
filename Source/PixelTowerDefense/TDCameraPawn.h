// TDCameraPawn.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "TDCameraPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UInputAction;
class UInputMappingContext;
class UTDPlacementComponent;

UCLASS()
class PIXELTOWERDEFENSE_API ATDCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ATDCameraPawn();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UFloatingPawnMovement* MovementComponent;

	// ===== Enhanced Input ���� =====
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PanAction;          // ���콺 �巡�� �д׿� (Axis2D - ���콺 X/Y �̵���)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PanActivateAction;  // ��Ŭ�� ���� ���� (Bool)

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PlaceConfirmAction; // Left click - 배치 확정

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PlaceCancelAction;  // Escape - 배치 취소

	// ===== Placement =====
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Placement")
	UTDPlacementComponent* PlacementComponent;

	// ===== �д� ���� =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Pan")
	float PanSpeed = 3.0f;

	// ===== �� ���� =====
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float ZoomSpeed = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float MinZoom = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
	float MaxZoom = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Angle")
	float CameraPitch = -50.f;

private:
	bool bIsPanning = false;

	void Zoom(const FInputActionValue& Value);
	void Pan(const FInputActionValue& Value);
	void PanStart(const FInputActionValue& Value);
	void PanEnd(const FInputActionValue& Value);
	void PlaceConfirm(const FInputActionValue& Value);
	void PlaceCancel(const FInputActionValue& Value);
};