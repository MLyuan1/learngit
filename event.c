#define event_GLOBALS
#include <includes.h>

//uint8 code str_text[]="O(00,05,1)E O(00,06,2)E O(00,19,3)E O(00,20,4)E ";
//O(00,17,1)E O(00,18,2)E O(00,07,3)E O(00,08,4)E 
//uint8 code str_text[]="O(00,09,1)E O(00,010,2)E O(00,11,3)E O(00,12,4)E O(00,17,1)E O(00,18,2)E O(00,19,3)E O(00,20,4)E";	
//uint8 code str_text[]="O(00,01,2)E O(00,02,3)E ";	
//code uint8 str_text[]="testEO(00,01,0)EO(00,02,0)EO(00,03,0)EO(00,04,0)EO(00,05,0)EO(00,06,0)EO(00,07,0)EO(00,08,0)EO(00,09,0)EO(00,10,0)EO(00,11,0)EO(00,12,0)EO(00,13,0)EO(00,14,0)EO(00,15,0)EO(00,16,0)EO(00,17,0)EO(00,18,0)EO(00,19,0)EO(00,20,0)EO(00,21,0)EO(00,22,0)E";

#define KEY_SEL 10//铃声选择
#define KEY_VOL 12//铃声音量
#define KEY_DUIMA 14//对码
/*-----------------------------------------------------------------*/
/* 按键处理                */
/*-----------------------------------------------------------------*/
uint8 KeyGet(void)
{//return VOID_KEY;

//	uint16 sum;
//	uint8 i;
//	ENTER_CRITICAL();
//	sum=ad_sum;
//	EXIT_CRITICAL();
//	i=sum/AD_SUM_+8;
//
//	return i/16;

	return (GetADC_8(5)+8)/16;
}

void Key_Down_short(uint8 Code)	//短按
{Code++;

}

void Key_Down_long(uint8 Code)	//长按
{Code++;

}
void Key_Down_rpt_first(uint8 Code)	//首次连发
{//Code++;
	if(Code==8)//监视
	{
		to_burn();
	}
	else if(Code==6)
	{
		if(WorkState==WORK_STANDBY)
		{
			if(nSet_==0)
			{//监视1#mkj
				
				onpow();
				if((gMKJ_n==0))
				{
					ASK_Tx(ASK_CONV(ASK_JIANSHI_1) );
				}
				mkj_sw(1);//
				to_monitor();
				Set_init_Tx();
				QD5228_init();
				Set_ctrl(1);
			}
		}
	
	}
	

}
void Key_Down_rpt(uint8 Code)	//连发
{Code++;
}

void Key_Up_long(uint8 Code)	//长按抬起
{Code++;
}



