# Integrantes:
- Francisca Abarca P.
- Isidora Cisternas C.
- Rodolfo Fernández V.
- Valentina Guzmán E.
- Fabián Solís P.

# Rama de laboratorio 1 asignatura Robótica y sistemas autónomos

# Parte 1: Identificación de componentes y configuración


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

# Acá se encuentran las respuestas al laboratorio 1 (parte 1 y 2)

## Preguntas parte 1

### ¿Qué función cumple los sensores, actuadores y controladores en el robot?

**Sensores**

Los sensores son los "órganos de los sentidos" del robot.

Su función principal es **detectar** el entorno y recopilar información. Esto permite al robot **percibir** qué hay a su alrededor, construir una representación del mundo y así **tomar decisiones “inteligentes”**. 

Estos pueden medir magnitudes como posición, distancia, proximidad, luz, sonido, fuerza, presión y también información inercial como aceleración y velocidad angular. La información recopilada por los sensores se utiliza para navegar, interactuar o cumplir tareas.

**Actuadores:**

Un actuador es un dispositivo que convierte energía en movimiento o fuerza. 

Su función es generar movimiento en el robot, permitiéndole realizar acciones físicas. 

Estos permiten que un robot:
-Se desplace (con ruedas o patas)
-Manipule objetos (con brazos robóticos)
-Aplique fuerza y precisión. 

Los actuadores pueden ser eléctricos (como motores DC, servomotores y motores paso a paso), neumáticos, hidráulicos o piezoeléctricos.

**Controladores**

El controlador es el **"cerebro" del robot**. 

Su función es procesar los datos recibidos de los sensores y coordinar las acciones del robot en tiempo real.

Gestiona la entrada de datos, ejecuta algoritmos y envía señales de control a los actuadores para que el robot realice las tareas deseadas.

### ¿Cómo se puede estimar la velocidad sin encoders?

De acuerdo a lo que hemos visto en el curso, hay dos maneras de estimar la velocidad sin utilizar encoders:

<u><strong>Experimental/manual</strong></u>
* Midiendo el tiempo que tarda el robot en recorrer una distancia en línea recta. 
* Calculamos la velocidad lineal (v): distancia/tiempo
* Si ambas ruedas giran igual, podemos suponer: 
  $$\omega_R = \omega_L = \frac{2v}{r}$$
* Si hacemos girar el robot en el sitio (una rueda hacia adelante y otra hacia atrás), podemos medir su velocidad angular y deducir la diferencia entre $\omega_R$ y $\omega_L$.

<u><strong>Integrando datos de la IMU</strong></u>

Si se dispone de una IMU, esta nos entrega los siguientes datos:

* Aceleración lineal $a_x, a_y$
* Velocidad angular $\omega_z$ (Yaw Rate)
* El modelo clásico es:

$$ \dot{x} = v \cos(\theta) $$
$$ \dot{y} = v \sin(\theta) $$
$$ \dot{\theta} = \omega $$

podemos hacer lo siguiente:

**Para obtener estimar la velocidad, integrar la aceleración**

  $$ v(t) = \int a_x(t) dt $$

donde:
* $v(t)$: velocidad lineal estimada en el instante $t$.
* $a_x(t)$: aceleración lineal medida por la IMU en el eje X local.

### ¿Cómo afecta la falta de encoders a la precisión del movimiento?

La falta de encoders impide medir directamente el movimiento real de las ruedas, obligando al robot a depender de estimaciones (manuales o por IMU) que no pueden compensar errores como el deslizamiento o el ruido. Estos pequeños errores constantes entre el movimiento estimado y el real se suman en cada paso. Dado que cada nueva estimación de posición y orientación se basa en la anterior —que ya era incorrecta—, los errores se componen y crecen, provocando una discrepancia cada vez mayor entre dónde cree el robot que está y su ubicación real. Esto disminuye drásticamente la precisión del movimiento a lo largo del tiempo y la distancia.


### ¿Qué es PWM y cómo ayuda a controlar la velocidad de los motores?

**¿Qué es?**
PWM (Pulse Width Modulation o Modulación por Ancho de Pulso) es una técnica utilizada para controlar la potencia suministrada a un dispositivo variando el ciclo de trabajo de una señal digital. 
Convierte una señal digital en una señal analógica simulada.

**¿Cómo ayuda a controlar la velocidad de los motores?**

