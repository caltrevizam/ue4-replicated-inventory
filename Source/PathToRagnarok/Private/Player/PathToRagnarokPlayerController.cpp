#include "Player/PathToRagnarokPlayerController.h"
#include "Framework/Commands/InputChord.h"
#include "InputCoreTypes.h"

APathToRagnarokPlayerController::APathToRagnarokPlayerController()
{
}

EInputSource APathToRagnarokPlayerController::GetInputSource() const
{
	return InputSource;
}

void APathToRagnarokPlayerController::BeginPlay()
{
	InputSource = EInputSource::Gamepad;
	RaiseInputSourceChanged();
}

void APathToRagnarokPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	check(InputComponent);

	// thanks to: https://forums.unrealengine.com/t/get-key-pressed/399560/2
	const FInputChord KeyChord(EKeys::AnyKey);

	FInputKeyBinding KB = FInputKeyBinding(KeyChord, EInputEvent::IE_Pressed);
	KB.KeyDelegate.BindDelegate(this, &APathToRagnarokPlayerController::DetectKeyInputSource);
	KB.bConsumeInput = false;
	InputComponent->KeyBindings.Emplace(MoveTemp(KB));

	KB = FInputKeyBinding(KeyChord, EInputEvent::IE_Released);
	KB.KeyDelegate.BindDelegate(this, &APathToRagnarokPlayerController::DetectKeyInputSource);
	KB.bConsumeInput = false;
	InputComponent->KeyBindings.Emplace(MoveTemp(KB));
}

void APathToRagnarokPlayerController::DetectKeyInputSource(FKey Key)
{
	EInputSource Detected = Key.IsGamepadKey() ? EInputSource::Gamepad : EInputSource::MouseAndKeyboard;
	if (Detected != InputSource)
	{
		InputSource = Detected;
		RaiseInputSourceChanged();
	}
}

void APathToRagnarokPlayerController::RaiseInputSourceChanged()
{
	OnInputSourceChanged();
	InputSourceChanged.Broadcast();
}

void APathToRagnarokPlayerController::OnInputSourceChanged()
{
	if (InputSource == EInputSource::MouseAndKeyboard)
		bShowMouseCursor = true;
	else
		bShowMouseCursor = false;
}
