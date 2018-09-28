## Pasos para cargar programar el ESP8266
1. Usamos conexion como muestra el diagrama *preload*
2. Presionamos el boton izquierdo y sin soltar presionamos el derecho *el modulo wifi hace un pequeño parpadeo*
3. Configuramos el SSID y Contraseña de nuestra red WiFi y la URL en main.ino
4. Compilamos y subimos el main.ino al microcontrolador
5. Usamos conexion como muestra el diagrama *postload*
6. Listo ahora, el modulo ESP8266 manda cada 10 segundos la información

Nota: Se pueden visualizar algunos datos en el monitor serial si no se desconectan los cables RX y TX del arduino
Nota2: Se pueden desconectar los cables a los pines 0 y 1 del arduino y continuara funcionando
