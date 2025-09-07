#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
	#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#else
	#define CLEAN_FUNCTION_NAME ([]{ \
		FString Full = ANSI_TO_TCHAR(__PRETTY_FUNCTION__); \
		int32 ParenIdx = INDEX_NONE; \
		if (Full.FindChar('(', ParenIdx)) \
			Full = Full.Left(ParenIdx); \
		Full.TrimStartInline(); \
		int32 SpaceIdx = INDEX_NONE; \
		if (Full.FindChar(' ', SpaceIdx)) \
			Full = Full.RightChop(SpaceIdx + 1); \
		return Full; \
	}())

	#define LOG_CALLINFO *CLEAN_FUNCTION_NAME
#endif

#define SK_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s] : %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogSkillMaker, Log, All);