// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class IInteractInterface
{
public:
    virtual void BeginInteract() = 0;
    virtual void StopInteract() = 0;
};
