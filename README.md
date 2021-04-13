# PiStaTus ——Pi资源实时监控
## 简介
显示Pi的CPU温度，CPU利用率，内存利用率到连接的OLED屏幕上
> 支持平台 4B(32位系统/64位系统)

## 接线


## 使用
OLED使用`SPI`通信，依赖`wiringPi  -version 2.52`,树莓派本身自带`wiringPi -version 2.50` 需要升级至2.52版本（<a href="http://wiringpi.com/wiringpi-updated-to-2-52-for-the-raspberry-pi-4b/">详见原作者文章</a>）
~~~
cd /tmp
wget https://project-downloads.drogon.net/wiringpi-latest.deb
sudo dpkg -i wiringpi-latest.deb
~~~
然后输入
~~~
gpio -v
~~~
查看版本是否为2.52。