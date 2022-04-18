/*************************************************************************
 *
 *    Used with ICCARM and AARM.
 *
 *    (c) Copyright IAR Systems 2006
 *
 *    File name   : drv_glcd_cnfg.h
 *    Description : Graphic LCD config file
 *
 *    History :
 *    1. Date        : December 2, 2006
 *       Author      : Stanimir Bonev
 *       Description : Create
 *
 *    $Revision: 48478 $ *AB*
 **************************************************************************/

#include "arm_comm.h"

#ifndef  __DRV_GLCD_CNFG_H
#define  __DRV_GLCD_CNFG_H

// GLCD DRV Default setup data         НАСТРОЙКИ ДИСПЛЕЯ
// SPI0 default clock                  Частота тактирования SPI
#define GLCD_DEF_CLOCK	        (1MHZ)
// SPI0 default data width             Разрядность данных SPI
#define GLCD_DEF_WIDTH	        9
// Horizontal Size of display [Pixels] Горизонтальный размер дисплея, пикселов
#define GLCD_HORIZONTAL_SIZE       132
// Vertical Size of display [Pixels]   Вертикальный размер дисплея, пикселов
#define GLCD_VERTICAL_SIZE         132
// Display On/Off 1/0                  Разрешение работы дисплея
#define GLCD_DISPLAY_STATE         1
// Default Display inversion setting   Инвертирование цветов
#define GLCD_DEF_DPL_INV           0
// Default Display color scheme        Цветовая схема
#define GLCD_DEF_DPL_8_COLOR       0
// Default Display Partial mode        Режим дисплея
#define GLCD_DEF_DPL_PARTIAL_MODE  0
// Default Display Row direction       Зеркальное отображение по вертикали
#define GLCD_DEF_DPL_MY            0
// Default Display column direction    Зеркальное отображение по горизонтали
#define GLCD_DEF_DPL_MX            0
// Default Display address direction   Направление адресации
#define GLCD_DEF_DPL_V             0
// Default Display scan direction      Направление сканирования
#define GLCD_DEF_DPL_ML            0
// Default Display RGB order           Порядок следования цветов 
#define GLCD_DEF_DPL_RGB_ORD       0

#endif  /* __DRV_GLCD_CNFG_H */
