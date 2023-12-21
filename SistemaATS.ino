int decimalPrecision = 2;
const int RelePrincipalPin = 2;  
const int ReleReservaPin = 3;
const int LedF1Pin = 4; 
const int LedF2Pin = 5;  
int VoltageAnalogInputPin = A0;
float LecturaMuestra = 0;
float UltimaLecturaMuestra = 0;
float SumaLecturaMuestra = 0;
float ContadorLecturaMuestra = 0;
float PromedioLectura;
float PromedioVoltajeRMS;
float AjustePromedioVoltajeRMS;
float VoltajeRMSFinal;
float VoltajeRMSAnterior = 0.00;
float Diferencia = 40;
float voltageOffset1 = 0.00;
float voltageOffset2 = 0.00;

void setup() {
    Serial.begin(9600);
    pinMode(RelePrincipalPin, OUTPUT);
    pinMode(ReleReservaPin, OUTPUT);
    pinMode(LedF1Pin, OUTPUT);
    pinMode(LedF2Pin, OUTPUT);
    digitalWrite(RelePrincipalPin, HIGH);
    digitalWrite(ReleReservaPin, HIGH);

int LecturaInicial = analogRead(VoltageAnalogInputPin);
float VoltajeInicial = (LecturaInicial - 512) + voltageOffset1;
float VoltajeRMSInicial = sqrt(sq(VoltajeInicial));

if (VoltajeRMSInicial >= 180 && VoltajeRMSInicial <= 240) {
    Serial.println("Voltaje normal");
    digitalWrite(ReleReservaPin, HIGH); 
    digitalWrite(LedF2Pin, HIGH);
    delay(3000);
    digitalWrite(RelePrincipalPin, LOW);
    digitalWrite(LedF1Pin, LOW);
} else if (VoltajeRMSInicial >= 0 && VoltajeRMSInicial <= 100) {
    Serial.println("Apagon electrico, cambiando de fase");
    digitalWrite(RelePrincipalPin, HIGH);  
    digitalWrite(LedF1Pin, HIGH);
    delay(3000);
    digitalWrite(ReleReservaPin, LOW);
    digitalWrite(LedF2Pin, LOW);
  }
}

void loop() {
  
if (micros() >= UltimaLecturaMuestra + 1000) {
    LecturaMuestra = (analogRead(VoltageAnalogInputPin) - 512) + voltageOffset1;
    SumaLecturaMuestra = SumaLecturaMuestra + sq(LecturaMuestra);
    ContadorLecturaMuestra = ContadorLecturaMuestra + 1;
    UltimaLecturaMuestra = micros();
}

if (ContadorLecturaMuestra == 1000) {
    PromedioLectura = SumaLecturaMuestra / ContadorLecturaMuestra;
    PromedioVoltajeRMS = sqrt(PromedioLectura) * 1.5;
    AjustePromedioVoltajeRMS = PromedioVoltajeRMS + voltageOffset2;
    VoltajeRMSFinal = PromedioVoltajeRMS + voltageOffset2;
    
SumaLecturaMuestra = 0;
ContadorLecturaMuestra = 0; 

if (abs(VoltajeRMSFinal - VoltajeRMSAnterior) > Diferencia) {

if (VoltajeRMSFinal >= 180 && VoltajeRMSFinal <= 240) {
    Serial.println("Voltaje normal");
    digitalWrite(ReleReservaPin, HIGH); 
    digitalWrite(LedF2Pin, HIGH);
    delay(3000);
    digitalWrite(RelePrincipalPin, LOW);
    digitalWrite(LedF1Pin, LOW);
  } else if (VoltajeRMSFinal >= 0 && VoltajeRMSFinal <= 100) {
    Serial.println("Apagon electrico, cambiando de fase");
    digitalWrite(RelePrincipalPin, HIGH);  
    digitalWrite(LedF1Pin, HIGH);
    delay(3000);
    digitalWrite(ReleReservaPin, LOW);
    digitalWrite(LedF2Pin, LOW);     
  }
}

VoltajeRMSAnterior = VoltajeRMSFinal;

    }
  }