void Key_Up_short(uint8 Code)	//短按抬起
{//Code++;

	switch (Code)
	{
		case 1://
		{

		}break;
		case 2://锁1/摄像头/上键
		{
			if(nSet_) 
			{
				if(nSet_==255)
				{
					IR_Tx(0x0f,0xf2);//查看照片（下一张）
					nSet_time=60;
				}
				else
				{
					Set_ctrl(nSet_+1);
				}
				return;
			}
			if(WorkState==WORK_TAIK_)
			{
//				if(gMKJ_n) unlock_start(1,11);//
				if(gMKJ_n) //
					unlock_start(2,50);
			}
		}break;

		case 3://
		{

		}break;

		case 4://锁2/下键
		{//对讲时，开锁作用。开关3#mkj
			if(nSet_) 
			{
				if(nSet_==255)
				{
					IR_Tx(0x0f,0xf1);//查看照片（上一张）
					nSet_time=60;
				}
				else
				{
					Set_ctrl(nSet_-1);
				}
				return;
			}

//				if( (WorkState==WORK_STANDBY) || (WorkState==WORK_HHT_call) )
//				{//户户通呼叫
//					if( (tm52_read(ADDR_DATA-1+8)) == 0xf1 ) return;
//					mkj_sw(0);
//					to_hht_call();//to_wait();//
//					ASK_Tx(ASK_CONV(ASK_HHT));
//				}
////			if(WorkState==WORK_STANDBY)
////			{
////				if(gV_n==2 ) V_S_(3);
////				else V_S_(2);
////				V_on();nVideo_time=60;//
////			}
//			else if(WorkState==WORK_TAIK_)
//			{
////				if(gMKJ_n) //
////					unlock_start(2,50);
//			}
		}break;

		case 5://
		{

		}break;
		case 6://菜单/拍照/回看
		{
//			if(WorkState==WORK_STANDBY)
//			{//翻看相片
//				if(nSet_==0xff) 
//					{Set_ctrl(0);}
//				else if(nSet_==0) 
//				{ 
//					onpow_d();
//					Set_ctrl(0xff);//查看照片（下一张）
//				}
//			}
//			else
//			{//拍照
//				if(nSet_==0) 
//					IR_Tx(0x0f,0xf0);//拍照
//			}

				if(nSet_==0)
				{//监视1#mkj
					
					if(WorkState==WORK_STANDBY)
					{
						onpow();
						if((gMKJ_n==0))
						{
							ASK_Tx(ASK_CONV(ASK_JIANSHI_1) );
						}
						mkj_sw(1);//
						to_monitor();
						Set_init_Tx();
						QD5228_init();
						Set_ctrl(1);					
	
					}

				}
				else
				{
					Set_ctrl(0);
				}

		}break;
		case 7://
		{

		}break;
		case 8://监视/左键
		{
			if(nSet_) 
			{
				Set_inc();//Set_dec();//
			}
			else
    		{
    			if(WorkState==WORK_STANDBY)
    			{//监视1#mkj
					onpow();
					if((gMKJ_n==0))
					{
						ASK_Tx(ASK_CONV(ASK_JIANSHI_1) );
					}
					mkj_sw(1);//
					to_monitor();
    			}
    			else if( (WorkState==WORK_WAIT_) || (WorkState==WORK_HHT_call) || (WorkState==WORK_TAIK_) )
    			{//
	    			    mkj_sw(0);//
	    			    to_idle();
	    			    ASK_Tx(ASK_CONV(ASK_CLOSE));
    			}
				else if(WorkState==WORK_MONITOR_)
    			{
					if( (tm52_read(ADDR_DATA-1+7)) == 0xf1 ) 
					{
	    			    mkj_sw(0);//
	    			    to_idle();
	    			    ASK_Tx(ASK_CONV(ASK_CLOSE));
					}
					else
					{
						onpow();
						if(gMKJ_n==1)
						{
							ASK_Tx(ASK_CONV(ASK_JIANSHI_2) );
							mkj_sw(2);//
							to_monitor();						
						}
						else
						{
		    			    mkj_sw(0);//
		    			    to_idle();
		    			    ASK_Tx(ASK_CONV(ASK_CLOSE));
						}
					}
	   			}
    		}
		}break;
		case 10://
		{

		}break;
		case 12://
		{//

		}break;
		case 14://接听/hht/右键
		{//
			if(nSet_) 
			{
				Set_dec();//Set_inc();
			}
			else
			{
				if( (WorkState==WORK_MONITOR_) || (WorkState==WORK_WAIT_) )
				{
					//key_to_talk();
					ASK_Tx(ASK_CONV(ASK_HAND));
					if(gMKJ_n==0)
						to_hht_talk();
					else
						to_talk();
 

				}
				else if(WorkState==WORK_TAIK_)
				{
    			    mkj_sw(0);//
    			    to_idle();
    			    ASK_Tx(ASK_CONV(ASK_CLOSE) );
				}
				else if( (WorkState==WORK_STANDBY) || (WorkState==WORK_HHT_call) )
				{//户户通呼叫
					if( (tm52_read(ADDR_DATA-1+8)) == 0xf1 ) return;
					mkj_sw(0);
					to_hht_call();//to_wait();//
					ASK_Tx(ASK_CONV(ASK_HHT));
				}
			}
		}break;
		default:
		{
		}break;
	
	}



}




/*-----------------------------------------------------------------*/
/* 每秒执行10次,在T0中断服务函数中调用.                */
/*-----------------------------------------------------------------*/
void Tick_Handler(void)
{//
led=!led;
	ask_time_proc();
	
	//ask_time_proc();
}

/*-----------------------------------------------------------------*/
/* 每秒执行TICKS_PER_SEC次,在SystemTick()中调用.                */
/*-----------------------------------------------------------------*/
void SystemTickExec(void)		//每秒TICKS_PER_SEC次
{//led=!led;
	call_chk();

}


/*-----------------------------------------------------------------*/
/* 每秒执行10次,在SystemTick()中调用.                */
/*-----------------------------------------------------------------*/
void UserProgram(void)			//每秒10次
{//led=!led;
	static uint8 Sec_cnt=10;
	static uint8 Sec_cnt1=25;
		
	unlock_Timeout_proc();

//	hand_chk();

	if(--Sec_cnt) return;
	Sec_cnt=10;
	close_Timeout_proc();
	ring_Timeout_proc();	
	Set_Timeout_proc();
//	Uart0_putbytes(gTemp,3);


	if(--Sec_cnt1) return;
	Sec_cnt=25;
	mkj_sw(1);//
	to_monitor();

}













