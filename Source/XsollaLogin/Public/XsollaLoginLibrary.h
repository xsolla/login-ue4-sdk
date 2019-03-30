// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "XsollaLoginLibrary.generated.h"

UCLASS()
class XSOLLALOGIN_API UXsollaLoginLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()
	
public:
	/**
	 * Adds a new user to the database. The user will receive an account confirmation message to the specified email.
	 *
	 * @param Username Username. Required.
	 * @param Password Password. Required.
	 * @param Email Email. Required.
	 */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login")
	static void RegistrateUser(const FString& Username, const FString& Password, const FString& Email);
	
	/**
	 * Authenticates the user by the username and password specified.
	 *
	 * @param Username Username. Required.
	 * @param Password Password. Required.
	 * @param bRememberMe Whether the user agrees to save the authentication data. Default is 'false'.
	 */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login")
	static void AuthenticateUser(const FString& Username, const FString& Password, bool bRememberMe = false);
	
	/**
	 * Resets the user's password.
	 *
	 * @param Username Username. Required.
	 */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login")
	static void ResetUserPassword(const FString& Username);

public:
	/** Direct access to login controller */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login", BlueprintPure)
	static UXsollaLoginController* GetLoginController();
};
