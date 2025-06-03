# Integrantes:
- Francisca Abarca P.
- Isidora Cisternas C.
- Rodolfo Fernández V.
- Valentina Guzmán E.
- Fabián Solís P.

# Rama de laboratorio 2 asignatura Robótica y sistemas autónomos

# Parte 1: Configuración del Hardware y pruebas iniciales

• Conectar los sensores ultrasóonico HC-SR04 y RGB en Arduino.



• Programar Arduino para leer la distancia con HC-SR04 y mostrarla en
el monitor serie.


• Programar Arduino para leer los valores RGB y mostrar el color detectado.
(Lo de la calibración es un paso oculto)


• Analizar la precisón de los sensores en diferentes condiciones (luz,
superficie, distancia). FALTA

AQUI VAS VALENTINA
[Explicación y conexión correcta de componentes](https://youtu.be/FO4zradAQBo "clic para ver video")


* Conectar Arduino UNO con el driver de motores y programar el movimiento básico de los motores (adelante, atrás, giro) sin controlar la velocidad.

[Video movimientos básicos](https://youtu.be/BtpZs1f40o8?si=-_AthLWavxAlf-Dt "Adelante, atrás, giro")

**Código en archivo:** Prueba_Movimientos.ino
---

* Verificar el funcionamiento del sensor ultrasónico HC-SR04 midiendo distancias.

[Funcionamiento HC-SR04](https://youtu.be/wvHxKn6BggM?si=rP3vbJPwl7Z80Ynp "Midiendo distancias")

**Código en archivo:** Prueba_Sensor_ultraSonico.ino
---

* Analizar los datos del IMU MPUC6050 para medir inclinación o giros del robot.

[Datos IMU inclinación o giros](https://youtu.be/ShGm47EegdY?si=PW9aZJe60bWVoij4 "Inclinación o giros del robot")


**Código en archivo:** Prueba_Sensor_MPU.ino

**Código en archivo:** Calibracion.ino
---

**Análisis parte 1 en archivo:** parte1Análisis.pdf
---

## Parte 2: Cinemática y Dinámica de Robots Móviles usando un IMU

* Aplicar la ecuación de cinemática diferencial para estimar la posición del robot usando tiempo y velocidad de motores.

[Foto Puntos inicial, aproximado y final](https://drive.google.com/file/d/1b8-uuxRBczcfXvhely9kyNeEN0WE4qRK/view?usp=sharing "clic para ver la imagen")


**Código en archivo:** Estimar_Posicion_Cinematica.ino

### Estimación de la posición del robot

**Cálculo de la velocidad lineal**

Primero, calculamos la velocidad lineal del robot. Para esto, realizamos varias pruebas en las que el robot avanzaba en línea recta durante 5 segundos. Luego medimos la distancia que recorrió en cada prueba.
Hicimos entre 3 a 4 repeticiones y calculamos el promedio de las distancias obtenidas.

**Uso de la velocidad lineal en el código**

La velocidad lineal fue necesaria para aplicar las ecuaciones mencionadas en el punto 1 de la parte 2 del trabajo.
Estas ecuaciones fueron utilizadas en el código **`Estimar_Posicion_Cinemática`**, con el objetivo de que el programa estimara la posición del robot durante su desplazamiento.

**Secuencia de movimiento del robot**

El recorrido programado del robot consistió en:

* Avanzar en línea recta durante 3 segundos.
* Girar sobre su propio eje durante 2 segundos.

Esta secuencia se encuentra detallada en el código. En la imagen correspondiente, se observa la posición inicial del robot en el punto **(0, 0)** y la posición final medida manualmente en aproximadamente **(143, -35)**. Esta posición real demuestra que el robot no avanzó en línea recta de forma perfecta.

El programa estimó un punto final diferente, que corresponde a donde **el robot *debería*** haber terminado su recorrido, según los cálculos de cinemática.

**Análisis de posibles errores y mejoras**

La desviación entre la posición real y la estimada puede deberse a varias causas:

* La batería se agota rápidamente (se probaron 3 baterías distintas).
* La posición de las ruedas: una rueda está un poco más apretada que la otra.
* El valor de la velocidad lineal es una **aproximación**.
* Condiciones del piso donde se realizó la prueba.
---

* Hacer que el robot se mueva en línea recta y registrar desviaciones usando el sensor IMU para detectar la inclinación y giro del robot.

**Código en archivo:** Punto2_Parte2.ino

En superficie plana, el robot mostró un movimiento muy estable: rumbo recto (Yaw ≈ -0.4°), inclinación frontal leve pero constante (Pitch ≈ -2.5°), y casi sin inclinación lateral ni sacudidas. Esto indica que los errores de odometría (como la desviación de -35 cm en Y) no se deben a giros o inestabilidad, sino a limitaciones del modelo basado en PWM (velocidades desiguales, slip, no linealidades).

En suelo de baldosas, el comportamiento fue mucho más inestable: fuertes sacudidas verticales, grandes fluctuaciones en Pitch y Roll, y un Yaw con giros rápidos y correcciones constantes. La IMU evidenció cómo las irregularidades del terreno afectan drásticamente la estabilidad y precisión del robot, haciendo que la odometría simple sea aún menos confiable.

**Todos los detalles se encuentran en los siguientes documentos:**

**Archivo:** DatosIMUbaldosas.pdf
**Archivo:** DatosIMUsueloLiso.pdf
---

* Usar el sensor IMU MPU6050 para medir la inclinación del robot y ajustar su dirección en tiempo real, realizando correcciones en el movimiento de acuerdo a su orientación.

**Código en archivo:** Punto3_Parte2.ino

[Video correspondiente punto 3](https://youtu.be/OEEr5RuJbBo "clic para ver el video")
---

* Programar el PWM para controlar la velocidad de los motores y hacer que el robot se mueva a diferentes velocidades sin IMU, variando el tiempo de activación de los motores.

[Video correspondiente punto 4](https://youtu.be/tACz6qkU_rw "clic para ver el video")


**Código en archivo:** Punto4_Parte2.ino

Se detectó que el robot se desviaba al avanzar recto, debido a que la rueda izquierda giraba más lento por mayor resistencia mecánica. Para corregir esto, se implementó un **control en lazo cerrado** usando el giroscopio del IMU (MPU-6500).

Se estimó el ángulo Yaw integrando la velocidad angular (gyroZ\_cal) en el tiempo, y se usó un **controlador proporcional (P)** con un objetivo de 0° (mantener rumbo recto). El error de Yaw se transformó en una señal de corrección que ajustaba los PWM de las ruedas:
`pwmLeft = BASE_PWM + correction`, `pwmRight = BASE_PWM - correction`.

Durante la depuración, se corrigió la dirección de la corrección (el signo estaba invertido) y se ajustó la ganancia proporcional **Kp**, encontrando que **Kp = 2.0** ofrecía una corrección eficaz y estable. Con este valor, el robot mantuvo un rumbo recto durante 10 segundos de prueba, compensando exitosamente las diferencias mecánicas entre ruedas.

**Conclusión**: El controlador proporcional basado en IMU permitió mejorar significativamente la precisión del movimiento recto, validando la efectividad del control en lazo cerrado frente al lazo abierto.
---

# Acá se encuentran las respuestas al laboratorio 2 (parte 1 y 2)

## Preguntas parte 1

### ¿Qué es la percepción en robótica y por qué es fundamental en los sistemas autónomos?

La percepción en robótica es la capacidad de un robot para recolectar datos de su entorno (y a veces de su propio estado interno) y transformarlos en información útil.  Esta información permite al robot navegar, interactuar o cumplir tareas.

Es fundamental en los sistemas autónomos porque para que los robots puedan tomar decisiones inteligentes, necesitan percibir qué hay a su alrededor y construir una representación del mundo.  Esta capacidad les permite operar de manera independiente y adaptarse a entornos dinámicos. 


### En el sensor ultrasónico HC-SR04 ¿Qué parámetro se mide para calcular la distancia?

En el sensor ultrasónico HC-SR04, el parámetro que se mide para calcular la distancia es el tiempo que tarda una onda ultrasónica en viajar desde el sensor hasta un objeto y regresar como un eco.  Específicamente, el microcontrolador mide el tiempo durante el cual el pin ECHO del sensor está activado, que corresponde al tiempo de ida y vuelta de la onda sonora. 

### ¿Cómo influye el ruido en las mediciones del sensor ultrasónico y cómo podría reducirse?

El ruido puede afectar las mediciones del sensor ultrasónico causando datos inconsistentes, es decir, mediciones que saltan bruscamente sin una causa real.  Esto se debe a que los sensores no son perfectos y sus lecturas pueden estar afectadas por factores como interferencias electromagnéticas o el mal diseño del circuito. 


El ruido en las mediciones del sensor ultrasónico podría reducirse mediante técnicas de filtrado de datos.  El filtrado ayuda a suavizar la señal, eliminar picos falsos o extremos y, en general, mejorar la calidad de la información que recibe el sistema.  Algunos tipos de filtros que se pueden aplicar son:


  1. Filtro de media móvil. 
  2. Filtro de media ponderada. 
  3. Filtro pasa bajos. 

Estas técnicas permiten reducir las fluctuaciones no deseadas y obtener mediciones más precisas y confiables.



----

## Preguntas parte 2

## ¿Cómo se calcula la velocidad del robot sin encoders usando PWM?
