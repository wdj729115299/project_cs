# project_cs
本项目的建立，主要是为了彻底理解linux下C/S架构下的I/O模型。
主要代码包括以下内容：
1.最基本的单进程C/S架构（client和server的对话）
2.server端为多进程的C/S架构。（client和server的对话）
3.server端为多线程的C/S架构。（client和server的对话）
4.server端为各种I/O模型的架构。（client和server的对话）
5.server端为基于libev的架构。（server端为http服务器）

以上的每种模式，都要实现一个简单的demo

整体目标：
1.实现一个基于多线程的简单聊天服务器（代码不少于一千行）
2.实现一个基于I/O多路复用的聊天服务器（代码不少于三千行）
3.实现一个基于libev的http简单服务器（代码不少于六千行）

本次项目最终包含的子系统：
1.日志系统
2.错误处理系统
3.进/线程池
4.内存池
5.子连接系统

第一阶段
1.简单的unix echo程序。(2-17)
2.多进程unix echo 程序。(2-17)
3.多线程unix echo 程序。(2-18)
4.完善echo程序。(2-19)

需要实现四个目标
1.simple ftp,多线程 
  simple ftp-v2.0目标：
  					1.维护一个连接池
  					2.模块分离
  					3.面向大并发的处理
  					4.采用多进程
2.simple http select
3.simple redis poll/epoll
4.内核实现vpn/dpi

