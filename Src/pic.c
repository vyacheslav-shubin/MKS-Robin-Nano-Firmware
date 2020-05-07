//#include "draw_ui.h"
#include "gui.h"
#include "pic.h"
#include "ff.h"
#include "mks_cfg.h"



#ifndef NULL
#define NULL    0
#endif

#if 0
unsigned char bmp_public_buf[16 * 1024] = {0};

GUI_BITMAP bmp_struct = {
  117,
  140,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
#if 0
GUI_BITMAP bmp_struct_92 = {
  117,
  92,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct_50 = {
  45,
  45,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct_100 = {
  100,
  100,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct_150 = {
  150,
  80,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
#endif
//FIL TEST_FIL;
#else
unsigned char bmp_public_buf[8 * 1024] = {0};

GUI_BITMAP bmp_struct = {
  117,
  35,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct10x17 = {
  10,
  17,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct70X40 = {
  70,
  40,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct90X30 = {
  90,
  30,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct70X28 = {
  70,
  28,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct90X40 = {
  90,
  40,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct68X40 = {
  68,
  40,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct68X90 = {
  68,
  20,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct144X40 = {
  144,
  10,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct26X26 = {
  26,
  26,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct_92 = {
  117,
  23,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
#if 1
GUI_BITMAP bmp_struct_50 = {
  45,
  45,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct_100 = {
  100,
  25,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};

GUI_BITMAP bmp_struct_150 = {
  150,
  20,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};


GUI_BITMAP bmp_struct_117x25 = {
  50,
  50,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct_50x50 = {
  50,
  50,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
GUI_BITMAP bmp_struct_141x60 = {
  140,
  15,
  160, /* BytesPerLine */
  16, /* BitsPerPixel */
  (unsigned char *)bmp_public_buf,  /* Pointer to picture data */
  NULL  /* Pointer to palette */
 ,GUI_DRAW_BMPM565
};
#endif
FIL TEST_FIL;

#endif
int ascii2dec_test(char *ascii)
{
#if 1
	int result = 0;

	if(ascii == 0)
		return 0;

	if(*(ascii) >= '0' && *(ascii) <= '9')
		result = *(ascii) - '0';
	else if(*(ascii) >= 'a' && *(ascii) <= 'f')
		result = *(ascii) - 'a' + 0x0a;
	else if(*(ascii) >= 'A' && *(ascii) <= 'F')
		result = *(ascii) - 'A' + 0x0a;
	else
		return 0;
		

	return result;
	#endif
}


#if 1
/*****************************************************/
//path:�ļ�·��
//xsize,ysize:��ʾԤ��ͼƬ��С;
//sel:
//		0:�ļ�Ŀ¼Ԥ����
//		1:��ӡ�ļ�Ԥ��
//pic_flg:
//		1:ֻ��ȡͼƬ��־��";simage:";
//		0:��ȡͼƬ���ݡ�
int pre_sread_cnt2;
uint8_t drawicon_preview(char *path,int xsize_small,int ysize_small,uint8_t part)
{
#if 1
	uint16_t *p_index;
	int i=0,j=0,k=0,h=0;
	char re;
	UINT read;
	char temp_test[200];
	int row_1=0;
  uint32_t *ps3,*ps4; 
	int pre_sread_cnt;
	if(part ==0)
	{
		re = f_open(&TEST_FIL,path, FA_OPEN_EXISTING | FA_READ);//huaping.gcode

		if(re == FR_OK)
		{
			memset(bmp_public_buf,0,sizeof(bmp_public_buf));
			//ps3 = (uint32_t *)&bmp_public_buf[0];
			f_read(&TEST_FIL,&bmp_public_buf[0],1024,&read);
			ps4 = (uint32_t *)strstr((uint8_t *)bmp_public_buf,";simage:");
			if(ps4)
			{
					pre_sread_cnt = (uint32_t)ps4-(uint32_t)((uint32_t *)(&bmp_public_buf[0]));
					pre_sread_cnt2=pre_sread_cnt;
					f_lseek(&TEST_FIL,pre_sread_cnt+8);
					
					while(1)
					{
						f_read(&TEST_FIL,&temp_test,200,&read);
						for(i=0;i<200;)
						{
							bmp_public_buf[row_1*200+100*k+j] = (char)(ascii2dec_test(&temp_test[i])<<4|ascii2dec_test(&temp_test[i+1]));
							j++;
							i+=2;
						}
						k++;
						j=0;
						if(k*100>=200)
						{
							for(h=0;h<200;)
							{
								p_index = (uint16_t *)(&bmp_public_buf[row_1*200+h]); 
								if(*p_index == 0x0000)*p_index=gCfgItems.preview_bk_color;
								h+=2;
							}
							k=0;
							row_1++;
							f_read(&TEST_FIL,&temp_test,9,&read);
						}
						if(row_1>=(ysize_small))
						{
							break;
						}
					}		
					//f_close(&TEST_FIL);
					return 1;
			}
		}
		f_close(&TEST_FIL);
		return 0;
	}
	else
	{
		//memset(bmp_public_buf,0,(xsize_small*ysize_small*2));
		f_lseek(&TEST_FIL,pre_sread_cnt2+8+part*(40900>>2));
					
		while(1)
		{
			f_read(&TEST_FIL,&temp_test,200,&read);
			for(i=0;i<200;)
			{
				bmp_public_buf[row_1*200+100*k+j] = (char)(ascii2dec_test(&temp_test[i])<<4|ascii2dec_test(&temp_test[i+1]));
				j++;
				i+=2;
			}
			k++;
			j=0;
			if(k*100>=200)
			{
				for(h=0;h<200;)
				{
					p_index = (uint16_t *)(&bmp_public_buf[row_1*200+h]); 
					if(*p_index == 0x0000)*p_index=gCfgItems.preview_bk_color;
					h+=2;
				}
				k=0;
				row_1++;
				f_read(&TEST_FIL,&temp_test,9,&read);
			}
			if(row_1 >= ysize_small)
			{
				break;
			}
		}		
		if(part == 3)f_close(&TEST_FIL);
		return 1;
	}
#endif
}
#endif


