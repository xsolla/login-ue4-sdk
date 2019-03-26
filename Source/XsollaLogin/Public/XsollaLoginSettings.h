// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#pragma once

#include "XsollaLoginSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class XSOLLALOGIN_API UXsollaLoginSettings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	/** Login ID from Publisher Account. Required. */
	UPROPERTY(Config, EditAnywhere)
	FString ProjectId;

	/**
	 * URL to generate the link with additional parameters and to redirect the user to
	 * after account confirmation. Must be identical to the Callback URL
	 * specified in Publisher Account in Login settings.
	 * Required if there are several Callback URLs.
	 */
	UPROPERTY(Config, EditAnywhere)
	FString LoginURL;
};
