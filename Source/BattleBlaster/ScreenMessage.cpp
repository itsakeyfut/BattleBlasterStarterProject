// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenMessage.h"

void UScreenMessage::SetMessageText(FString Message)
{
	if (MessageTextBlock)
	{
		FText MessageText = FText::FromString(Message);
		MessageTextBlock->SetText(MessageText);
	}
}
