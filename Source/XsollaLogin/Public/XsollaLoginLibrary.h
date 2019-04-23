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
	/** Direct access to login controller */
	UFUNCTION(BlueprintPure, Category = "Xsolla|Login", meta = (WorldContext = "WorldContextObject"))
	static UXsollaLoginController* GetLoginController(UObject* WorldContextObject);
};
