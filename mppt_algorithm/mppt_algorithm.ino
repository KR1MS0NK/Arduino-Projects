
// variables
int pwr, v, I, T;
int pwrT = 0;


void sleep() {  //rest values
  delay(1000 * T);
}

void climb() {

}

void drop() {

}

void setup() {
  Serial.begin(9600);

}

void loop() {
  //  v = voltageSensor;
  //  I = CurrentSensor;
  pwr = v * I;

  // display values
  Serial.print("v= ");
  Serial.print( v);
  Serial.print(", I= ");
  Serial.print(I);
  Serial.print(", pwr= ");
  Serial.print(pwr);
  Serial.print(", pwrT= ");
  Serial.println(pwrT);

  if (pwrT < 0.995 * pwr ) {
    climb();
  }
  else if (pwrT > 0.995 * pwr ) {
    drop();
  }
  else{
    sleep();
  }
  pwrT = pwr;

 }
