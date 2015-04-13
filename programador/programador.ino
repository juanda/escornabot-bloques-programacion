/*
 Programador por bloques para el escornabot.
*/

#define MIN_DIGITAL_OUT 3
#define MAX_DIGITAL_OUT 5
#define MIN_ANALOG_IN 0
#define MAX_ANALOG_IN 0
#define NUM_ANALOG_IN   MAX_ANALOG_IN - MIN_ANALOG_IN + 1
#define NUM_DIGITAL_OUT MAX_DIGITAL_OUT - MIN_DIGITAL_OUT + 1 

int btnReadProg    = 2;
int leerPrograma   = 0;
int timeStab       = 500; // tiempo que dejaremos pasar para que se
                          // estabilicen las entradas analogicas
char programa[NUM_ANALOG_IN][NUM_DIGITAL_OUT];


void setup(){
  pinMode(btnReadProg, INPUT);
  
  // Definimos los pines del MIN_DIGITAL_OUT al MAX_DIGITAL_OUT
  // de salida y colocamos 0 Volts en ellos
  for(int pin = MIN_DIGITAL_OUT; pin <= MAX_DIGITAL_OUT; pin++){
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
  // HIGH. La cosa es que la condicion se cumpla cuando se
  // ha pulsado el boton de lectura del programa 
  if(leerPrograma == HIGH){    
    for(int pin = MIN_DIGITAL_OUT; pin <= MAX_DIGITAL_OUT; pin++){
      // Encendemos la salida digital dada por 'pin'
      nextOutputConfiguration(pin);
      delay(timeStab); // Esperamos un ratito
      // leemos todas las entradas analogicas
      for(int a = MIN_ANALOG_IN; a <= MAX_ANALOG_IN; a++){
        programa[a-MIN_ANALOG_IN][pin-MIN_DIGITAL_OUT] = decodeVoltageLevel(analogRead(a));
        dump(a, pin);
        
      }
    }
    // Ahora lanzamos el programa al monitor serial. En su momento
    // habra que hacerlo al bluetooth.
    sendToSerial();
    sendToBot();
       
  }  

}

// Esta funcion coloca todos los pines digitales de salida a nivel
// 0, a excepcion del pin dado por el argumento 'pin', que lo 
// coloca a nivel alto
void nextOutputConfiguration(int pin){
  for(int i = MIN_DIGITAL_OUT; i <= MAX_DIGITAL_OUT; i++){
    if(i == pin) digitalWrite(i, HIGH); else digitalWrite(i, LOW);
  }
}

// voltageLevel puede tomar valores comprendidos entre 0 y 1023
char decodeVoltageLevel(int voltageLevel){

  char result;
  
  if(voltageLevel >= 0  && voltageLevel <= 260)   result = 'N'; // NORTE
  if(voltageLevel >= 261 && voltageLevel <= 350)  result = 'E'; // ESTE
  if(voltageLevel >= 351 && voltageLevel <= 500)  result = 'S'; // SUR
  if(voltageLevel >= 501 && voltageLevel <= 1023) result = 'W'; // OESTE

  dumpResult(voltageLevel, result);
  
  return result;
}

void sendToSerial(){
  Serial.println("Programa");
  Serial.println('R'); // Reset
  for(int a = MIN_ANALOG_IN; a <= MAX_ANALOG_IN; a++){
    for(int pin = MIN_DIGITAL_OUT; pin <= MAX_DIGITAL_OUT; pin++){               
      Serial.println(programa[a-MIN_ANALOG_IN][pin-MIN_DIGITAL_OUT]);
    }      
  } 
  Serial.println('G'); // Go
  Serial.println("End Programa");
}

void sendToBot(){
  // send 'R' (reset)
  // send programa
  // send 'G' (go)
}

void dump(int a, int pin){
  Serial.println("Lectura");
  Serial.print(a);
  Serial.print("\t");
  Serial.print(pin);
  Serial.print("\t");
  Serial.println(programa[a-MIN_ANALOG_IN][pin-MIN_DIGITAL_OUT]);
  Serial.println("End Lectura");
  
}

void dumpResult(int vl, char r){
  Serial.println("Result");
  Serial.print(vl);
  Serial.print("\t");
  Serial.println(r);
  Serial.println("End Result");
}
