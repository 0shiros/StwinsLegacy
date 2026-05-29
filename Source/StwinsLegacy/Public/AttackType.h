// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	Basic UMETA(DisplayName = "Basic"),
	Heavy UMETA(DisplayName = "Heavy"),
	Special UMETA(DisplayName = "Special"),
	
	Max UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAttackType, EAttackType::Max);
