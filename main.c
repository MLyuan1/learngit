//20160225 用户卡数量由5张增加到50张


#include <includes.h>

//idata uint8 gTemp[24];



uint8 nM1_cnt=0;
uint8 nM2_cnt=0;




bit bMKJ_type=0;//1为旧协议门口机，只能1t1


bit bPowerUp=0;


void Key_Down_short(uint8 Code)	//短按
{Code++;
}
void Key_Down_long(uint8 Code)	//长按
{Code++;
	if(!bPowerUp) TEST_ON();
}

void Key_Up_short(uint8 Code)	//短按抬起
{//Code++;
	bPowerUp=1;TEST_OFF();
	switch (Code)
	{
		case 2://卡设置
		{
			bb(6);//BBT(600);//
			CardStateChange(128);
		}break;
		case 4://呼叫1键
		{
			//if(snj_n()==1)
			{//先发ask，再切换
				snj_sel(1);
				ASK_Tx(ASK_CONV(ASK_CLOSE));
				delay_mS(30);
			}
			snj_sel(0);

			if(MKJ_n==1) 
				ASK_Tx(ASK_CONV(ASK_HUJIAO_1));		//yuan  按呼叫键 PD7
			else if(MKJ_n==2) 
				ASK_Tx(ASK_CONV(ASK_HUJIAO_2));
	
			WorkState=WORK_WAIT_;
			mkj_to_call_wait();
		}break;
		case 6://呼叫2键
		{
			//MANCHESTER_OFF();
			//if(snj_n()==0)
			{//先发ask，再切换
				snj_sel(0);
				ASK_Tx(ASK_CONV(ASK_CLOSE));
				delay_mS(30);
			}			
			snj_sel(1);
			if(MKJ_n==1) 
				ASK_Tx(ASK_CONV(ASK_HUJIAO_1));		//yuan  按呼叫键 PD7
			else if(MKJ_n==2) 
				ASK_Tx(ASK_CONV(ASK_HUJIAO_2));
	
			WorkState=WORK_WAIT_;
			mkj_to_call_wait();
		}break;
		default:
		{
		}break;
	}
}



#if (_CONFIG_ID_CARD_==1)


void ID_CARD_proc(void)
{
	uint8 err;
	uint8 addr;
	uint8 sta;
	if(ID_CARD_get(gTemp)==0) return;

	err = get_exist_card(gTemp, &addr);

sta=gTemp[3];

	if((addr<3)&&(err==0)&&(nCardState<128))//系统卡//
	{
		if(sta!=1) return;
		if(0==addr)
		{
			_card_del_all_();//card_del_all(3,0);
			CardStateChange(0);
		}
		else
		{
			CardStateChange(addr+1);
			bb(4);
		}
		return;
	}
	CardStateChange(nCardState);//延时10秒




	switch (nCardState)
	{
		case 0://
		{
			if(err==0)
			{
				if(sta==2)////0未刷卡 1刷卡瞬间 2刷卡3秒 3刷卡抬起（3秒内）
				{
					unlock(1,20);
				}
				else if(sta==3)
				{
//					unlock(1,20);
//					MANCHESTER_OFF();
//					delay_mS(550);
//					MANCHESTER_ON();
					MANCHESTER_OFF();
					ASK_Tx(ASK_CONV(ASK_UNLOCK2));
					unlock(2,11);
					BBT(150);
					delay_mS(850);
					MANCHESTER_ON();
				}
				//voice_play(1);
			}
			else
			{bb(2);}
		}break;	
		case 1:
		{
			if(sta==1)
			{
				_card_del_all_();
				CardStateChange(0);			
			}
		}break;
		case 2:
		{
			if(sta==1)
			{
				card_add(gTemp,addr,err);		
			}
		}break;
		case 3:
		{
			if(sta==1)
			{
				card_del(addr,err);		
			}
		}break;
		case 128:
		{
			if(sta==1)
			{
				WrToROM(gTemp, 0, 3);
				CardStateChange(129);
				bb(1);	
			}
			WrToROM(gTemp, 0, 3);
			CardStateChange(129);
			bb(1);
		}break;
		case 129:
		{
			if(sta==1)
			{
				if(addr<1)
				{
					bb(2);
				}
				else
				{
					WrToROM(gTemp, 3, 3);
					WrToROM(gTemp, 6, 3);
					CardStateChange(0);
					BBT(400);//bb(1);
				}	
			}
		}break;
		case 130:
		{
			if(sta==1)
			{
				if(addr<2)
				{
					bb(2);
				}
				else
				{
					WrToROM(gTemp, 6, 3);
					_card_del_all_();
					CardStateChange(0);
					BBT(400);//bb(1);
				}	
			}
		}break;
		default:
			break;
	}
}
#endif










