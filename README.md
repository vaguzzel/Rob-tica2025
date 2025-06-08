# Integrantes:
- Francisca Abarca P.
- Isidora Cisternas C.
- Rodolfo Fernández V.
- Valentina Guzmán E.
- Fabián Solís P.

# Rama de laboratorio 2 asignatura Robótica y sistemas autónomos

# Parte 1: Configuración del Hardware y pruebas iniciales

• Conectar los sensores ultrasóonico HC-SR04 y RGB en Arduino.

https://drive.google.com/drive/folders/1-lmEVydz8T__cc-LVlhVatEcMLFJZqhf?usp=sharing


---

• Programar Arduino para leer la distancia con HC-SR04 y mostrarla en
el monitor serie.

https://drive.google.com/file/d/1Dln0i0CTzF9HutFU3nLVMCJgMgfRARYl/view?usp=sharing

Código 1.2Calibracion_HC-SR04.ino

Análisis: 1.2-Programar Arduino para leer la distancia con HC-SR04 y mostrarla en
el monitor serie.

---

• Programar Arduino para leer los valores RGB y mostrar el color detectado.
(Lo de la calibración es un paso oculto)

https://drive.google.com/file/d/1yw82cpVHTPf_nC-ASSIViSTHahrVLBS7/view?usp=sharing


Código: 1.3Calibracion_RGB.ino

---

• Analizar la precisón de los sensores en diferentes condiciones (luz,
superficie, distancia). FALTA

https://drive.google.com/drive/folders/1oTPKupgWfmSwbjtq8sOh8ZZjr274s7IM?usp=sharing

Análisis: .pdf
---

# Parte 2: Procesamiento de datos y Algoritmo de Navegación


* Aplicar umbralización al sensor ultrasónico para detectar si hay un
obstáculo a menos de 10cm.

https://drive.google.com/file/d/1kmuZMPMnTSEb44W-o0wPDBE9jijCDrMz/view?usp=sharing

Código: Punto1_Parte2.ino

Análisis: .pdf
---

* Definir umbrales para detectar colores, rojo, verde y azul usando el
sensor RGB.

**AQUÍ VOY AQUÍ VOY**


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
