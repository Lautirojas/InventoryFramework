// Copyright Epic Games, Inc. All Rights Reserved.

#include "InventoryFramework.h"
#include "InventoryFrameworkLog.h"


#define LOCTEXT_NAMESPACE "FInventoryFrameworkModule"

void FInventoryFrameworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	UE_LOG(LogInventoryFramework, Warning, TEXT("InventoryFramework module has started!"));
}

void FInventoryFrameworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module. 
	UE_LOG(LogInventoryFramework, Warning, TEXT("InventoryFramework module is shutting down"));
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FInventoryFrameworkModule, InventoryFramework)