void SystemTickExec(void)		//
{
	
#if (_CONFIG_ID_CARD_==1)
	//CARD_Timeout_proc();
	ID_CARD_proc();
#endif	
}

void UserProgram(void)			//每秒10次
{
	static uint8 t1s=10;
#if (_CONFIG_ID_CARD_==1)
	//ID_CARD_proc();
	CARD_Timeout_proc();
#endif	

	if(WorkState==WORK_STANDBY) 
	{
		if(redconfig()&1)
		{MKJ_n=2;}//PWM1_on();
		else
		{MKJ_n=1;}//PWM1_on();
	}

	if(--t1s==0)
	{
		WorkTimeout_proc();
		TestProc();
		t1s=10;//5秒一次
	}
}



bit ASK_Proc_(void)
{
	uint8 i,j;
	i=ASK_Rx_get();

	if(i < ASK_CONV_L(ASK_HUJIAO_1) ) 
		return 0;//
	if(i > 200 ) //250ms
		return 0;//
Uart0_putbyte(1);Uart0_putbyte(i);
	if( i < ASK_CONV_H(ASK_HUJIAO_2) )//ASK_HUJIAO_1 or ASK_HUJIAO_2
	{//其它mkj发出
		//if(snj_n()==0)

//				{
//					snj_sel(1);
//					ASK_Tx(ASK_CONV(ASK_CLOSE));
//					delay_mS(30);
//				}
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}
	}
	else if( i < ASK_CONV_H(ASK_CLOSE) )//ASK_CLOSE
	{
		if(snj_n()==0)
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;		
		}
	}
	else if( i < ASK_CONV_H(ASK_HAND) )//ASK_HAND
	{
		if(snj_n()==0)
		{
			if(WorkState!=WORK_STANDBY)  //非待机转到接听状态
			{
				mkj_to_talk();
				WorkState=WORK_TAIK_;
			}
		}
	}
	else if( i < ASK_CONV_H(ASK_JIANSHI_2) )//16 ASK_JIANSHI_1 or ASK_JIANSHI_2
	{
		j=2;
		if(i<ASK_CONV_H(ASK_JIANSHI_1)) j=1;

			if(MKJ_n==1)
			{//先发ask，再切换
				//if(WorkState!=WORK_STANDBY)  //
				{
					snj_sel(1);
					ASK_Tx(ASK_CONV(ASK_CLOSE));
					delay_mS(30);
				}
			}
		if(MKJ_n==j)
		{
			snj_sel(0);
			mkj_to_monitor();
			WorkState=WORK_MONITOR_;
		}
		else
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}

	}
	else if( i < ASK_CONV_H(ASK_HHT) )	//ASK_HHT
	{//ASK_HHT
		if(snj_n()==0)
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}
	}
	else if( i < ASK_CONV_H(ASK_KAOJI) )	//20 ASK_KAOJI
	{
	}
	else if( i < ASK_CONV_H(ASK_UNLOCK2) )	//24 ASK_UNLOCK
	{
		if(snj_n()==0)
		{
			if(WorkState!=WORK_STANDBY)
			{
//				unlock(1,20);
				if(i<ASK_CONV_H(ASK_UNLOCK1))
					unlock(1,20);
				else
					unlock(2,11);//	
			}
		}
	}
return 1;
}	
	
