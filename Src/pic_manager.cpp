#include "type_define.h"
#include "string.h"
#include "pic_manager.h"
#include "GUI.h"
#include "spi_flash.h"
#include "mks_cfg.h"
#include "draw_ui.h"
extern uint8_t DMA_ERRO_FLAG;


uint32_t pic_addr_offset;
uint16_t pic_row;
uint8_t little_image_flag=0;
void Pic_Read(uint8_t *Pname,uint8_t *P_Rbuff,uint8_t part,uint8_t allcnt)
{
	uint8_t i, j;
	uint8_t Pic_cnt;
	uint32_t tmp_cnt = 0;
	PIC_MSG PIC;
	uint16_t *tmp_16bit_addr;
	uint8_t *tmp_8bit_addr;
    
    if(part==0) {
    	for(i=0;i<gCfgItems.total_pic;i++) {
    		j = 0;
    		do {
    			SPI_FLASH_BufferRead(&PIC.name[j],PIC_NAME_ADDR + tmp_cnt,1);
    			tmp_cnt++;
    		} while(PIC.name[j++] != '\0');

    		if((strcmp((char*)Pname,(char*)PIC.name))==0) {
        		SPI_FLASH_BufferRead(PIC.size.bytes,PIC_SIZE_ADDR+i*4,4);
				if(PIC.size.dwords < sizeof(bmp_public_buf))
					little_image_flag=1;
                pic_addr_offset=PIC.size.dwords>>2;
    		    pic_row=i;
    		    break;
    		}
        }
    }
	if(little_image_flag == 1) {
		SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,PIC_DATA_ADDR+pic_row*PER_PIC_MAX_SPACE,PIC.size.dwords);
		if(DMA_ERRO_FLAG) {
			DMA_ERRO_FLAG = 0;
			SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,PIC_DATA_ADDR+pic_row*PER_PIC_MAX_SPACE,PIC.size.dwords);
		}
	} else {
		SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,PIC_DATA_ADDR+pic_row*PER_PIC_MAX_SPACE+part*pic_addr_offset,pic_addr_offset);
		if(DMA_ERRO_FLAG) {
			DMA_ERRO_FLAG = 0;
			SPI_FLASH_BufferRead((uint8_t *)P_Rbuff,PIC_DATA_ADDR+pic_row*PER_PIC_MAX_SPACE+part*pic_addr_offset,pic_addr_offset);
		}
	}
}
