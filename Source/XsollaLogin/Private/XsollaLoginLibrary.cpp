// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaLoginLibrary.h"

#include "XsollaLogin.h"

UXsollaLoginLibrary::UXsollaLoginLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UXsollaLoginLibrary::RegistrateUser(const FString& Username, const FString& Password, const FString& Email)
{
	
}

void UXsollaLoginLibrary::AuthenticateUser(const FString& Username, const FString& Password, bool bRememberMe)
{
	
}

void UXsollaLoginLibrary::ResetUserPassword(const FString& Username)
{
	
}

UXsollaLoginController* UXsollaLoginLibrary::GetLoginController()
{
	return FXsollaLoginModule::Get().GetLoginController();
}
