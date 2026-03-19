
#include "TChatWidget.h"
#include "Player/TPlayerController.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UTChatWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
    {
        EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);		
    }	
}

void UTChatWidget::NativeDestruct()
{
    Super::NativeDestruct();
    
    if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
    {
        EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
    }
}

void UTChatWidget::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter)
    {
        APlayerController* OwningPlayerController = GetOwningPlayer();
        if (IsValid(OwningPlayerController) == true)
        {
            ATPlayerController* OwningTPlayerController = Cast<ATPlayerController>(OwningPlayerController);
            if (IsValid(OwningTPlayerController) == true)
            {
                OwningTPlayerController->SetChatMessageString(Text.ToString());

                EditableTextBox_ChatInput->SetText(FText());
            }
        }
    }
}
