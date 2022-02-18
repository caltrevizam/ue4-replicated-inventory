#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PathToRagnarokPlayerController.generated.h"

UENUM()
enum EInputSource {
	Gamepad,
	MouseAndKeyboard,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputSourceChanged);

UCLASS()
class APathToRagnarokPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	EInputSource InputSource;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintAssignable)
	FInputSourceChanged InputSourceChanged;

	APathToRagnarokPlayerController();

	UFUNCTION(BlueprintPure, Category="Input")
	EInputSource GetInputSource() const;

	virtual void BeginPlay() override;
	
protected:
	virtual void SetupInputComponent() override;

	virtual void OnInputSourceChanged();

private:
	UFUNCTION()
	void DetectKeyInputSource(FKey Key);

	void RaiseInputSourceChanged();
};
