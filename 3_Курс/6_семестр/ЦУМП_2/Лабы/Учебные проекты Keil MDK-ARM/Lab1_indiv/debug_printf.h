
#ifndef _DEBUG_PRINTF_H
#define _DEBUG_PRINTF_H

////////////////////////////////////////////////////////////////////////////////////////
// —ледующий блок операторов используетс€ при симул€ции дл€ перенаправлени€           //
// вывода функции printf вместо последовательного порта в отладочное окно             //
// "Debug (printf) Viewer". ќкно открываетс€/закрываетс€ в режиме отладки             //
// выбором пункта меню "View / Serial Windows / Debug (printf) Viewer"                //
//                                                                                    //
#ifdef SIMUL                                                                          //
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))               //
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))               //
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))               //
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))                   //
#define TRCENA          0x01000000                                                    //
                                                                                      //
struct __FILE { int handle; };                                                        //
FILE __stdout, __stdin;                                                               //
                                                                                      //
int fputc(int ch, FILE *f)                                                            //
{                                                                                     //
  if (DEMCR & TRCENA)                                                                 //
  { while (ITM_Port32(0) == 0);                                                       //
    ITM_Port8(0) = ch;                                                                //
  }                                                                                   //
  return (ch);                                                                        //
}                                                                                     //
                                                                                      //
#define LCD_printf(...)  printf(__VA_ARGS__)                                          //
#else                                                                                 //
#define LCD_printf(...)  LCD_print(__VA_ARGS__)                                       //
#endif                                                                                //
////////////////////////////////////////////////////////////////////////////////////////

#endif /* _DEBUG_PRINTF_H */
