// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "XsollaLoginTypes.generated.h"

USTRUCT(BlueprintType)
struct FXsollaAuthToken
{
	GENERATED_BODY()

public:
	/** A JWT signed by the secret key is generated for each successfully authenticated user. */
	UPROPERTY(BlueprintReadWrite)
	FString AccessToken;

	FXsollaAuthToken() {}
};

USTRUCT(BlueprintType)
struct FXsollaLoginData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FXsollaAuthToken AuthToken;
};