#####################增加交换
###禁止直接执行本脚本#########
sudo dphys-swapfile swapoff
#在/etc/dphys-swapfile文件中：
#CONF_SWAPSIZE=1024
sudo dphys-swapfile setup
sudo dphys-swapfile swapon
