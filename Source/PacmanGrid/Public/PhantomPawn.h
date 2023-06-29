// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridPawn.h"
#include "PhantomPawn.generated.h"

UENUM()
enum EPhantomstateId { Chase, Scatter, Frightened };

UCLASS()
class PACMANGRID_API APhantomPawn : public AGridPawn
{
	GENERATED_BODY()
public:
	// Sets default values for this pawn's properties
	APhantomPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnNodeReached() override;

private:
	UPROPERTY(VisibleAnywhere)
		class APacmanPawn* Player;

public:
	virtual void Tick(float DeltaTime) override;
	void PhantomSpawn(int x, int y);
	UFUNCTION()
		APacmanPawn* GetPlayer() const;
	UFUNCTION()
		void SetSpeed(float Speed);
	UFUNCTION()
		virtual AGridBaseNode* GetPlayerRelativeTarget();
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BlueMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* WhiteMesh;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* PhantomMesh;
	UPROPERTY(EditAnywhere)
		TEnumAsByte<EPhantomstateId> PhantomState;
	UPROPERTY(EditAnywhere)
	bool IsEatable;
	void SetGhostTarget();
};
