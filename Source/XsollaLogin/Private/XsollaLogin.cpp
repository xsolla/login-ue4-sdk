// Copyright 2019 Xsolla Inc. All Rights Reserved.
// @author Vladimir Alyamkin <ufna@ufna.ru>

#include "XsollaLogin.h"

#include "XsollaLoginController.h"
#include "XsollaLoginDefines.h"
#include "XsollaLoginSettings.h"

#include "Developer/Settings/Public/ISettingsModule.h"
#include "UObject/Package.h"

#define LOCTEXT_NAMESPACE "FXsollaLoginModule"

void FXsollaLoginModule::StartupModule()
{
	XsollaLoginSettings = NewObject<UXsollaLoginSettings>(GetTransientPackage(), "XsollaLoginSettings", RF_Standalone);
	XsollaLoginSettings->AddToRoot();

	// Register settings
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "XsollaLogin",
			LOCTEXT("RuntimeSettingsName", "Xsolla Login SDK"),
			LOCTEXT("RuntimeSettingsDescription", "Configure Xsolla Login SDK"),
			XsollaLoginSettings);
	}

	// Create login data controller
	XsollaLoginController = NewObject<UXsollaLoginController>(GetTransientPackage());
	XsollaLoginController->SetFlags(RF_Standalone);
	XsollaLoginController->AddToRoot();

	UE_LOG(LogXsollaLogin, Log, TEXT("%s: XsollaLogin module started"), *VA_FUNC_LINE);
}

void FXsollaLoginModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "XsollaLogin");
	}

	if (!GExitPurge)
	{
		// If we're in exit purge, this object has already been destroyed
		XsollaLoginSettings->RemoveFromRoot();
		XsollaLoginController->RemoveFromRoot();
	}
	else
	{
		XsollaLoginSettings = nullptr;
		XsollaLoginController = nullptr;
	}
}

UXsollaLoginSettings* FXsollaLoginModule::GetSettings() const
{
	check(XsollaLoginSettings);
	return XsollaLoginSettings;
}

UXsollaLoginController* FXsollaLoginModule::GetLoginController() const
{
	check(XsollaLoginController);
	return XsollaLoginController;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FXsollaLoginModule, XsollaLogin)

DEFINE_LOG_CATEGORY(LogXsollaLogin);
