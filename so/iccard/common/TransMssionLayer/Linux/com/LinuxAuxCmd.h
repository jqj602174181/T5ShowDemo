/******************************************************************************************
* 
*  Copyright(C), 2002-2012, Centerm Information Co.,Ltd,All Rights Reserved
*
*  FileName:   AuxCmd.h
*
*  Author:      laiwenhui
*
*  Date:        2012-06-14 17:16
*
*  Description: ���ڲ�����ض���
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#ifndef _AUX_CMD_H_
#define _AUX_CMD_H_

/*���帨��*/
#define AUX1            (1)         //����1
#define AUX2            (2)         //����2
#define AUX3            (3)         //����3
#define AUX4            (4)         //����4

/*���ڷ�������*/
#define AUX_SIM_STAR                       (1)         //ʵ��
#define AUX_SIM_GREATWALL                  (2)         //����
#define AUX_SIM_GUOGUANG                   (3)         //����
#define AUX_SIM_NEWLAND                    (4)         //�´�½
#define AUX_SIM_WYSE                       (5)         //WYSE
#define AUX_SIM_SP                             (6)         //dcc�������ն�����

/*���������*/
#define CMD_LOCK_KEYBOARD                   ("\033[2h")     //��ס����
#define CMD_UNCLOCK_KEYBOARD            ("\033[2l")     //��������

#define CMD_AUX_OPEN                            ("\033!%d;%d;%d;%d%c")      //ʵ��򿪸���                                                                                          
#define CMD_AUX_DATATOHOST              ("\033[/53h")                            //ʵ�︨������ֱ��������                                                                                            
#define CMD_AUX_DTHOBO                       ("\033[/54l")                              //ʵ�︨���յ�һ���ֽ��������Ϸ���������                                                                                            
#define CMD_AUX_ENABLE                         ("\033[/50h")                             //ʵ�������ڲ���                                                                                          
#define CMD_AUX_DISABLE                        ("\033[/50l")                              //ʵ���ֹ���ڲ���                                                                                          
#define CMD_AUX_LEAD                             ("\033[/51h")                             //ʵ��ֱ��������������                                                                                          
#define CMD_AUX_END                               ("\033[/51l")                              //ʵ�ֱ�������������ڡ�����                                                                                          

#define CMD_AUX_OPEN_GG                     ("\033[/%d;%d;%d;%d;%d;%dM")      //����򿪸���
#define CMD_AUX_CLOSE_GG                    ("\033[/M")                                          //����رո���
#define CMD_AUX_LEAD_GG                      ("\033[/|")                                            //����������д
#define CMD_AUX_END_GG                        ("|")                                                     //����������д����

#define CMD_AUX_OPEN_GW                     ("\033[%d>")                         //���Ǵ򿪸���
#define CMD_AUX_CLOSE_GW                   ("\033[<")                              //���ǹرո���

#define CMD_AUX_OPEN_WYSE                 ("\033!%d;1m")           //WYSE�򿪸���
#define CMD_AUX_CLOSE_WYSE                ("\033!%d;0m")           //WYSE�رո���
#define CMD_AUX_ENABLE_WYSE              ("\033!b")                    //WYSE������
#define CMD_AUX_DISABLE_WYSE             ("\033!P")                    //WYSE��ֹ����

/*������Ѷ���ɼ����ڲ���ģ��ѡͨͨѶЭ��*/ 
#define CMD_JC_MPC           ("\033%B")               //�ر�����ģ��,ѡͨ������ָ����ģ��
#define CMD_JC_ICC             ("\033%I%d")           //�ر�����ģ��,ѡͨ�Ӵ�ʽ IC��ģ�顢�ǽӴ�ʽIC��ģ�顣
#define CMD_JC_IDC             ("\033%S%d")         //�ر�����ģ��,ѡͨ����֤ģ�顣

#endif
