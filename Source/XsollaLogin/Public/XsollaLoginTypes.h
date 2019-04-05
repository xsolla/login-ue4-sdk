// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "XsollaLoginTypes.generated.h"

USTRUCT(BlueprintType)
struct FXsollaAuthToken
{
	GENERATED_BODY()

	/** A JWT signed by the secret key is generated for each successfully authenticated user. */
	UPROPERTY(BlueprintReadOnly)
	FString JWT;
	
	/** Token verification status. */
	UPROPERTY(BlueprintReadOnly)
	bool bIsVerified;

	FXsollaAuthToken()
		: bIsVerified(false)
	{}
};

USTRUCT(BlueprintType)
struct FXsollaLoginData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FXsollaAuthToken AuthToken;
	
	UPROPERTY(BlueprintReadOnly)
	FString Username;
	
	UPROPERTY(BlueprintReadOnly)
	bool bRememberMe;
	
	FXsollaLoginData()
		: bRememberMe(false)
	{}
};
