#ifndef __ERRORHANDLER_H__
#define __ERRORHANDLER_H__

// Needed if a .c file includes this
#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char* file, int line);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)

// Needed if a .c file includes this
#ifdef __cplusplus
}
#endif

#endif // __ERRORHANDLER_H__