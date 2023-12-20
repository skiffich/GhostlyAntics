// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class IInteractInterface
{
public:
    virtual void BeginInteract(APawn* InstigatorPawn) = 0;
    virtual void StopInteract(APawn* InstigatorPawn) = 0;
};
