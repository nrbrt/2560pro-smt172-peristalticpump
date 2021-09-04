mkdir /home/pi/mega;
cd /home/pi/mega;
npm install avrgirl-arduino;
wget -O mega2560-dual-smt172-peristalticpump.ino.mega.hex https://raw.githubusercontent.com/nrbrt/2560pro-smt172-peristalticpump/master/mega2560-dual-smt172-peristalticpump.ino.mega.hex;
wget -O programmega.js https://raw.githubusercontent.com/nrbrt/2560pro-smt172-peristalticpump/master/programmega.js
node /home/pi/mega/programmega.js;
cd /home/pi;
rm -r /home/pi/mega;
