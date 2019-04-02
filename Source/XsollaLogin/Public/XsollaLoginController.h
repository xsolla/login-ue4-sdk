// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "Http.h"

#include "XsollaLoginController.generated.h"

DECLARE_DELEGATE(FOnAuthCompleted);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnAuthError, const FString&, Code, const FString&, Description);
DECLARE_DELEGATE_OneParam(FOnRegisteredNewUser, const FString& /** @TODO AuthData should be here */);

UCLASS()
class XSOLLALOGIN_API UXsollaLoginController : public UObject
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
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login", meta = (AutoCreateRefTerm = "ErrorCallback"))
	void RegistrateUser(const FString& Username, const FString& Password, const FString& Email, const FOnAuthError& ErrorCallback);

	/**
	 * Authenticates the user by the username and password specified.
	 *
	 * @param Username Username. Required.
	 * @param Password Password. Required.
	 * @param bRememberMe Whether the user agrees to save the authentication data. Default is 'false'.
	 */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login", meta = (AutoCreateRefTerm = "ErrorCallback"))
	void AuthenticateUser(const FString& Username, const FString& Password, const FOnAuthError& ErrorCallback, bool bRememberMe = false);

	/**
	 * Resets the user's password.
	 *
	 * @param Username Username. Required.
	 */
	UFUNCTION(BlueprintCallable, Category = "Xsolla|Login", meta = (AutoCreateRefTerm = "ErrorCallback"))
	void ResetUserPassword(const FString& Username, const FOnAuthError& ErrorCallback);

protected:
	void Default_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnAuthError ErrorCallback);

protected:
	static const FString RegistrationEndpoint;
	static const FString LoginEndpoint;
	static const FString ResetPasswordEndpoint;
};