* Se aplica una señal digital oscilante al motor a través del driver.
* El ciclo de trabajo de esta señal es la proporción del tiempo durante un período en que la señal está en estado "alto" (encendido) en comparación con el tiempo en que está en estado "bajo" (apagado).
* Al variar el ciclo de trabajo, se varía la cantidad promedio de voltaje que se aplica al motor.
* Un ciclo de trabajo alto significa que el motor recibe la mayor parte del voltaje y girará más rápido.
* Un ciclo de trabajo bajo significa que el motor recibe menos voltaje promedio y girará más lento.

El PWM permite controlar la velocidad del motor de forma eficiente al regular la potencia eléctrica que se le entrega, sin necesidad de variar el voltaje de la fuente de alimentación.

### ¿Cómo afecta el control de velocidad a la precisión de la navegación sin encoders?

El control de velocidad mediante PWM puede mejorar la precisión de la navegación sin encoders en comparación con simplemente aplicar la máxima potencia a los motores, pero **no elimina las limitaciones inherentes a la falta de retroalimentación directa**

* Al poder ajustar la velocidad de los motores, se puede intentar seguir las velocidades deseadas para cada rueda según un modelo cinemático. Esto permite realizar giros controlados y movimientos en línea recta de manera más predecible que si los motores siempre funcionaran a máxima velocidad.
* Un control de velocidad más fino puede ayudar a reducir el deslizamiento de las ruedas en ciertas condiciones, ya que se puede aplicar la potencia necesaria en lugar de un exceso que podría causar patinaje.
* Sin embargo, sin encoders, no hay una forma directa de verificar si las ruedas están girando a la velocidad deseada. Factores como la carga, la fricción del suelo, o las pequeñas diferencias entre motores pueden hacer que las velocidades reales difieran de las ordenadas, lo que inevitablemente llevará a errores acumulativos en la odometría (estimación de la posición y orientación).
* Aunque el control de velocidad permite una ejecución más predecible de los comandos de movimiento, la ausencia de retroalimentación sobre la distancia real recorrida y la rotación de las ruedas impide corregir los errores que se van acumulando durante la navegación.
---

## Preguntas parte 2

## ¿Cómo se calcula la velocidad del robot sin encoders usando PWM?

Para calcular la velocidad de un robot sin encoders usando PWM, se sigue un procedimiento experimental:

1.  **Controlar la Potencia del Motor con PWM**:
    * Se utiliza una señal PWM (Modulación por Ancho de Pulso) para controlar la velocidad de los motores. Esto se hace, por ejemplo, con la función `analogWrite(pinPWM, valorPWM);` en Arduino, donde `valorPWM` (un número entre 0-255) se envía al pin de habilitación del controlador de motor.
    * Un controlador de motores como el L298N "Controla velocidad mediante PWM".

2.  **Realizar Medición Experimental para el Valor de PWM Establecido**:
    * Debido a que no hay encoders, el método es "experimental/manual".
    * Se hace que el robot recorra una distancia conocida en línea recta.
    * Se mide el tiempo que tarda el robot en cubrir esa distancia.

3.  **Calcular la Velocidad Lineal ($v$)**:
    * La velocidad lineal ($v$) se calcula usando la distancia y el tiempo medidos:
        $v = \text{distancia} / \text{tiempo}$

4.  **Estimar la Velocidad Angular de las Ruedas ($\omega_R, \omega_L$)**:
    * Con la velocidad lineal ($v$) obtenida (correspondiente al valor de PWM usado) y conociendo el radio de las ruedas ($r$), se puede estimar la velocidad angular.
    * Si se asume que ambas ruedas giran a la misma velocidad (por ejemplo, si se aplicó el mismo PWM a ambas y el robot se movió en línea recta):
        $\omega_R = \omega_L = \frac{2v}{r}$
    

Este método proporciona una estimación de la velocidad para un nivel de PWM específico. Para conocer la velocidad a diferentes niveles de PWM, el proceso experimental debería repetirse.


## ¿Cómo factores afectan la trayectoria y velocidad del robot al cambiar los intervalos de tiempo?


* **Cálculo de la trayectoria**: Al estimar la posición futura $(x_{(k+1)}, y_{(k+1)})$, $\Delta t$ multiplica directamente el avance del robot en cada paso ($v \cos(\theta) \Delta t$ y $v \sin(\theta) \Delta t$) . Por lo tanto, cambios en $\Delta t$ alteran la longitud de cada segmento de la trayectoria calculada.

* **Precisión de la odometría**: La integración de la velocidad en el tiempo para obtener la posición (odometría) depende de $\Delta t$ como paso de integración. Un $\Delta t$ inadecuado puede reducir la precisión de la trayectoria estimada.

