CO2 Air Quality Sensor
=======

This software is capable to detect CO2 in the air (ppm). The data is sended via wifi to ThingSpeak server and is possible to view graphics and historic data. Also you can add a widgwet to your smartphone and configure alerts based on the CO2 level. 

The device haves a LED that changes his color depending on the CO2 level. The configuration is very easy. The device starts in AP mode and shows an easy to use UI and control panel.

More info at: https://iberotecno.com/tienda/kits-de-arduino/35-50-detector-de-co2-y-calidad-del-aire.html

User manual (Spanish): https://iberotecno.com/tienda/documentos/instrucciones_co2.pdf

You will need to install Arduino IDE: https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/107-como-instalar-y-configurar-el-ide-de-arduino-tutorial

And the requiered dependencies for ESP32 boards: https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/108-como-instalar-esp32-en-el-ide-de-arduino-tutorial

You have the possibility of reset wifi settings with pin 27 set to HIGH at reboot. 

You have the possibility to format SPIFFS to original state with no data stored with the pin 26 set to HIGH at reboot.

Thanks and enjoy!!
