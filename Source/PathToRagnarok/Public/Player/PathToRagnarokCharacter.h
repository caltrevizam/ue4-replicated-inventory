#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PathToRagnarokCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;

UCLASS()
class APathToRagnarokCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* CameraComponent;

public:
	APathToRagnarokCharacter();

	UFUNCTION(BlueprintPure, Category = "Character")
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	UFUNCTION(BlueprintPure, Category = "Character")
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

private:
	void MoveForward(float Value);

	void MoveRight(float Value);
};

