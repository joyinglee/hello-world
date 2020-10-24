###############################################
##raspberry pi
###############################################
#####################增加交换
###禁止直接执行本脚本#########
sudo dphys-swapfile swapoff
#在/etc/dphys-swapfile文件中：
#CONF_SWAPSIZE=1024
sudo dphys-swapfile setup
sudo dphys-swapfile swapon
#####################远程桌面
screen
＃Ctrl +a +d 退出
x11vnc -forever -usepw -display :0
################################################
##GEANT 4
################################################
build-essential
cmake 
-DCMAKE_INSTALL_PREFIX=/absolute/path/to/geant4.10.06-install
-DGEANT4_INSTALL_DATA=ON 
-DGEANT4_USE_GDML=ON
GEANT4_USE_QT=ON
/path/to/geant4.10.06
##########################
#slackware
##########################
/usr/share/zoneinfo
/etc/localtime
cat /etc/os-release
