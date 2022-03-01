# PiStaTus ——Pi资源实时监控
## 简介
显示RPi的CPU温度，IP地址，CPU利用率，内存利用率到连接的OLED屏幕上
> 支持平台 4B(32位系统/64位系统)

* 界面中间数字为`CPU`温度
* `CPU`温度下方会显示树莓派的`IP`地址
* 下面4个条表示树莓派4个核心的利用率
* 右上角表示过去时刻`内存Mem`利用率的曲线图
* 右下角表示过去时刻`CPU总利用率`的曲线图
> 效果展示

<img src = "./img/pic1.png">
<img src = "./img/pic2.png">

## 接线
使用7接口的0.96寸oled屏幕（128*64）<br>
输入
~~~shell
gpio readall
~~~
来查看引脚分布
> 无法查看？
树莓派4B使用的是`2.50`版本的`WiringPi`，需要先升级到`2.52`版本，见下文 __使用__ ,升级后再输入`gpio readall`即可查看引脚。

VCC - > VCC<br>
GND - > GND<br>
D0 - > SCLK(BCM编号为11)<br>
D1 - > MOSI(BCM编号为10)<br>
RES - > GPIO. 3(BCM编号为22)<br>
DC - > GPIO. 2(BCM编号为27)<br>
CS - > GPIO. 0(BCM编号为17)<br>


## 使用
### Before Build
OLED使用`SPI`通信，依赖`wiringPi  -version 2.52`,树莓派本身自带`wiringPi -version 2.50` 需要升级至2.52或更高版本
更建议使用`WiringPi开源库`

#### Build WiringPi
~~~shell
cd ~
sudo apt install make gcc
git clone https://github.com/WiringPi/WiringPi.git
cd WiringPi
./build
~~~
然后输入
~~~shell
gpio -v
~~~
查看版本是否大于等于2.60

#### Enable SPI
~~~shell
sudo raspi-config
~~~
在其中打开使能`SPI`接口

### Build
1. 获取`PiStaTus`
~~~shell
cd ~
git clone https://github.com/Janey7695/PiStaTus.git
~~~
2. 编译`PiStaTus`
~~~shell
cd PiStaTus/
sudo make
~~~
会有如下输出：
~~~shell
Build dep PiStaTus.o successfully
Build oled base driver
Build oled api
/usr/bin/ld: 当搜索用于 /usr/lib/gcc/aarch64-linux-gnu/8/../../../../lib/libwiringPi.so 时跳过不兼容的 -lwiringPi 
/usr/bin/ld: 当搜索用于 /lib/../lib/libwiringPi.so 时跳过不兼容的 -lwiringPi 
/usr/bin/ld: 当搜索用于 /usr/lib/../lib/libwiringPi.so 时跳过不兼容的 -lwiringPi 
/usr/bin/ld: 当搜索用于 /usr/lib/gcc/aarch64-linux-gnu/8/../../../libwiringPi.so 时跳过不兼容的 -lwiringPi 
Build PiStaTus successfully
~~~
3. `PiStaTus`初始化
~~~shell
sh INSTALL.sh
~~~
初始化会设置`PiStaTus`为开机自启动，执行完这步后oled屏幕应该就有显示东西了，`PiStaTus`安装完成。
