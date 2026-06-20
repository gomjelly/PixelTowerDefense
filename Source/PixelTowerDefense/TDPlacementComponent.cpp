// TDPlacementComponent.cpp
#include "TDPlacementComponent.h"
#include "TDGrid.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

UTDPlacementComponent::UTDPlacementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTDPlacementComponent::StartPlacement(UStaticMesh* Mesh)
{
	if (!Mesh || !TargetGrid) return;

	DestroyGhost();
	bIsPlacementMode = true;

	GhostMesh = NewObject<UStaticMeshComponent>(GetOwner());
	GhostMesh->SetStaticMesh(Mesh);
	GhostMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GhostMesh->SetCastShadow(false);
	GhostMesh->RegisterComponent();
	GhostMesh->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
}

void UTDPlacementComponent::ConfirmPlacement()
{
	if (!bIsPlacementMode || !TargetGrid) return;
	if (!TargetGrid->IsValidCell(HoveredCell) || TargetGrid->IsOccupied(HoveredCell)) return;

	TargetGrid->SetOccupied(HoveredCell, true);
	OnTowerPlaced.Broadcast(HoveredCell);

	DestroyGhost();
	bIsPlacementMode = false;
}

void UTDPlacementComponent::CancelPlacement()
{
	DestroyGhost();
	bIsPlacementMode = false;
}

void UTDPlacementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsPlacementMode)
	{
		UpdateGhostTransform();
	}
}

void UTDPlacementComponent::UpdateGhostTransform()
{
	APlayerController* PC = GetPlayerController();
	if (!PC || !TargetGrid || !GhostMesh) return;

	FVector WorldOrigin, WorldDir;
	if (!PC->DeprojectMousePositionToWorld(WorldOrigin, WorldDir)) return;

	// Intersect ray with the grid's horizontal plane (Z = grid actor Z)
	const float GridZ = TargetGrid->GetActorLocation().Z;
	if (FMath::IsNearlyZero(WorldDir.Z)) return;

	const float T = (GridZ - WorldOrigin.Z) / WorldDir.Z;
	if (T < 0.f) return;

	const FVector HitPoint = WorldOrigin + WorldDir * T;
	HoveredCell = TargetGrid->WorldToGrid(HitPoint);

	const bool bValid = TargetGrid->IsValidCell(HoveredCell) && !TargetGrid->IsOccupied(HoveredCell);

	if (TargetGrid->IsValidCell(HoveredCell))
	{
		GhostMesh->SetWorldLocation(TargetGrid->GridToWorld(HoveredCell));
		GhostMesh->SetVisibility(true);

		// Apply tint material if set
		UMaterialInterface* Mat = bValid ? ValidMaterial : InvalidMaterial;
		if (Mat)
		{
			GhostMesh->SetMaterial(0, Mat);
		}
	}
	else
	{
		GhostMesh->SetVisibility(false);
	}

	// Debug: highlight hovered cell
	if (TargetGrid->IsValidCell(HoveredCell))
	{
		FColor CellColor = bValid ? FColor::Green : FColor::Red;
		FVector Center = TargetGrid->GridToWorld(HoveredCell);
		float Half = TargetGrid->CellSize * 0.5f - 2.f;
		DrawDebugBox(GetWorld(), Center + FVector(0, 0, 2.f), FVector(Half, Half, 2.f), CellColor.ReinterpretAsLinear().ToFColor(false), false, -1.f, 0, 3.f);
	}
}

void UTDPlacementComponent::DestroyGhost()
{
	if (GhostMesh)
	{
		GhostMesh->DestroyComponent();
		GhostMesh = nullptr;
	}
}

APlayerController* UTDPlacementComponent::GetPlayerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn)
	{
		return Cast<APlayerController>(OwnerPawn->GetController());
	}
	return nullptr;
}