bit ASK_Proc_2(void)
{
	uint8 i,j;
	i=ASK_Rx_get2();

	if(i < ASK_CONV_L(ASK_HUJIAO_1) ) 
		return 0;//
	if(i > 200 ) //250ms
		return 0;//
Uart0_putbyte(2);Uart0_putbyte(i);
	if( i < ASK_CONV_H(ASK_HUJIAO_2) )//ASK_HUJIAO_1 or ASK_HUJIAO_2
	{//其它mkj发出
		//if(snj_n()==1)
//				{
//					snj_sel(0);
//					ASK_Tx(ASK_CONV(ASK_CLOSE));
//					delay_mS(30);
//				}

		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}

	}
	else if( i < ASK_CONV_H(ASK_CLOSE) )//ASK_CLOSE
	{
		if(snj_n()==1)
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;		
		}
	}
	else if( i < ASK_CONV_H(ASK_HAND) )//ASK_HAND
	{
		if(snj_n()==1)
		{
			if(WorkState!=WORK_STANDBY)  //非待机转到接听状态
			{
				mkj_to_talk();
				WorkState=WORK_TAIK_;
			}
		}
	}
	else if( i < ASK_CONV_H(ASK_JIANSHI_2) )//16 ASK_JIANSHI_1 or ASK_JIANSHI_2
	{
		j=2;
		if(i<ASK_CONV_H(ASK_JIANSHI_1)) j=1;


			if(MKJ_n==1)
			{//先发ask，再切换
				//if(WorkState!=WORK_STANDBY)  //
				{
					snj_sel(0);
					ASK_Tx(ASK_CONV(ASK_CLOSE));
					delay_mS(30);
				}
			}
		if(MKJ_n==j)
		{

			snj_sel(1);
			mkj_to_monitor();
			WorkState=WORK_MONITOR_;
		}
		else
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}


	}
	else if( i < ASK_CONV_H(ASK_HHT) )	//ASK_HHT
	{//ASK_HHT
		if(snj_n()==1)
		{
			mkj_to_idle();
			WorkState=WORK_STANDBY;
		}
	}
	else if( i < ASK_CONV_H(ASK_KAOJI) )	//20 ASK_KAOJI
	{
	}
	else if( i < ASK_CONV_H(ASK_UNLOCK2) )	//24 ASK_UNLOCK
	{
		if(snj_n()==1)
		{
			if(WorkState!=WORK_STANDBY)
			{
//				unlock(1,20);
				if(i<ASK_CONV_H(ASK_UNLOCK1))
					unlock(1,20);
				else
					unlock(2,11);//unlock(2,20);		
			}
		}
	}
return 1;
}


void ASK_Proc(void)
{
	if(ASK_Proc_()) return;
	ASK_Proc_2();
}

//void test_(void)
//{
//	PWM1_8_set(15,29);
//	while(1)
//	{
//		PWM1_on();
//		delay_mS(100);
//		PWM1_off();
//		delay_mS(100);
//	
//	}
//	
//
//}






void main(void)
{
	PortConfig();
	init();



	InitSystemTick();	//init_timer0();
	EXIT_CRITICAL();	//
	serial_init();
		while(get_Uart0_Rx(gTemp))	//可能不止1帧数据 用while
		{
			//uart_cmd_pro();
			_nop_();
			
		}
		_Uart0_putbytes_("Z",1);
//		ES = 0;

BBT(150);
bb(1);
delay_mS(250);//待验证
WDT_reset();
	PWM1_8_set(15,29);
	mkj_to_idle();
	init_ROM();

delay_mS(500);	

ASK_init();
ASK_Tx(ASK_CONV(ASK_CLOSE));
mkj_to_idle();

#if (_CONFIG_ID_CARD_==1)
	ID_CARD_ON();
	CardStateChange(0);
#endif



//test_();
	while (1) 
	{
//		while(get_Uart0_Rx(gTemp))	//可能不止1帧数据 用while
//		{
//			//uart_cmd_pro();
//			_nop_();
//		}

		ASK_Proc();
		SystemTick();	//
		WDT_reset();	//
		Idle();
	}
}










