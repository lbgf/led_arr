# led 灯阵控制

#### 介绍
控制led的灯阵，主控板是esp32，可以使用串口或蓝牙进行通讯

#### 硬件准备
一块esp32的主控板（本实例选用esp32 uno d1，原因是便宜，兼容arduino，还自带wifi和蓝牙模块^_^!）

![a](https://gitee.com/lbgf/led_arr/raw/master/res/1.png)

一块16*16的led灯阵板

![b](https://gitee.com/lbgf/led_arr/raw/master/res/2.png)

一个5V的电源和N条杜邦线

#### 接线方式

接线比较简单

主控板 IO12 接入 led灯板 DIN

主控板 GND 接入 led灯板 GND

led灯板 单独接5V电源线

![c](https://gitee.com/lbgf/led_arr/raw/master/res/3.png)

#### 开发环境
arduino

#### 文件说明
16_16文件夹为控制16*16的led灯阵（屏）的程序源码，可用arduino工具烧录到主控板上

#### 通讯协议
f_ ：结束并显示 （ 例子：f_@ ）

c_ ：清屏 （ 例子：c_@ ）

p_ ：在屏上显示一个点 （ 例子：p_0000ffcc00@ 指令意思是在坐标(0,0)位置显示一个点，颜色(RGB)为(ffcc00)）

s_ ：在屏上显示字符串 （ 例子：s_0006ffcc00ABC@ 指令意思是在坐标(0,6)位置显示字符串ABC，颜色(RGB)为(ffcc00)）

v_ ：在屏上显示一条水平线 （ 例子：v_000006ffcc00@ 指令意思是在坐标(0,0)和(6,0)之间位置显示一条线，颜色(RGB)为(ffcc00)）

h_ ：在屏上显示一条垂直线 （ 例子：h_000006ffcc00@ 指令意思是在坐标(0,0)和(0,6)之间显示一条线，颜色(RGB)为(ffcc00)）

每个指令以 @ 结尾

#### 测试例子
可使用串口进行调试，例如arduino的调试器，在文本处输入指令并发送，如下图

![d](https://gitee.com/lbgf/led_arr/raw/master/res/7.png)

1.显示一个点
指令：p_0000ffcc00@f_@

![d](https://gitee.com/lbgf/led_arr/raw/master/res/4.png)

2.显示字符串
指令：s_0006ffcc00ABC@f_@

![d](https://gitee.com/lbgf/led_arr/raw/master/res/5.png)

3.绘制一条线
指令：v_000006ffcc00@f_@

![d](https://gitee.com/lbgf/led_arr/raw/master/res/6.png)

4.清屏
指令：c_@f_@

#### 特别的工具
利用通讯协议我们做个工具玩一下硬件，可以关注一下相关的另一个项目，可视化编辑器（[ks_ide](https://gitee.com/lbgf/ks_ide)），如下图

![a1](https://gitee.com/lbgf/ks_ide/raw/master/setup/3.png)

#### 参考资料
<https://www.arduino.cc>  
