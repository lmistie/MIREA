//---------------------------------------------------------------------------
// 
//  ТЕСТОВЫЙ ПРОЕКТ
//  Работа с последовательным интерфейсом RS-232 (USART)
//  (C) 2016 МИРЭА
//
//  Для проверки работы соединить стандартным кабелем DB-9M - DB-9F
//  соединитель отладочной платы RS232_1 и COM-порт компьютера.
//  На компьютере запустить терминальную программу с настройками порта:
//  9600 бит/с, 8 бит данных, четность: нет, 1 стоп-бит, упр.потоком: нет.
//
//---------------------------------------------------------------------------

#include "stm32_p407.h"                 //Файл конфигурации отладочной платы
#include "lcd.h"                        //Функции для работы с дисплеем
#include "usart.h"                      //Функции для работы с интерфейсом
#include <stdio.h>                      //Стандартные функции C/C++ (здесь - sprintf)

#define FONTSIZE   9                    //Размер шрифта для вывода на дисплей
#define SYMPERSTR  20                   //Число символов в строке дисплея

//---------------------------------------------------------------------------

int main()
{
  uint8_t sym, n = 0, m;
  char str[64];
    
  //Инициализация кнопок и джойстика
  STM_PBInit(BUTTON_RIGHT, BUTTON_MODE_GPIO);   //Позиции джойстика
  STM_PBInit(BUTTON_LEFT, BUTTON_MODE_GPIO);    // - " -
  STM_PBInit(BUTTON_UP, BUTTON_MODE_GPIO);      // - " -
  STM_PBInit(BUTTON_DOWN, BUTTON_MODE_GPIO);    // - " -
  STM_PBInit(BUTTON_SEL, BUTTON_MODE_GPIO);     // - " -

  //Инициализация дисплея
  LCD_Init(); LCD_Clear(1); LCD_FontSize(FONTSIZE);
  LCD_print("   ТЕСТОВЫЙ ПРОЕКТ\r\n     U S A R T\r\n");

  //Инициализация последовательного интерфейса
  USART_Config();

  //Цикл обслуживания
  while (1) 
  {
    //Проверка наличия принятых данных (байта)
    if (USART_RX_Flag())
    {
      //Чтение принятого байта
      sym = USART_RX_Data();
      
      //Если код соответствует отображаемым символам, вывод символа, 
      // иначе - вывод кода в 16-ричном формате
      if (' ' <= sym && sym <= '~') LCD_print("%c", sym), n++;
      else LCD_print("\\0x%02X", sym), n += 5;
      if (n >= SYMPERSTR) LCD_print("\r\n"), n = 0;
    }

    //Проверка нажатия джойстика
    if (STM_PBGetState(BUTTON_LEFT))
    { m = sprintf(str, "Left ");                //Вывод сообщения в буфер
      USART_TX_Data(str, m);                    //Передача буфера по UART
      while (STM_PBGetState(BUTTON_LEFT));      //Ожидание отпускания
    }
    if (STM_PBGetState(BUTTON_RIGHT))
    { m = sprintf(str, "Right ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_RIGHT));
    }
    if (STM_PBGetState(BUTTON_UP))
    { m = sprintf(str, "Up ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_UP));
    }
    if (STM_PBGetState(BUTTON_DOWN))
    { m = sprintf(str, "Down ");
      USART_TX_Data(str, m);
      while (STM_PBGetState(BUTTON_DOWN));
    }
    
  }

}

//---------------------------------------------------------------------------
