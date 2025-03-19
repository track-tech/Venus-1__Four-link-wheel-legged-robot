# 启明-1 四连杆轮腿机器人 | Venus-1 Four-link wheel-legged robot  

<img src=".\3.Docs & Pic\cover.jpg" alt="Real Robot" />  

## 0x00 声明&介绍 | Declaration & Introduction  

该项目为**个人本科毕业设计**，由于时间与个人能力不足，**存在设计缺陷**，请谨慎复刻。  
This project is an **undergraduate graduation design**. Due to time constraints and personal skill limitations, **design flaws exist**. Replication is not recommended.  

项目遵循**GPL 3.0**协议，量产及商业使用需授权。  
This project is licensed under **GPL 3.0**. Authorization is required for mass production or commercial use.  

> 演示视频合集 | Demo Videos  
> [Venus轮腿机器人视频合集]: https://space.bilibili.com/21198864/lists/4193332?type=series

**概况 | Overview**  
- 机械建模使用SolidWorks 2024 SP0.1绘制，使用请注意软件版本  
  Mechanical modeling is done in SolidWorks 2024 SP0.1. Please check software compatibility.  
- 基于STM32H723 HAL库开发，使用FreeRTOS进行任务管理  
  Developed using STM32H723 HAL library with FreeRTOS for task management.  
- 关节电机使用小米CyberGear微电机，轮电机使用DJI C610+M2006 P36  
  Joint motors: Xiaomi CyberGear micro motors; Wheel motors: DJI C610 + M2006 P36.  
- 外壳及主要结构均使用3mm 3240环氧板（玻纤板），高强度  
  Main structure: 3mm 3240 epoxy fiberglass board (high strength).  
- 完全使用PID控制  
  Fully PID-controlled.  
- 机器人遥控使用FS-I6X 主控通过SBUS与接收机FS-IA6B通信  
  Remote control: FlySky FS-I6X transmitter with FS-IA6B receiver via SBUS.  
- 基本采用商业模块构件，拼装快速  
  Primarily uses commercial modules for rapid assembly.  

---

### 机体测试 | Robot Testing  

#### 重心自适应 | Self-Adaptive Center of Gravity  
<img src=".\3.Docs & Pic\最终测试插图\重心自适应new.jpg" alt="重心自适应new" style="zoom:25%;" />  

#### 下落缓冲测试 | Drop Buffer Test  
<img src=".\3.Docs & Pic\最终测试插图\下落测试.jpg" alt="下落测试" style="zoom:35%;" />  

#### 主动悬挂测试（Roll角自稳定） | Active Suspension Test (Roll Angle Stabilization)  
<img src=".\3.Docs & Pic\最终测试插图\机体主动悬挂测试-左侧.jpg" alt="机体主动悬挂测试-左侧" style="zoom:25%;" />  
<img src=".\3.Docs & Pic\最终测试插图\机体主动悬挂测试-前侧.jpg" alt="机体主动悬挂测试-前侧" style="zoom:25%;" />  

#### 跳跃测试 | Jump Test  
<img src=".\3.Docs & Pic\最终测试插图\机体跳跃测试.jpg" alt="机体跳跃测试" style="zoom:40%;" />  

---

## 0x01 机械结构 | Mechanical Structure  

主体结构使用3mm 3240环氧树脂板及六面四方螺母固定，电气仓3D打印制成。  
Main structure: 3mm 3240 epoxy board with hex nuts. Electrical compartment is 3D-printed.  

### 连杆设计 | Linkage Design  
连杆由Pyslvs拟合后，在SolidWorks中建模。  
Linkages are designed in Pyslvs and modeled in SolidWorks.  

> [Pyslvs官网]: https://pyslvs-ui.readthedocs.io/en/stable/

| <img src=".\3.Docs & Pic\机械部分插图\pyslvs.png" style="zoom:50%;" /> | <img src=".\3.Docs & Pic\机械部分插图\pyslvs运动参数分析图片.png" style="zoom:80%;" /> |  

### 三维简图 | 3D Schematic  
<img src=".\3.Docs & Pic\机械部分插图\车体简图.bmp" style="zoom:50%;" />  

| 电器仓示意 | 截面视图 |  
| :--------: | :------: |  
| <img src=".\3.Docs & Pic\机械部分插图\电气仓.bmp" style="zoom:33%;" /> | <img src=".\3.Docs & Pic\机械部分插图\机体截面图.bmp" style="zoom:33%;" /> |  

---

## 0x02 电气连接 | Electrical Connections  

系统电源以及CAN总线通过中心分电板集合分配，下图为连接示意：  
Power and CAN bus are distributed via a central power board. Connection diagram:  
<img src=".\3.Docs & Pic\软硬件部分插图\连接图.png" alt="连接图" />  

分电板开源链接 | Power Board Open-Source Link:  
[XT60/XT30分电板 | CAN Bus分线板]: https://oshwhub.com/marktracker/xt60-xt30-fen-dian-ban-can-fen-xian-ban

<img src=".\3.Docs & Pic\最终测试插图\分线板实物图.jpeg" alt="分线板实物图" style="zoom:30%;" />  

| 实物装配图-1 | 实物装配图-2 |  
| :----------: | :----------: |  
| <img src=".\3.Docs & Pic\最终测试插图\最终测试插图.jpeg" style="zoom:15%;" /> | <img src=".\3.Docs & Pic\最终测试插图\最终测试插图.jpg" style="zoom:15%;" /> |  

---

## 0x03 软件架构 | Software Architecture  

<img src=".\3.Docs & Pic\软硬件部分插图\软件层次图.png" alt="软件层次图" style="zoom:50%;" />  

软件整体分为五大块，具体见完整程序工程。  
The software is divided into five modules. Refer to the full code repository for details.  

---

## 0x04 参考成本&已知缺陷报告 | Cost & Known Issues  

### 参考成本表格 | Cost Table  

| 物料名 | Component | 单价（RMB） | Unit Price (RMB) | 数量 | Qty | 总价（RMB） | Total (RMB) |  
|--------|-----------|-------------|------------------|------|-----|-------------|-------------|  
| CyberGear微电机 | CyberGear micro motor | 499 | 499 | 2 | 2 | 998 | 998 |  
| DJI M2006动力套装 | DJI M2006 Kit (C610+M2006 P36) | 434 | 434 | 2 | 2 | 868 | 868 |  
| 主控开发板 | Main Control Board (YCMC-H7A/DM-MC02) | 259 | 259 | 1 | 1 | 259 | 259 |  
| 3240环氧板定制费用 | 3240 Epoxy Board Customization | 200 | 200 | 1 | 1 | 200 | 200 |  
| 轴承+螺栓螺母 | Bearings & Fasteners | 40 | 40 | 1 | 1 | 40 | 40 |  
| 中心分电板 | Central Power Board | 10 | 10 | 1 | 1 | 10 | 10 |  
| 连接线束+接口 | Cables & Connectors | 25 | 25 | 1 | 1 | 25 | 25 |  
| 24V35C航模锂电 | 24V35C LiPo Battery | 176 | 176 | 1 | 1 | 176 | 176 |  
| 遥控器+接收机 | Remote Controller (FS-I6X + FS-IA6B) | 301 | 301 | 1 | 1 | 301 | 301 |  
| **合计** | **Total** | | | | **2877** (遥控器 included) / **2576** (遥控器 excluded) | |  

