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
################################################
#slackware
################################################
/usr/share/zoneinfo
/etc/localtime
cat /etc/os-release
################################################
#mariaDB
################################################
DROP USER 'root'@'localhost';
CREATE USER 'root'@'%' IDENTIFIED BY '密码';
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
FLUSH PRIVILEGES;
################################################
#Nextcloud
################################################
crontab -u www-data -e
*/30  *  *  *  * php -f /var/www/html/pan/cron.php
php-zip php-dompdf php-xml php-curl php-mbstring php-mysql php-intl php-bcmath php-gmp php-imagick 
################################################
#伪-内网穿透
################################################
#本地运行，将本地的端口映到远程的端口
ssh -fCNR 远程的内部端口:本地或内网地址:本地端口 用户@地址
#远程运行，将远程的端口暴露出去
ssh -fCNL *:远程的暴露端口:远程的本地地址:远程的内部端口 远程的本机地址
#或者sshd_config加入GatewayPorts yes,只要第一条就可以了。
