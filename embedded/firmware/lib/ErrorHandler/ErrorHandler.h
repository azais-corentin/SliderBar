#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char* file, int line);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

#endif // __ERRORHANDLER_H__