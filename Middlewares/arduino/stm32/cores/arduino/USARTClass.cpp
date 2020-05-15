/*
  Copyright (c) 2014 Makerbase & Andy Sze(andy.sze.mail@gmail.com)  All right reserved.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "USARTClass.h"
#include "Arduino.h"
#include "usart.h"
#include "mks_cfg.h"

// Constructors ////////////////////////////////////////////////////////////////

USARTClass::USARTClass( USART_TypeDef* pUsart, IRQn_Type dwIrq, uint32_t dwId, RingBuffer* pRx_buffer ) {
	_rx_buffer = pRx_buffer ;
	_pUsart=pUsart ;
	_dwIrq=dwIrq ;
	_dwId=dwId ;
}

// Public Methods //////////////////////////////////////////////////////////////
void USARTClass::begin( const uint32_t dwBaudRate ) {
	/* Enable USART Receive interrupts */
	USART_ITConfig(_pUsart, USART_IT_RXNE, ENABLE);
	// Enable UART interrupt in NVIC
	NVIC_EnableIRQ( _dwIrq ) ;
	// Enable the USART
	USART_Cmd(_pUsart, ENABLE);
}

void USARTClass::end(void) { }

int USARTClass::available(void) {
	return (uint32_t)(SERIAL_BUFFER_SIZE + _rx_buffer->_iHead - _rx_buffer->_iTail) % SERIAL_BUFFER_SIZE;
}

int USARTClass::peek(void) {
	if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
		return -1 ;
	return _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
}

short USARTClass::read(void) {
  // if the head isn't ahead of the tail, we don't have any characters
  if ( _rx_buffer->_iHead == _rx_buffer->_iTail )
    return -1 ;

  uint8_t uc = _rx_buffer->_aucBuffer[_rx_buffer->_iTail] ;
  _rx_buffer->_iTail = (unsigned int)(_rx_buffer->_iTail + 1) % SERIAL_BUFFER_SIZE ;
  return uc ;
}

void USARTClass::flush(void) {
	_rx_buffer->_iTail = _rx_buffer->_iHead;
}

size_t USARTClass::write(const uint8_t uc_data) {
	// Send one byte from USART
	USART_SendData(_pUsart, uc_data);
	// Loop until USART DR register is empty
	while(USART_GetFlagStatus(_pUsart, USART_FLAG_TXE) == RESET) { }
	return 1;
} 

void USARTClass::IrqHandler( void ) {
	if (USART_GetFlagStatus(_pUsart, USART_FLAG_PE) != RESET) {
		USART_ReceiveData(_pUsart);
		USART_ClearFlag(_pUsart, USART_FLAG_PE);
	}
	if (USART_GetFlagStatus(_pUsart, USART_FLAG_ORE) != RESET) {
		USART_ReceiveData(_pUsart);
		USART_ClearFlag(_pUsart, USART_FLAG_ORE);
	}
    if (USART_GetFlagStatus(_pUsart, USART_FLAG_FE) != RESET) {
    	USART_ReceiveData(_pUsart);
    	USART_ClearFlag(_pUsart, USART_FLAG_FE);
	}
	// Did we receive data ?
	if(USART_GetITStatus(_pUsart, USART_IT_RXNE) != RESET) {
		/* Read one byte from the receive data register */
		uint8_t RxBuffer;
		USART_ClearFlag(_pUsart, USART_FLAG_RXNE);
		USART_ClearITPendingBit(_pUsart, USART_IT_RXNE);
		RxBuffer = USART_ReceiveData(_pUsart);
		_rx_buffer->store_char( RxBuffer ) ;
	}
}

void USARTClass::inject(char * injection) {
	this->injected = injection;
	this->just_injected = 1;
	this->write("INJECTED: ");
	this->write(this->injected);
	this->write("\n");
}


void USARTClass::loopInjection(void) {
	if (this->injected==0)
		return;
	if (*this->injected == 0) {
		this->injected = 0;
		return;
	}

	unsigned char i = 0;
	if(this->check()) {
		if (this->just_injected) {
			_rx_buffer->store_char('\n');
			this->just_injected = 0;
		} else {
			unsigned char c;
			do{
				 c = *this->injected;
				 if (c==0)
					 break;
				 this->injected++;
				 if (c==';')
					 break;
				_rx_buffer->store_char(c);
			} while (++i<SERIAL_BUFFER_SIZE-1);
			if (i!=0)
				_rx_buffer->store_char('\n');
		}
	}
}

int USARTClass::check( void ) {
	return (_rx_buffer->_iHead==_rx_buffer->_iTail)?1:0;
}


#if 1
extern uint8_t next_cnt;
extern uint8_t leveling_start_flg;

void USARTClass::do_leveling_move_action(uint8_t point) {
	uint8_t i;
	char move_point_temp[50]={0};

	if(USARTClass::check())
	{
		memset(move_point_temp,0,sizeof(move_point_temp));
		sprintf((char*)move_point_temp,"G91\nG1 Z%d\nG90\nG1 X%d Y%d\nG28 Z0\n",gCfgItems.pause_zpos,gCfgItems.leveling_points[point].x,gCfgItems.leveling_points[point].y);
		for(i=0;i<strlen(move_point_temp);i++)
		{
  			_rx_buffer->store_char(move_point_temp[i]) ;
		}
	}

}

void USARTClass::Leveling_move_action(void)
{
	uint8_t i;
	char move_point_temp[50]={0};

    
	if(leveling_start_flg == 1)//??��??a��?2?D����a??��?
	{
		leveling_start_flg = 0;
		if(USARTClass::check())
		{
			memset(move_point_temp,0,sizeof(move_point_temp));
			sprintf((char*)move_point_temp,"G28\n");
			for(i=0;i<strlen(move_point_temp);i++)
			{
      			_rx_buffer->store_char(move_point_temp[i]) ;
			}
		}			
	} 

	if ((next_cnt > 0) && (next_cnt<6))
		USARTClass::do_leveling_move_action(next_cnt - 1);
}
#endif



