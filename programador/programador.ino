int btnReadProg    = 2;
int leerPrograma   = 0;
int minDigitalOut  = 3;
int maxDigitalOut  = 13;
int minAnalogIn    = 0;
int maxAnalogIn    = 5;
int timeStab       = 500; // tiempo que dejaremos pasar para que se
                          // estabilice las entradas analogicas

// Desgraciadamente en C/C++ para declarar un array de tamaño constante
// hay que saber en tiempo de compilacion el tamaño de este, por lo que
// no podemos declarar las siguientes definiciones como variables.
#define NUM_ANALOG_IN 6     // Debe ser maxAnalogIn - minAnalogIn + 1
#define NUM_DIGITAL_OUT 11  // Debe ser maxDigitalOut - minDigitalOut + 1;      

char programa[NUM_ANALOG_IN][NUM_DIGITAL_OUT];


void setup(){
  pinMode(btnReadProg, INPUT);
  
  // Definimos los pines del 3 al 13 como de salida y colocamos 
  // 0 Volts en ellos
  for(int pin = minDigitalOut; pin <= maxDigitalOut; pin++){
    pinMode(pin, OUTPUT);
    digitalWrite(pin,LOW);
  }
  
  // Utilizaremos el monitor serial para volcar el contenido
  // del programa (depuracion)
  Serial.begin(9600);
}

void loop(){
  leerPrograma = digitalRead(btnReadProg);
  
  // Si el pulsador de lectura de programa es normalmente
  // cerrado hay que poner LOW, si es normalmente abierto
  // HIGH. La cosa es que la condicion se cumple cuando se
  // ha pulsado el boton de lectura del programa 
  if(leerPrograma == LOW){    
    for(int pin = minDigitalOut; pin <= maxDigitalOut; pin++){
      // Encendemos la salida digital dada por 'pin'
      nextOutputConfiguration(pin);
      delay(timeStab); // Esperamos un ratito
      // leemos todas las entradas analogicas
      for(int a = minAnalogIn; a <= maxAnalogIn; a++){
        programa[a][pin] = decodeVoltageLevel(analogRead(a));
      }      
    }    
  }
  
  // Ahora lanzamos el programa al monitor serial. En su momento
  // habra que hacerlo al bluetooth.
  for(int a = minAnalogIn; a <= maxAnalogIn; a++){
    for(int pin = minDigitalOut; pin <= maxDigitalOut; pin++){            
        Serial.println(programa[a][pin]);
      }      
    }
}

// Esta funcion coloca todos los pines digitales de salida a nivel
// 0, a excepcion del pin dado por el argumento 'pin', que lo 
// coloca a nivel alto
void nextOutputConfiguration(int pin){
  for(int i = minDigitalOut; i <= maxDigitalOut; i++){
    if(i == pin) digitalWrite(i, HIGH); else digitalWrite(i, LOW);
  }
}

// voltageLevel puede tomar valores comprendidos entre 0 y 1023
char decodeVoltageLevel(int voltageLevel){
  
  char result;
  
  if(voltageLevel >= 0   && voltageLevel <= 204)  result = 'S'; // STOP
  if(voltageLevel >= 205 && voltageLevel <= 408)  result = 'A'; // AVANZA
  if(voltageLevel >= 409 && voltageLevel <= 612)  result = 'R'; // RETROCEDE
  if(voltageLevel >= 613 && voltageLevel <= 816)  result = 'D'; // GIRO DERECHA
  if(voltageLevel >= 817 && voltageLevel <= 1023) result = 'I'; // GIRO IZQUIERDA  
  
  return result;
}
