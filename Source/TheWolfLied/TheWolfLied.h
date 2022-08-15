// Made by Kristjanmn

#pragma once

#include "Engine.h"
#include <chrono>
#include <thread>
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Async.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWorldManager, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogLifeManager, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogScriptManager, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogQuestManager, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWeatherManager, Log, All);