// Made by Kristjanmn

#pragma once

#include "Engine.h"
#include "Settings/VideoSettings.h"
#include "TWLEngine.generated.h"

class GameVariables;

UCLASS()
class THEWOLFLIED_API UTWLEngine : public UGameEngine
{
	GENERATED_BODY()

public:
	virtual void Init(class IEngineLoop* InEngineLoop);
};