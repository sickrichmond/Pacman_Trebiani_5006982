// Fill out your copyright notice in the Description page of Project Settings.


#include "Blinky.h"

AGridBaseNode* ABlinky::GetPlayerRelativeTarget()
{
	FVector2D coords;
	if (this->PhantomState == Chase) {
		return Super::GetPlayerRelativeTarget();
	}
	else if (this->PhantomState == Scatter) {
		coords.X = 30, coords.Y = 0;
		return TheGridGen->GetNodeByCoords(coords);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Frightened!")));
		int32 XCoords = rand() % 31;
		int32 YCoords = rand() % 30;
		coords.X = XCoords, coords.Y = YCoords;
		return TheGridGen->GetNodeByCoords(coords);
	}
}
ABlinky::ABlinky()
{
	CurrentGridCoords = FVector2D(27, 26);
}
