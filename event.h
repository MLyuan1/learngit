#ifndef __event__H
#define __event__H


#ifdef event_GLOBALS
#define event_EXT
#else 
#define event_EXT extern
#endif
event_EXT uint8 nbelled_n;
event_EXT uint8 gTemp[UART_BUF_MAX];//通用缓存

void SystemTickExec(void);//只被SystemTick()调用
void UserProgram(void);//只被SystemTick()调用





#endif


/*





*/
