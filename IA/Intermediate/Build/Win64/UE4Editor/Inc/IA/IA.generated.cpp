// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "GeneratedCppIncludes.h"
#include "IA.generated.dep.h"
PRAGMA_DISABLE_OPTIMIZATION
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCode1IA() {}
#if USE_COMPILED_IN_NATIVES
// Cross Module References
	AIMODULE_API class UClass* Z_Construct_UClass_AAIController();

	IA_API class UClass* Z_Construct_UClass_APathAIController_NoRegister();
	IA_API class UClass* Z_Construct_UClass_APathAIController();
	IA_API class UPackage* Z_Construct_UPackage__Script_IA();
	void APathAIController::StaticRegisterNativesAPathAIController()
	{
	}
	UClass* Z_Construct_UClass_APathAIController_NoRegister()
	{
		return APathAIController::StaticClass();
	}
	UClass* Z_Construct_UClass_APathAIController()
	{
		static UClass* OuterClass = NULL;
		if (!OuterClass)
		{
			Z_Construct_UClass_AAIController();
			Z_Construct_UPackage__Script_IA();
			OuterClass = APathAIController::StaticClass();
			if (!(OuterClass->ClassFlags & CLASS_Constructed))
			{
				UObjectForceRegistration(OuterClass);
				OuterClass->ClassFlags |= 0x20900280;


				static TCppClassTypeInfo<TCppClassTypeTraits<APathAIController> > StaticCppClassTypeInfo;
				OuterClass->SetCppTypeInfo(&StaticCppClassTypeInfo);
				OuterClass->StaticLink();
#if WITH_METADATA
				UMetaData* MetaData = OuterClass->GetOutermost()->GetMetaData();
				MetaData->SetValue(OuterClass, TEXT("HideCategories"), TEXT("Collision Rendering Utilities|Transformation"));
				MetaData->SetValue(OuterClass, TEXT("IncludePath"), TEXT("PathAIController.h"));
				MetaData->SetValue(OuterClass, TEXT("ModuleRelativePath"), TEXT("PathAIController.h"));
#endif
			}
		}
		check(OuterClass->GetClass());
		return OuterClass;
	}
	IMPLEMENT_CLASS(APathAIController, 959775406);
	static FCompiledInDefer Z_CompiledInDefer_UClass_APathAIController(Z_Construct_UClass_APathAIController, &APathAIController::StaticClass, TEXT("/Script/IA"), TEXT("APathAIController"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(APathAIController);
	UPackage* Z_Construct_UPackage__Script_IA()
	{
		static UPackage* ReturnPackage = nullptr;
		if (!ReturnPackage)
		{
			ReturnPackage = CastChecked<UPackage>(StaticFindObjectFast(UPackage::StaticClass(), nullptr, FName(TEXT("/Script/IA")), false, false));
			ReturnPackage->SetPackageFlags(PKG_CompiledIn | 0x00000000);
			FGuid Guid;
			Guid.A = 0x8F9066D2;
			Guid.B = 0x9E0196C0;
			Guid.C = 0x00000000;
			Guid.D = 0x00000000;
			ReturnPackage->SetGuid(Guid);

		}
		return ReturnPackage;
	}
#endif
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
PRAGMA_ENABLE_OPTIMIZATION
