
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SlateEnums.h"
#include "TChatWidget.generated.h"

class UEditableTextBox;
class UScrollBox;
class UTextBlock;

UCLASS()
class TEST1_API UTChatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_ChatInput;

protected: // 위젯에서의 생성자, 소멸자 역할
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
// ctrl k c -> 주석처리 단축키
protected:
	UFUNCTION()
	void OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
};
