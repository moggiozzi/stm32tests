#include "stdio.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_flash.h"
#include "stm32f7xx_hal_flash_ex.h"

uint32_t sector_size[]={
16,16,16,16,64,128,128,128
};

HAL_StatusTypeDef setWriteProtect(void){
	HAL_StatusTypeDef res = HAL_OK;
	FLASH_OBProgramInitTypeDef ob;
	FLASH_OBProgramInitTypeDef *pOBInit = &ob;
	HAL_FLASHEx_OBGetConfig(pOBInit);
	pOBInit->OptionType = OPTIONBYTE_WRP;
	pOBInit->WRPState = OB_WRPSTATE_ENABLE;
	pOBInit->WRPSector = OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1;
	res |= HAL_FLASH_Unlock();
	res |= HAL_FLASH_OB_Unlock();
	res |= HAL_FLASHEx_OBProgram(pOBInit);
	res |= HAL_FLASH_OB_Launch();
	res |= HAL_FLASH_OB_Lock();
	return res;
}

HAL_StatusTypeDef setReadProtectL1(void){
	HAL_StatusTypeDef res = HAL_OK;
	FLASH_OBProgramInitTypeDef ob;
	FLASH_OBProgramInitTypeDef *pOBInit = &ob;
	HAL_FLASHEx_OBGetConfig(pOBInit);
	pOBInit->OptionType = OPTIONBYTE_RDP;
	pOBInit->RDPLevel = OB_RDP_LEVEL_1;
	res |= HAL_FLASH_Unlock();
	res |= HAL_FLASH_OB_Unlock();
	res |= HAL_FLASHEx_OBProgram(pOBInit);
	res |= HAL_FLASH_OB_Launch();
	res |= HAL_FLASH_OB_Lock();
	return res;
}

// !!!После выполнения этого кода не удается подключиться к плате!!!
//Долго тыкался, вот так вылечилось:
//1) Target -> Settings: Mode HotPlug
//2) Option Bytes: Level 1, PCROP_RDP checked
//3) Option Bytes: Level 0
//-нельзя применять вместе с writeProtection
//-нельзя применять к сектору с вектором прерываний
HAL_StatusTypeDef setReadProtect(void){
	HAL_StatusTypeDef res = HAL_OK;
	FLASH_OBProgramInitTypeDef ob;
	FLASH_OBProgramInitTypeDef *pOBInit = &ob;
	HAL_FLASHEx_OBGetConfig(pOBInit);
  pOBInit->OptionType = OPTIONBYTE_PCROP;
  pOBInit->PCROPSector = OB_PCROP_SECTOR_0 | OB_PCROP_SECTOR_1; //-нельзя применять к сектору с вектором прерываний
	res |= HAL_FLASH_Unlock();
	res |= HAL_FLASH_OB_Unlock();
	res |= HAL_FLASHEx_OBProgram(pOBInit);
	res |= HAL_FLASH_OB_Launch();
	res |= HAL_FLASH_OB_Lock();
	return res;
}

HAL_StatusTypeDef setReadProtect2(void){
	HAL_StatusTypeDef res = HAL_OK;
	FLASH_OBProgramInitTypeDef ob;
	FLASH_OBProgramInitTypeDef *pOBInit = &ob;
	HAL_FLASHEx_OBGetConfig(pOBInit);
    pOBInit->OptionType = OPTIONBYTE_PCROP;
    pOBInit->PCROPSector = OB_PCROP_SECTOR_1;
    pOBInit->OptionType = OPTIONBYTE_PCROP_RDP;
    pOBInit->PCROPRdp = OB_PCROP_RDP_ENABLE;
	res |= HAL_FLASH_Unlock();
	res |= HAL_FLASH_OB_Unlock();
	res |= HAL_FLASHEx_OBProgram(pOBInit);
	res |= HAL_FLASH_OB_Launch();
	res |= HAL_FLASH_OB_Lock();
	return res;
}

HAL_StatusTypeDef setProtect(void){
  HAL_StatusTypeDef res = HAL_OK;
  FLASH_OBProgramInitTypeDef ob;
  FLASH_OBProgramInitTypeDef *pOBInit = &ob;
  memset(pOBInit, 0, sizeof(*pOBInit));
  //HAL_FLASHEx_OBGetConfig(pOBInit);
  pOBInit->OptionType = OPTIONBYTE_RDP | OPTIONBYTE_WRP;
  pOBInit->RDPLevel = OB_RDP_LEVEL_1;
  pOBInit->WRPState = OB_WRPSTATE_ENABLE;
  pOBInit->WRPSector = OB_WRP_SECTOR_0 | OB_WRP_SECTOR_1;
  res |= HAL_FLASH_Unlock();
  res |= HAL_FLASH_OB_Unlock();
  res |= HAL_FLASHEx_OBProgram(pOBInit);
  res |= HAL_FLASH_OB_Launch();
  res |= HAL_FLASH_OB_Lock();
  return res;
}

void test(void){
  HAL_StatusTypeDef res = setProtect();

  printf("Hello %d\r\n", res);
}
