// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "ObjectMacros.h"
#include "ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef IA_PathAIController_generated_h
#error "PathAIController.generated.h already included, missing '#pragma once' in PathAIController.h"
#endif
#define IA_PathAIController_generated_h

#define IA_Source_IA_PathAIController_h_15_RPC_WRAPPERS
#define IA_Source_IA_PathAIController_h_15_RPC_WRAPPERS_NO_PURE_DECLS
#define IA_Source_IA_PathAIController_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAPathAIController(); \
	friend IA_API class UClass* Z_Construct_UClass_APathAIController(); \
public: \
	DECLARE_CLASS(APathAIController, AAIController, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/IA"), NO_API) \
	DECLARE_SERIALIZER(APathAIController) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define IA_Source_IA_PathAIController_h_15_INCLASS \
private: \
	static void StaticRegisterNativesAPathAIController(); \
	friend IA_API class UClass* Z_Construct_UClass_APathAIController(); \
public: \
	DECLARE_CLASS(APathAIController, AAIController, COMPILED_IN_FLAGS(0), 0, TEXT("/Script/IA"), NO_API) \
	DECLARE_SERIALIZER(APathAIController) \
	enum {IsIntrinsic=COMPILED_IN_INTRINSIC};


#define IA_Source_IA_PathAIController_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APathAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APathAIController) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APathAIController); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APathAIController); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APathAIController(APathAIController&&); \
	NO_API APathAIController(const APathAIController&); \
public:


#define IA_Source_IA_PathAIController_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API APathAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API APathAIController(APathAIController&&); \
	NO_API APathAIController(const APathAIController&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, APathAIController); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(APathAIController); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(APathAIController)


#define IA_Source_IA_PathAIController_h_15_PRIVATE_PROPERTY_OFFSET
#define IA_Source_IA_PathAIController_h_12_PROLOG
#define IA_Source_IA_PathAIController_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	IA_Source_IA_PathAIController_h_15_PRIVATE_PROPERTY_OFFSET \
	IA_Source_IA_PathAIController_h_15_RPC_WRAPPERS \
	IA_Source_IA_PathAIController_h_15_INCLASS \
	IA_Source_IA_PathAIController_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define IA_Source_IA_PathAIController_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	IA_Source_IA_PathAIController_h_15_PRIVATE_PROPERTY_OFFSET \
	IA_Source_IA_PathAIController_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	IA_Source_IA_PathAIController_h_15_INCLASS_NO_PURE_DECLS \
	IA_Source_IA_PathAIController_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID IA_Source_IA_PathAIController_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
