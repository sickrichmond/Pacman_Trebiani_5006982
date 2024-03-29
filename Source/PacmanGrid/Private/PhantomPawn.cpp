// Fill out your copyright notice in the Description page of Project Settings.


#include "PhantomPawn.h"
#include "PacmanPawn.h"
#include "Kismet/GameplayStatics.h"

APhantomPawn::APhantomPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PhantomState = Chase;
	IsEatable = false;
	BlueMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlueMesh"));
	BlueMesh->SetVisibility(false);
	WhiteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WhiteMesh"));
	WhiteMesh->SetVisibility(false);
}

void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));
}

void APhantomPawn::PhantomSpawn(int x, int y) {
	CurrentGridCoords = FVector2D(x, y);
	SetNextNode(*(CustomTileMap.Find(FVector2D(x, y))));
	SetTargetNode(NextNode);
	LastNode = (*(CustomTileMap.Find(FVector2D(x, y))));
	const FVector spawn((x*100+50), (y*100+50), this->GetActorLocation().Z);
	SetActorLocation(spawn);
}

void APhantomPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	const auto PN = Cast<APacmanPawn>(OtherActor);
	if (PN && !this->IsEatable)
	{
		if (PN->Lifes > 0) {
			//PN->Eat();
			PN->Lifes--;
			PN->GetBlinky()->PhantomSpawn(5, 26);
			PN->GetInky()->PhantomSpawn(5, 26);
			PN->GetPinky()->PhantomSpawn(27, 26);
			PN->GetClyde()->PhantomSpawn(1, 7);
			PN->PacmanSpawn();
		}
		else if (PN->Lifes == 0) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I Reached you..")));
			PN->Destroy();//ferma il gioco quando pacman vien mangiato
		}
	}
	else if (PN && this->IsEatable) {
		GetPlayer()->AddPoints(200);
		this->PhantomState = Chase;
		this->IsEatable = false;
		this->PhantomSpawn(27, 26);
		this->StaticMesh->SetVisibility(true);
		this->BlueMesh->SetVisibility(false);
		this->WhiteMesh->SetVisibility(false);
	}
}

void APhantomPawn::OnNodeReached()
{
	Super::OnNodeReached();
}

void APhantomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

APacmanPawn* APhantomPawn::GetPlayer() const
{
	return Player;
}

void APhantomPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

AGridBaseNode* APhantomPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();
}

void APhantomPawn::SetGhostTarget()
{
	const AGridBaseNode* Target = GetPlayerRelativeTarget();
	if (!Target)
	{
		Target = GetPlayer()->GetLastNode();
	}

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	const FVector Dimensions(60, 60, 20);
	DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}
}
