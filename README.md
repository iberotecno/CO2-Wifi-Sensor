<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://iberotecno.com/">
    <img src="https://iberotecno.com/images/logo_hz.png" alt="Logo" width="277" height="46">
  </a></p>
  

  <h1 align="center">Detector de CO2 conectado a Internet</h1>
<p align="center">Hecho con ❤️ :smile: por iberotecno</p>
  <p align="center">
    Puedes obtener todas las piezas necesarias para hacer el montaje y una placa con el firmware ya grabado en:
    <br />
    <a href="https://iberotecno.com/tienda/kits-de-arduino/35-detector-de-co2-y-calidad-del-aire.html"><strong>Tienda iberotecno »</strong></a>
    <br />
  </p>
</p>


## Introducción

<img src="https://i.imgur.com/HEfpYgu.png" alt="image" width="430" height="330" >

Este software está pensado para poder aprender y experimentar con la medición de CO2 en el aire (en partes por millón) utilizando la placa ESP32 compatible con el ecosistema Arduino y con capacidad de conectarse a Internet por wifi. El software cuenta con las siguientes funciones destacadas:

* Panel de configuración sencillo, conectandose via Wifi al dispositivo en modo punto de acceso AP.
* Enviar las lecturas al servidor gratuito ThingSpeak via Wifi. Descargar los datos. Insertarlos en una web.
* Poder establecer un offset personalizado para calibrar el sensor.
* Poder usar un programa para móvil que conecte con tu ThingSpeak con el que poder recibir alertas si se supera cierto nivel de CO2.
* Poder usar un LED que cambie de color según la calidad del aire.
* Poder usar un relé que active un aparato como una alarma, una sirena o un ventilador que funcione a AC 220V.
* Poder usar una pantalla OLED para mostrar la información.

Es un proyecto muy bueno para enseñar Arduino en escuelas empresas o institutos. La mejor forma de aprender es experimentar por uno mismo.

## Sensor electroquímico

El sensor MQ-135 es capáz de detectar diversos tipos de gases. En su interior, dispone de un sensor que reaccione variando su resistividad según la cantidad de moléculas de gases presentes en el ambiente. A diferencia de los sensores NDIR, de mucho más coste, este sensor puede sufrir variaciones en sus mediciones según la humedad y la temperatura en el ambiente. Sin embargo, el sensor viene equipado con un calefactor interno dentro de su módulo. De esta forma, se logran mediciones muy precisas, lineales y estables en comparación en el uso del sensor sin el calefactor.

## Calibración

El sensor es capáz de detectar diferentes tipos de gases, por ello a su medición en bruto se le aplica automáticamente un offset. Dicho offset ha sido obtenido comparando las mediciones del sensor con un medidor de CO2 profesional. Si usted lo desea, puede hacer su propia calibración y modificar el parámetro del offset de forma sencilla a través del panel de configuración Wifi.

<img src="https://iberotecno.com/tienda/img/cms/co2/facil.jpg" alt="image" width="550" height="330" >

## Posibilidad de compra en iberotecno tienda

Si no quieres fallar, te lo ponemos fácil, entra en <a href="https://iberotecno.com/tienda/">tienda.iberotecno.com</a>

* No tendrás problemas de compatibilidad a la hora de elegir los componentes.
* Envío rápido desde España en 24 horas.
* Soporte por chat, email o teléfono.
* Garantía de 2 años y 14 días de devolución.
* Vídeo explicativo del código fuente para aprender uno mismo o enseñar a los alumnos.

<img src="https://i.imgur.com/lloOV3z.png" alt="image" width="600" height="330" >


## Lista de materiales

1x Caja estanca IP67<br />
1x Sensor de gases MQ-135<br />
1x LED RGB WS2812B<br />
1x Pantalla OLED (opcional)<br />
1x Relé 5V 250V 10A (opcional)<br />
1x Placa de control basada en ESP32 con tecnología Wifi, Bluetooth y salida a +5V<br />
1x Cable USB<br />
1x Transformador USB 5V 1A<br />
1x Cables dupont macho y hembra para la conexión<br />

<img src="https://i.imgur.com/kbFXw4D.png" alt="image" width="430" height="330" >

## Requisitos de software

Si adquieres ek kit de montaje en iberotecno tienda no necesitarás más que conectarte por Wifi a la placa y acceder a su sencillo y cómodo panel de control. 

Si realizas el montaje por ti mismo, necesitarás realizar estos pasos previos:

* <a href="https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/107-como-instalar-y-configurar-el-ide-de-arduino-tutorial">Instalar el IDE de Arduino</a><br />
* <a href="https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/108-como-instalar-esp32-en-el-ide-de-arduino-tutorial">Instalar las dependencias para ESP32</a><br />

Una véz hecho, descarga el firmware que necesites desde esta página y grábalo en tu placa. Recuerda que debes descargar previamente las librerías necesarias, que se indican en el código fuente.

<img src="https://iberotecno.com/tienda/img/cms/co2/pantalla/COMPRENSION.jpg" alt="image" width="530" height="330" >

## Proceso de montaje y conexiones

* Versión LED: <a href="https://www.youtube.com/watch?v=x2a2iRNfipQ">Vídeo</a><br />
* Versión Relé: <a href="https://www.youtube.com/watch?v=o4NenwlGSC4">Vídeo</a><br />
* Versión pantalla OLED: <a href="https://www.youtube.com/watch?v=EoOteV0yaKs">Vídeo</a>

<img src="https://i.imgur.com/IL1LEyf.png" alt="image" width="300" height="300" >


## Manual de instrucciones

* Versión pantalla OLED: <a href="https://iberotecno.com/tienda/documentos/instrucciones_co2_pantalla.pdf">Manual en pdf</a>

<img src="https://iberotecno.com/tienda/img/cms/co2/colores.jpg" alt="image" width="530" height="330" >

## English

This software is capable to detect CO2 in the air (ppm). The data is sended via wifi to ThingSpeak server and is possible to view graphics and historic data. Also you can add a widgwet to your smartphone and configure alerts based on the CO2 level. 

The device haves a LED that changes his color depending on the CO2 level. The configuration is very easy. The device starts in AP mode and shows an easy to use UI and control panel.

* More info at: https://iberotecno.com/tienda/kits-de-arduino/35-50-detector-de-co2-y-calidad-del-aire.html
* User manual (Spanish): https://iberotecno.com/tienda/documentos/instrucciones_co2.pdf
* You will need to install Arduino IDE: https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/107-como-instalar-y-configurar-el-ide-de-arduino-tutorial
* And the requiered dependencies for ESP32 boards: https://iberotecno.com/blog-supratecno/47-tutoriales-paso-a-paso/108-como-instalar-esp32-en-el-ide-de-arduino-tutorial

You have the possibility of reset wifi settings with pin 27 set to HIGH at reboot. 

You have the possibility to format SPIFFS to original state with no data stored with the pin 26 set to HIGH at reboot.

Thanks and enjoy!!
