// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaLoginController.h"

#include "XsollaLoginDefines.h"
#include "XsollaLoginSettings.h"

#include "Http.h"

UXsollaLoginController::UXsollaLoginController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

/**
 * POST https://login.xsolla.com/api/user?projectId={projectId}&login_url={login_url}
 */
void UXsollaLoginController::RegistrateUser(const FString& Username, const FString& Password, const FString& Email)
{
	const UXsollaLoginSettings* Settings = FXsollaLoginModule::Get().GetSettings();
	
	const FString PostContent = FString::Printf(TEXT("projectId=%s&login_url=%s"),
												*Settings->ProjectId,
												*Settings->CallbackURL);
	FString Url(FString::Printf(TEXT("%s?client_id=%s"), *TokenValidateUrl, *FGenericPlatformHttp::UrlEncode(Subsystem->GetAppId())));

	
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	
	HttpRequest->OnProcessRequestComplete().BindRaw(this, &UXsollaLoginController::ExchangeRequest_HttpRequestComplete, LocalUserNum, InExchangeToken, InCompletionDelegate);
	
	HttpRequest->SetURL(Endpoints.TokenEndpoint);
	
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	HttpRequest->SetVerb(TEXT("POST"));
	
	HttpRequest->SetContentAsString(PostContent);
	HttpRequest->ProcessRequest();
}

void UXsollaLoginController::AuthenticateUser(const FString& Username, const FString& Password, bool bRememberMe)
{
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: Not implemented yet"), *VA_FUNC_LINE);
}

void UXsollaLoginController::ResetUserPassword(const FString& Username)
{
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: Not implemented yet"), *VA_FUNC_LINE);
}
