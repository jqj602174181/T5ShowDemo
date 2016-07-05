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
*  Description: 辅口操作相关定义
*
*  History:     
*               
*     <flag>   <author>       <date>          <version>     <description>
*******************************************************************************************/
#ifndef _AUX_CMD_H_
#define _AUX_CMD_H_

/*定义辅口*/
#define AUX1            (1)         //辅口1
#define AUX2            (2)         //辅口2
#define AUX3            (3)         //辅口3
#define AUX4            (4)         //辅口4

/*辅口仿真类型*/
#define AUX_SIM_STAR                       (1)         //实达
#define AUX_SIM_GREATWALL                  (2)         //长城
#define AUX_SIM_GUOGUANG                   (3)         //国光
#define AUX_SIM_NEWLAND                    (4)         //新大陆
#define AUX_SIM_WYSE                       (5)         //WYSE
#define AUX_SIM_SP                             (6)         //dcc的特殊终端类型

/*控制命令定义*/
#define CMD_LOCK_KEYBOARD                   ("\033[2h")     //锁住键盘
#define CMD_UNCLOCK_KEYBOARD            ("\033[2l")     //解锁键盘

#define CMD_AUX_OPEN                            ("\033!%d;%d;%d;%d%c")      //实达打开辅口                                                                                          
#define CMD_AUX_DATATOHOST              ("\033[/53h")                            //实达辅口数据直接送主机                                                                                            
#define CMD_AUX_DTHOBO                       ("\033[/54l")                              //实达辅口收到一个字节数据马上发给服务器                                                                                            
#define CMD_AUX_ENABLE                         ("\033[/50h")                             //实达允许辅口操作                                                                                          
#define CMD_AUX_DISABLE                        ("\033[/50l")                              //实达禁止辅口操作                                                                                          
#define CMD_AUX_LEAD                             ("\033[/51h")                             //实达直接往辅口送数据                                                                                          
#define CMD_AUX_END                               ("\033[/51l")                              //实达“直接送数据至辅口”结束                                                                                          

#define CMD_AUX_OPEN_GG                     ("\033[/%d;%d;%d;%d;%d;%dM")      //国光打开辅口
#define CMD_AUX_CLOSE_GG                    ("\033[/M")                                          //国光关闭辅口
#define CMD_AUX_LEAD_GG                      ("\033[/|")                                            //国光往辅口写
#define CMD_AUX_END_GG                        ("|")                                                     //国光往辅口写结束

#define CMD_AUX_OPEN_GW                     ("\033[%d>")                         //长城打开辅口
#define CMD_AUX_CLOSE_GW                   ("\033[<")                              //长城关闭辅口

#define CMD_AUX_OPEN_WYSE                 ("\033!%d;1m")           //WYSE打开辅口
#define CMD_AUX_CLOSE_WYSE                ("\033!%d;0m")           //WYSE关闭辅口
#define CMD_AUX_ENABLE_WYSE              ("\033!b")                    //WYSE允许辅口
#define CMD_AUX_DISABLE_WYSE             ("\033!P")                    //WYSE禁止辅口

/*升腾资讯集成键盘内部各模块选通通讯协议*/ 
#define CMD_JC_MPC           ("\033%B")               //关闭其它模块,选通磁条、指纹仪模块
#define CMD_JC_ICC             ("\033%I%d")           //关闭其它模块,选通接触式 IC卡模块、非接触式IC卡模块。
#define CMD_JC_IDC             ("\033%S%d")         //关闭其它模块,选通二代证模块。

#endif
