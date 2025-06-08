# Integrantes:
- Francisca Abarca P.
- Isidora Cisternas C.
- Rodolfo Fernández V.
- Valentina Guzmán E.
- Fabián Solís P.

# Rama de laboratorio 2 asignatura Robótica y sistemas autónomos

* Conexiones laboratorio 2: 

https://drive.google.com/drive/folders/1r1DnHRjlIKAMqeIxQTCVya8Y_QAMsd5l?usp=sharing


# Parte 1: Configuración del Hardware y pruebas iniciales

• Conectar los sensores ultrasóonico HC-SR04 y RGB en Arduino.

Visual:
https://drive.google.com/drive/folders/1-lmEVydz8T__cc-LVlhVatEcMLFJZqhf?usp=sharing

---

• Programar Arduino para leer la distancia con HC-SR04 y mostrarla en
el monitor serie.

Visual:

https://drive.google.com/file/d/1Dln0i0CTzF9HutFU3nLVMCJgMgfRARYl/view?usp=sharing

Código 1.2Calibracion_HC-SR04.ino

Análisis: 1.2-Programar_Arduino_para_leer_la_distancia_con_HC-SR04_y_mostrarla_en_el_monitor_serie.pdf

---

• Programar Arduino para leer los valores RGB y mostrar el color detectado.
(Lo de la calibración es un paso oculto)

Visual:

https://drive.google.com/file/d/1yw82cpVHTPf_nC-ASSIViSTHahrVLBS7/view?usp=sharing


Código: 1.3Calibracion_RGB.ino

Análisis: 1.3-Programar_Arduino_para_leer_los_valores_RGB_y_mostrar_el_color_detectado..pdf

---

• Analizar la precisón de los sensores en diferentes condiciones (luz,
superficie, distancia). 

Visual:

https://drive.google.com/drive/folders/1oTPKupgWfmSwbjtq8sOh8ZZjr274s7IM?usp=sharing

Análisis: 1.4-_Analizar_la_precisn_de_los_sensores_en_diferentes_condiciones..pdf

---

# Parte 2: Procesamiento de datos y Algoritmo de Navegación


* Aplicar umbralización al sensor ultrasónico para detectar si hay un
obstáculo a menos de 10cm.

Visual:

https://drive.google.com/file/d/1kmuZMPMnTSEb44W-o0wPDBE9jijCDrMz/view?usp=sharing

Código: Punto1_Parte2.ino

Análisis: 2.1-_aplicar_umbralizacin_al_sensor_ultrasnico_para_detectar_si_hay_un_obstculo_a_menos_de_10cm.pdf

---

* Definir umbrales para detectar colores, rojo, verde y azul usando el
sensor RGB.

Código: Punto2_Parte2.ino

Análisis: 2.2-_Definir_umbrales_para_detectar_colores_rojo_verde_y_azul_usando_el_sensor_RGB..pdf

---

* Implementar un algoritmo en Arduino que detenga el robot ante obstáculos y cambie de direcciín según el color detectado.

Visual:

https://drive.google.com/file/d/10o_qux5o9AZBja2WXdH1fjMPl_ppmOCz/view?usp=sharing

Código: Punto3_4_5_6_Parte2_Lab2.ino

Análisis: 2.3-Implementar_un_algoritmo_en_Arduino_que_detenga_el_robot_ante_obstculos_y_cambie_de_direccin_segn_el_color_detectado..pdf

---

* Probar navegación en un circuito con obstáculos y superficies en diferentes colores.

Visual:

https://drive.google.com/file/d/1TN1dWF_Sal6o7_lUwptsFynhYqnonaRr/view?usp=sharing

Código: Punto3_4_5_6_Parte2_Lab2.ino

Análisis: 2.4-Probar_navegacin_en_un_circuito_con_obstculos_y_superficies_en_diferentes_colores..pdf

---

* Ajustar parámetros para mejorar la detección y estabilidad del sistema.

Código: Punto3_4_5_6_Parte2_Lab2.ino

Análisis: 2.5-_Ajustar_parmetros_para_mejorar_la_deteccin_y_estabilidad_del_sistema..pdf
---

* Implementación de estrategias de navegación basadas en reglas.

Código: Punto3_4_5_6_Parte2_Lab2.ino

Análisis: 2.6-_Implementacin_de_estrategias_de_navegacin_basadas_en_reglas..pdf


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
