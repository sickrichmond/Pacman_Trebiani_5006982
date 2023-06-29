// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanPawn.h"
#include "TestGridGameMode.h"

APacmanPawn::APacmanPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//// Set this pawn to be controlled by the lowest-numbered player
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	//// vettore di direzione inizializzato con zero (il pawn non si muove allo start del gioco fino a che non
	//   viene premuto uno dei tasti W-A-S-D )
	LastInputDirection = FVector(0, 0, 0);
	LastValidInputDirection = FVector(0, 0, 0);
	////posizione iniziale  del pawn nelle coordinate di griglia (1,1)
	CurrentGridCoords = FVector2D(1, 1);
	Points = 0;
	Lifes = 3;
	ScatterCount = 0;
	CanEat = false;
	EatToWin = 300;
}

void APacmanPawn::StartScatter() {
	GetWorldTimerManager().ClearTimer(ScatterTimer);
	ScatterCount++;
	if (ScatterCount <= 4) {
		PhantomInvert();
		this->GetBlinky()->PhantomState = Scatter;
		this->GetInky()->PhantomState = Scatter;
		this->GetPinky()->PhantomState = Scatter;
		this->GetClyde()->PhantomState = Scatter;
		if (ScatterCount<=2) {
			GetWorldTimerManager().SetTimer(ScatterTimer, this, &APacmanPawn::StopScatter, 7.0f, false);
		}
		else {
			GetWorldTimerManager().SetTimer(ScatterTimer, this, &APacmanPawn::StopScatter, 5.0f, false);
		}
	}
}

void APacmanPawn::PhantomInvert() {
	this->GetBlinky()->SetLastValidDirection(-1* this->GetBlinky()->GetLastValidDirection());
	this->GetInky()->SetLastValidDirection(-1 * this->GetBlinky()->GetLastValidDirection());
	this->GetPinky()->SetLastValidDirection(-1 * this->GetBlinky()->GetLastValidDirection());
	this->GetClyde()->SetLastValidDirection(-1 * this->GetBlinky()->GetLastValidDirection());
}

void APacmanPawn::StopScatter() {
	GetWorldTimerManager().ClearTimer(ScatterTimer);
	PhantomInvert();
	this->GetBlinky()->PhantomState = Chase;
	this->GetInky()->PhantomState = Chase;
	this->GetPinky()->PhantomState = Chase;
	this->GetClyde()->PhantomState = Chase;
	if (ScatterCount <= 4) {
		GetWorldTimerManager().SetTimer(ScatterTimer, this, &APacmanPawn::StartScatter, 20.0f, false);
	}
}

void APacmanPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APacmanPawn::BeginPlay()
{
	Super::BeginPlay();
	//// posizione iniziale del pawn
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];
	BlinkyPawn = Cast<ABlinky>(UGameplayStatics::GetActorOfClass(GetWorld(), ABlinky::StaticClass()));
	InkyPawn = Cast<AInky>(UGameplayStatics::GetActorOfClass(GetWorld(), AInky::StaticClass()));
	PinkyPawn = Cast<APinky>(UGameplayStatics::GetActorOfClass(GetWorld(), APinky::StaticClass()));
	ClydePawn = Cast<AClyde>(UGameplayStatics::GetActorOfClass(GetWorld(), AClyde::StaticClass()));
	GetWorldTimerManager().SetTimer(ScatterTimer, this, &APacmanPawn::StartScatter, 20.0f, false);
}

ABlinky* APacmanPawn::GetBlinky() const
{
	return BlinkyPawn;
}

AInky* APacmanPawn::GetInky() const
{
	return InkyPawn;
}

APinky* APacmanPawn::GetPinky() const
{
	return PinkyPawn;
}

AClyde* APacmanPawn::GetClyde() const
{
	return ClydePawn;
}

void APacmanPawn::AddPoints(int Pointz) {
	Points += Pointz;
}

void APacmanPawn::SetVerticalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorForwardVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir.GetSafeNormal();
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::SetHorizontalInput(float AxisValue)
{
	if (AxisValue == 0) return;
	const FVector Dir = (GetActorRightVector() * AxisValue).GetSafeNormal();
	LastInputDirection = Dir;
	SetNextNodeByDir(LastInputDirection);
}

void APacmanPawn::OnClick()
{
	FHitResult Hit = FHitResult(ForceInit);
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, Hit);
	if (AGridBaseNode* CurrTile = Cast<AGridBaseNode>(Hit.GetActor()))
	{
		FVector2D CurrCoords = CurrTile->GetGridPosition();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Position x=%f  Y=%f "), CurrCoords.X, CurrCoords.Y));
	}
}

void APacmanPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//Bind Actions here
	InputComponent->BindAxis(TEXT("MoveForward"), this, &APacmanPawn::SetVerticalInput);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &APacmanPawn::SetHorizontalInput);
	// bind the OnClick function to InputComponent for test purpose
	InputComponent->BindAction("Click", IE_Pressed, this, &APacmanPawn::OnClick);
}



void APacmanPawn::HandleMovement()
{
	Super::HandleMovement();
	// si muove autonomamente fino a che il next node è walkable
	if (!TargetNode && !NextNode)
	{
		if (TheGridGen->IsNodeValidForWalk(TheGridGen->GetNextNode(CurrentGridCoords, LastInputDirection)))
		{
			SetLastValidDirection(LastInputDirection);
		}
		SetNodeGeneric(LastValidInputDirection);
	}
}


void APacmanPawn::SetTargetNode(AGridBaseNode* Node)
{
	Super::SetTargetNode(Node);
}

void APacmanPawn::StopEating() 
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I can't eat anymore...")));
	GetWorldTimerManager().ClearTimer(CanEatTimer);
	this->GetBlinky()->IsEatable = false;
	this->GetInky()->IsEatable = false;
	this->GetPinky()->IsEatable = false;
	this->GetClyde()->IsEatable = false;
	/*
	this->GetBlinky()->BlueMesh->SetVisibility(false);
	this->GetInky()->BlueMesh->SetVisibility(false);
	this->GetPinky()->BlueMesh->SetVisibility(false);
	this->GetClyde()->BlueMesh->SetVisibility(false);
	this->GetBlinky()->StaticMesh->SetVisibility(true);
	this->GetInky()->StaticMesh->SetVisibility(true);
	this->GetPinky()->StaticMesh->SetVisibility(true);
	this->GetClyde()->StaticMesh->SetVisibility(true);
	*/
	GetBlinky()->PhantomState = Chase;
	GetInky()->PhantomState = Chase;
	GetPinky()->PhantomState = Chase;
	GetClyde()->PhantomState = Chase;
	GetWorldTimerManager().UnPauseTimer(ScatterTimer);
}

void APacmanPawn::PacmanSpawn() {
	CurrentGridCoords = FVector2D(1, 1);
	SetNextNode(*(CustomTileMap.Find(FVector2D(1, 1))));
	SetTargetNode(NextNode);
	LastNode = (*(CustomTileMap.Find(FVector2D(1, 1))));
	const FVector spawn(150.0f, 150.0f, GetActorLocation().Z);
	SetActorLocation(spawn);
}

void APacmanPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto PowerNodeCheck = Cast<APowerNode>(OtherActor);
	if (PowerNodeCheck) {
		if (PowerNodeCheck->CanBeEat()) {
			PowerNodeCheck->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PowerNodeCheck->StaticMesh->SetVisibility(false);
			PowerNodeCheck->IsEatable = false;
			this->AddPoints(50);
			this->EatToWin--;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(this->Points));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I can eat!")));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("PACMAN ha mangiato power node")));
			this->GetBlinky()->IsEatable = true;
			this->GetInky()->IsEatable = true;
			this->GetPinky()->IsEatable = true;
			this->GetClyde()->IsEatable = true;
			/*
			* è stata inserita la logica della mesh blu per i fantasmi in frightened 
			* che però nonostante svariati tentativi non 
			* risultava visibile nel gioco
			* 
			this->GetBlinky()->StaticMesh->SetVisibility(false);
			this->GetInky()->StaticMesh->SetVisibility(false);
			this->GetPinky()->StaticMesh->SetVisibility(false);
			this->GetClyde()->StaticMesh->SetVisibility(false);
			this->GetBlinky()->BlueMesh->SetVisibility(true);
			this->GetInky()->BlueMesh->SetVisibility(true);
			this->GetPinky()->BlueMesh->SetVisibility(true);
			this->GetClyde()->BlueMesh->SetVisibility(true);
			*/
			this->GetBlinky()->PhantomState = Frightened;
			this->GetInky()->PhantomState = Frightened;
			this->GetPinky()->PhantomState = Frightened;
			this->GetClyde()->PhantomState = Frightened;
			GetWorldTimerManager().PauseTimer(ScatterTimer);
			GetWorldTimerManager().SetTimer(CanEatTimer, this, &APacmanPawn::StopEating, 6.0f, false);
		}
	}
	else {
		const auto PointNode = Cast<AGridBaseNode>(OtherActor);

		if (PointNode && PointNode->CanBeEat())
		{
			PointNode->Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			PointNode->StaticMesh->SetVisibility(false);
			PointNode->IsEatable = false;
			this->AddPoints(10);
			this->EatToWin--;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::FromInt(this->Points));
			//quando il pacman mangia un coin compare la scritta 

		}
	}
	if (this->EatToWin == 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("You won!")));
		this->Destroy();
	}
};
