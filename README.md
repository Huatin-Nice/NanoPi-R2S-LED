# NanoPi-R2S-LED 修复wan/lan LED状态

NanoPi-R2S 在刷入armbian系统之后，两颗网络状态灯没法亮起来的解决方法。

下载 led-monitor.c 代码 (用于控制lan_led)

```
wget https://raw.githubusercontent.com/Huatin-Nice/NanoPi-R2S-LED/refs/heads/main/led-monitor.c
```

查看网络接口

```
ip link
```

找到网卡名称，wan口是end0， 另外一个enx....... 就是lan口

修改c代码中的IFACE_NAME，替换enxae3af448a08e为你自己网卡名称

```
#define IFACE_NAME "enxae3af448a08e"
```

创建存放目录

```
mkdir /etc/led/
```

编译代码

```
gcc led-monitor.c -o /etc/led/led-monitor
```

权限安全

```
chmod 655 /etc/led/led-monitor
```

将如下代码放入/etc/rc.local 开机自启动脚本最后的 exit 0 之前

```
echo stmmac-0:00:link > /sys/class/leds/wan_led/trigger
/etc/led/led-monitor >/dev/null 2>&1 &
```

完成，接下来只需要重启系统

```
reboot
```


## 说明: 

因为wan口是直接连接在总线上面的，linux内核里面提供了方法直接更新状态。

lan口搓代码每3秒监测一次，检测网络接口状态然后更新led状态。
