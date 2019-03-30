// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaLoginController.h"

#include "XsollaLogin.h"
#include "XsollaLoginDefines.h"
#include "XsollaLoginSettings.h"

const FString UXsollaLoginController::RegistrationEndpoint(TEXT("https://login.xsolla.com/api/user"));
const FString UXsollaLoginController::LoginEndpoint(TEXT("https://login.xsolla.com/api/login"));
const FString UXsollaLoginController::ResetPasswordEndpoint(TEXT("https://login.xsolla.com/api/password/reset"));

UXsollaLoginController::UXsollaLoginController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

/**
 * POST https://login.xsolla.com/api/user?projectId={projectId}&login_url={login_url}
 */
void UXsollaLoginController::RegistrateUser(const FString& Username, const FString& Password, const FString& Email)
{
	// Prepare request payload
	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject);
	RequestDataJson->SetStringField(TEXT("username"), Username);
	RequestDataJson->SetStringField(TEXT("password"), Password);
	RequestDataJson->SetStringField(TEXT("email"), Email);
	
	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);
	
	// Generate endpoint url
	const UXsollaLoginSettings* Settings = FXsollaLoginModule::Get().GetSettings();
	const FString Endpoint = (Settings->UserDataStorage == EUserDataStorage::Xsolla) ? RegistrationEndpoint : "";
	const FString Url = FString::Printf(TEXT("%s?projectId=%s&login_url=%s"),
										*Endpoint,
										*Settings->ProjectId,
										*FGenericPlatformHttp::UrlEncode(Settings->CallbackURL));
	
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UXsollaLoginController::Default_HttpRequestComplete);
	
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(PostContent);
	
	HttpRequest->ProcessRequest();
}

void UXsollaLoginController::AuthenticateUser(const FString& Username, const FString& Password, bool bRememberMe)
{
	// Prepare request payload
	TSharedPtr<FJsonObject> RequestDataJson = MakeShareable(new FJsonObject);
	RequestDataJson->SetStringField(TEXT("username"), Username);
	RequestDataJson->SetStringField(TEXT("password"), Password);
	RequestDataJson->SetBoolField(TEXT("remember_me"), false);
	
	FString PostContent;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&PostContent);
	FJsonSerializer::Serialize(RequestDataJson.ToSharedRef(), Writer);
	
	// Generate endpoint url
	const UXsollaLoginSettings* Settings = FXsollaLoginModule::Get().GetSettings();
	const FString Endpoint = (Settings->UserDataStorage == EUserDataStorage::Xsolla) ? LoginEndpoint : "";
	const FString Url = FString::Printf(TEXT("%s?projectId=%s&login_url=%s"),
										*Endpoint,
										*Settings->ProjectId,
										*FGenericPlatformHttp::UrlEncode(Settings->CallbackURL));
	
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
	
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UXsollaLoginController::Default_HttpRequestComplete);
	
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(PostContent);
	
	HttpRequest->ProcessRequest();
}

void UXsollaLoginController::ResetUserPassword(const FString& Username)
{
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: Not implemented yet"), *VA_FUNC_LINE);
}

void UXsollaLoginController::Default_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: Not implemented yet"), *VA_FUNC_LINE);
}
