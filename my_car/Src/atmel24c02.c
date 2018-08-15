
/* atm 24c02 ¥Ê¥¢≤‚ ‘
 //  for(i=0; i<256; i++)
 //      WriteBuffer[i]=i;    /* WriteBuffer init */
 //  for (i=0; i<32; i++)
 //         {
 //                 if(HAL_I2C_Mem_Write(&hi2c2, ADDR_24LCxx_Write, 0+8*i, I2C_MEMADD_SIZE_8BIT,WriteBuffer + 8*i,8, 0xff) == HAL_OK)
 //                 {
 //                    printf("\r\n EEPROM 24C02 Write Test OK \r\n");
 //                    HAL_Delay(5);
 //                 }
 //                 else
 //                 {
 //                    HAL_Delay(5);
 //                    printf("\r\n EEPROM 24C02 Write Test False \r\n");
 //                 }
 //
 //         }
 // /* read date from EEPROM */
 //  HAL_I2C_Mem_Read(&hi2c2, ADDR_24LCxx_Read, 0, I2C_MEMADD_SIZE_8BIT,ReadBuffer,BufferSize, 0xff);
 //  for(i=0; i<256; i++)
 //      printf("0x%02X  ",ReadBuffer[i]);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