* **Estimación del estado**: En filtros como el de Kalman, $\Delta t$ ("intervalo de tiempo entre pasos de muestreo") se usa en la predicción del estado (ej. $x_k^- = x_{k-1} + \omega_k \cdot \Delta t$) . Cambios en $\Delta t$ afectan la exactitud de esta predicción, impactando la estimación de la orientación (trayectoria) y la velocidad.

* **Frecuencia de sensado y control**: $\Delta t$ define la frecuencia con que se leen los sensores (ej. IMU) y se actualizan los controles. Intervalos mayores pueden disminuir la capacidad del robot para seguir con precisión una trayectoria o mantener una velocidad deseada debido a una menor frecuencia de actualización.

* **Latencia**: Intervalos de tiempo ($\Delta t$) grandes pueden introducir "Latencia o desfase temporal" en sistemas de tiempo real, afectando la respuesta del robot para ajustar su velocidad y trayectoria.

## ¿Cuáles son las ventajas y desventajas de usar un IMU para ajustar la dirección en lugar de encoders?

**IMU**

* **Ventajas:**
    * Permite "medir orientación si no tienes encoders".
    * Se utiliza para "mejorar [la estimación de la] orientación".
    * En un robot diferencial, la "Velocidad angular en Z [del giroscopio de la IMU]" se usa normalmente "para estimar la orientación (Yaw $\theta$)" , lo cual es una medida directa relacionada con la dirección.

* **Desventajas:**
    * El giroscopio, que mide la velocidad angular para determinar la orientación, "deriva con el tiempo".
    * Las IMU requieren calibración para "compensar el offset de los sensores".
    * El acelerómetro (que puede usarse en la fusión de sensores para estimar ángulos) proporciona una "medición ruidosa".

**Encoders**

* **Ventajas (relacionadas con la base para calcular cambios de dirección):**
    * Miden "cuántos pasos o vueltas ha dado cada rueda", información fundamental para la odometría que calcula cambios de posición y orientación.
    * Los encoders absolutos proporcionan una "posición única para cada ángulo" y son "Ideal para robótica precisa", lo que sugiere una base fiable para calcular cambios de orientación si no hay deslizamiento.

* **Desventajas (relacionadas con el ajuste de la dirección global del robot):**
    * Los encoders incrementales "No indican posición absoluta $\rightarrow$ necesitan referencia inicial" para conocer la orientación de partida.
    * La odometría basada en encoders (que se usa para inferir la dirección) acumula errores. La IMU o el LIDAR se pueden usar para "corregir errores" de la odometría.

## ¿Qué efecto tiene la inclinación o el giro en el movimiento del robot, y cómo se corrige con el IMU?

* **Giro (Yaw):**
    * El Yaw ($\theta$) es la orientación del robot en el plano y define su dirección de movimiento
    * Un cambio en el Yaw ($\dot{\theta} \neq 0$) significa que el robot está girando, lo cual altera su trayectoria. La velocidad angular ($\omega$) del robot define cuánto rota sobre su propio eje.

* **Inclinación (Roll, Pitch):**
    * La inclinación es un cambio de postura que puede ser detectado por el acelerómetro de una IMU.
    * Afecta la "estabilidad dinámica" del robot, que es su capacidad de mantener el equilibrio mientras se mueve considerando cambios de postura.

**Cómo se Corrige con la IMU:**

* **Detección por la IMU:**
    * El **giroscopio** mide la velocidad angular en los tres ejes (roll, pitch, yaw).
    * El **acelerómetro** mide la aceleración lineal y puede detectar si el robot "se inclina".

* **Corrección del Giro (Yaw / Dirección):**
    * La "Velocidad angular en Z" medida por el giroscopio se usa para estimar la orientación (Yaw, $\theta$) del robot.
    * Esta estimación de $\theta$ se obtiene al integrar la velocidad angular Z. El valor de $\theta$ resultante se utiliza en el sistema de control y en las ecuaciones de estimación de posición para ajustar la dirección.

* **Corrección de la Inclinación y Estimación General del Ángulo:**
    * Para obtener una estimación precisa del ángulo de inclinación, se pueden combinar las mediciones del giroscopio y del acelerómetro mediante un **Filtro de Kalman**.
    * El Filtro de Kalman proporciona la mejor estimación del estado actual combinando predicción y mediciones ruidosas, permitiendo una corrección más fiable del ángulo.
    * Para la estabilidad dinámica, un "sistema de control activo del equilibrio" utiliza sensores IMU para compensar los cambios de postura como la inclinación.