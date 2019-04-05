// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaLoginController.h"

#include "XsollaLogin.h"
#include "XsollaLoginDefines.h"
#include "XsollaLoginSettings.h"

#include "Json.h"

const FString UXsollaLoginController::RegistrationEndpoint(TEXT("https://login.xsolla.com/api/user"));
const FString UXsollaLoginController::LoginEndpoint(TEXT("https://login.xsolla.com/api/login"));
const FString UXsollaLoginController::ResetPasswordEndpoint(TEXT("https://login.xsolla.com/api/password/reset/request"));

UXsollaLoginController::UXsollaLoginController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

/**
 * POST https://login.xsolla.com/api/user?projectId={projectId}&login_url={login_url}
 */
void UXsollaLoginController::RegistrateUser(const FString& Username, const FString& Password, const FString& Email, const FOnRequestSuccess& SuccessCallback, const FOnAuthError& ErrorCallback)
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

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UXsollaLoginController::Default_HttpRequestComplete, SuccessCallback, ErrorCallback);

	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(PostContent);

	HttpRequest->ProcessRequest();
}

/**
 * POST https://login.xsolla.com/api/login?projectId={projectId}&login_url={login_url}
 */
void UXsollaLoginController::AuthenticateUser(const FString& Username, const FString& Password, const FOnAuthUpdate& SuccessCallback, const FOnAuthError& ErrorCallback, bool bRememberMe)
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

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UXsollaLoginController::AuthUpdated_HttpRequestComplete, SuccessCallback, ErrorCallback);

	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(PostContent);

	HttpRequest->ProcessRequest();
}

/**
 * POST https://login.xsolla.com/api/password/reset/request?projectId={projectId}&login_url={login_url}
 */
void UXsollaLoginController::ResetUserPassword(const FString& Username, const FOnRequestSuccess& SuccessCallback, const FOnAuthError& ErrorCallback)
{
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: Not implemented yet"), *VA_FUNC_LINE);
}

void UXsollaLoginController::Default_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnRequestSuccess SuccessCallback, FOnAuthError ErrorCallback)
{
	if (HandleRequestError(HttpRequest, HttpResponse, bSucceeded, ErrorCallback))
	{
		return;
	}

	FString ResponseStr = HttpResponse->GetContentAsString();
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: THE ANSWER IS %s"), *VA_FUNC_LINE, *ResponseStr);

	SuccessCallback.ExecuteIfBound();
}

void UXsollaLoginController::AuthUpdated_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnAuthUpdate SuccessCallback, FOnAuthError ErrorCallback)
{
	if (HandleRequestError(HttpRequest, HttpResponse, bSucceeded, ErrorCallback))
	{
		return;
	}

	FString ResponseStr = HttpResponse->GetContentAsString();
	UE_LOG(LogXsollaLogin, Warning, TEXT("%s: THE ANSWER IS %s"), *VA_FUNC_LINE, *ResponseStr);

	// @TODO Fill auth data and parse token https://github.com/xsolla/login-ue4-sdk/issues/15
	SuccessCallback.ExecuteIfBound(FXsollaLoginData());
}

bool UXsollaLoginController::HandleRequestError(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FOnAuthError ErrorCallback)
{
	FString ErrorStr;
	FString ErrorCode = TEXT("204");

	if (bSucceeded && HttpResponse.IsValid())
	{
		if (!EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			FString ResponseStr = HttpResponse->GetContentAsString();

			ErrorCode = FString::Printf(TEXT("%d"), HttpResponse->GetResponseCode());
			ErrorStr = FString::Printf(TEXT("Invalid response. code=%d error=%s"), HttpResponse->GetResponseCode(), *ResponseStr);

			// Example: {"error":{"code":"003-003","description":"The username is already taken"}}
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(*ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject))
			{
				static const FString ErrorFieldName = TEXT("error");
				if (JsonObject->HasTypedField<EJson::Object>(ErrorFieldName))
				{
					TSharedPtr<FJsonObject> ErrorObject = JsonObject.Get()->GetObjectField(ErrorFieldName);
					ErrorCode = ErrorObject.Get()->GetStringField(TEXT("code"));
					ErrorStr = ErrorObject.Get()->GetStringField(TEXT("description"));
				}
				else
				{
					ErrorStr = FString::Printf(TEXT("Can't deserialize error json: no field '%s' found"), *ErrorFieldName);
				}
			}
			else
			{
				ErrorStr = TEXT("Can't deserialize error json");
			}
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogXsollaLogin, Warning, TEXT("%s: request failed. %s"), *VA_FUNC_LINE, *ErrorStr);
		ErrorCallback.ExecuteIfBound(ErrorCode, ErrorStr);
		return true;
	}

	return false;
}
