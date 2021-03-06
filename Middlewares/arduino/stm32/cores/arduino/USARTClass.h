/*
  Copyright (c) 2014 MakerLab.me & Andy Sze(andy.sze.mail@gmail.com)  All right reserved.
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _USART_CLASS_
#define _USART_CLASS_

#include "HardwareSerial.h"
#include "RingBuffer.h"

#include "stm32f10x_usart.h"
// Includes Atmel CMSIS
//#include <chip.h>
#if defined (STM32F10X_MD) || defined (STM32F10X_HD)
//#include <stm32f10x_conf.h>
#endif

#ifdef STM32F40_41xxx
#include <stm32f4xx_conf.h>
#endif

#define id_serial  0
#define id_serial1 1
#define id_serial2 2
#define id_serial3 3
#define id_serial4 4
#define id_serial5 5
#define id_serial6 6


class USARTClass : public HardwareSerial {
	protected:
    	RingBuffer *_rx_buffer ;
    	char just_injected = 0;
    	char * injected = 0;
	protected:
    	USART_TypeDef* _pUsart ;
    	USART_InitTypeDef USART_InitStructure ;
    	IRQn_Type _dwIrq ;
    	uint32_t _dwId ;
    	void do_leveling_move_action(uint8_t point);
	public:
    	USARTClass(USART_TypeDef* pUsart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer* pRx_buffer);
    	void begin(const uint32_t dwBaudRate);
    	void end(void) ;
    	int available(void);
    	int peek(void);
    	int16_t read(void);
    	void flush(void);
    	using HardwareSerial:: write;
    	size_t write(const uint8_t c);
    	void IrqHandler( void );
    	void inject(char * injection);
    	void sdPause(void);
    	void sdContinue(void);
    	int check(void);
		void loopInjection(void);
		void Leveling_move_action(void);

    
#if defined __GNUC__ /* GCC CS3 */
    using Print::write ; // pull in write(str) and write(buf, size) from Print
#elif defined __ICCARM__ /* IAR Ewarm 5.41+ */
#endif

		operator bool() {return true;}; // USART always active
};

#endif // _USART_CLASS_
