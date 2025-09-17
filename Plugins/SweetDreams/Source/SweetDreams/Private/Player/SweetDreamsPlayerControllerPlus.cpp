

#include "Player/SweetDreamsPlayerControllerPlus.h"
#include "Player/SweetDreamsHUD.h"

ASweetDreamsPlayerControllerPlus::ASweetDreamsPlayerControllerPlus() {}

ASweetDreamsHUD* ASweetDreamsPlayerControllerPlus::GetSweetDreamsHUD() const
{
	return Cast<ASweetDreamsHUD>(GetHUD());
}


