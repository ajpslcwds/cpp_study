#include <stdlib.h>  
#include <stdio.h>  
#include <sys/time.h>  
#include <event.h>  
// ��ʱ�¼��ص�����   
void onTime(int sock, short event, void *arg)   
{   
    printf("Hello,World!\n");  
  
    struct timeval tv;   
    tv.tv_sec = 1;   
    tv.tv_usec = 0;   
    // ������Ӷ�ʱ�¼�����ʱ�¼�������Ĭ���Զ�ɾ����   
    event_add((struct event*)arg, &tv);   
}   
     
int main()   
{   
    // ��ʼ��   
    event_init();   
     
    struct event ev_time;   
    // ���ö�ʱ�¼�   
    evtimer_set(&ev_time, onTime, &ev_time);   
     
    struct timeval tv;   
    tv.tv_sec = 1;   
    tv.tv_usec = 0;   
    // ��Ӷ�ʱ�¼�   
    event_add(&ev_time, &tv);   
     
    // �¼�ѭ��   
    event_dispatch();   
     
    return 0;   
}  
