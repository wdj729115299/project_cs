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

02-06~02-12 第一周目标
实现基本的单进程傻瓜式聊天服务器（代码不少于600行）