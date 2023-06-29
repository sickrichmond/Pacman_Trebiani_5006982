// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PacmanGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PACMANGRID_API UPacmanGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int Points = 0;
	UPROPERTY(EditAnywhere)
	int Lifes = 3;
	void AddPoints(int Pointz);

	
};
