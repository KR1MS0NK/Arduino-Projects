
//These are the pins from the motor driver to the arduino board
#define in1 2
#define in2 3
#define in3 4
#define in4 5

//TX is white and RX is black

//pin from the limit switch to the arduino ('Normally open' connection)
#define limit 6

int timer = 3000;
int incom = 0;

bool lastButtonState = LOW;  // Assuming active-low button
bool buttonPressed = false;
bool isMovingBackward = false;

void setup() {//setup determines the initial state of the system
  // initializes the serial read and write and allows the bluetooth to send commands
  Serial.begin(9600);

  // this part states which pins are to recieve or send signals
  pinMode (limit, INPUT_PULLUP);
  
  pinMode (in1, OUTPUT);
  pinMode (in2, OUTPUT);
  pinMode (in3, OUTPUT);
  pinMode (in4, OUTPUT);
  Serial.print("Begin \n");
}

void loop() {
  // put your main code here, to run repeatedly:
  bool currentButtonState = digitalRead(limit);
  
  if (lastButtonState == LOW && currentButtonState == LOW) {
        buttonPressed = true;
  }
  else{
    buttonPressed = false;
  }
  
  lastButtonState = currentButtonState;

  
  
  if (Serial.available() > 0){//checking for bluetooth connection
    Serial.print("CONNECTED \n"); 

    if (buttonPressed){
       Serial.print("Active \n");
    }
    else {
       Serial.print("Falling \n");
        hold();
    
    }
    
    incom = Serial.read();
    Serial.print("value ="); 
    Serial.print(incom);
    Serial.print("\n");

    if (incom == 49){// forward signal
      forward();
      isMovingBackward = false;
    }
    else if (incom ==50){// reverse signal
      isMovingBackward = true;
      back();
    }
    else if (incom ==51){// stop signal
      isMovingBackward = false;
      hold();
    }
    else if (incom ==52){// automatic run signal
      autom();
    }
  }

  if (isMovingBackward && !buttonPressed) {
    Serial.println("Limit switch opened - stopping motors");
    hold();
  }
}

void forward(){
  Serial.print("moving forward \n"); 
  digitalWrite (in1, HIGH);
  digitalWrite (in2, LOW);
  digitalWrite (in3, HIGH);
  digitalWrite (in4, LOW);
  }

void hold(){
  
    Serial.print("stopped \n"); 
    digitalWrite (in1, LOW);
    digitalWrite (in2, LOW);
    digitalWrite (in3, LOW);
    digitalWrite (in4, LOW);

}

void back(){
  if (!buttonPressed){
    hold();
  }
  else{
    Serial.print("moving back \n"); 
    digitalWrite (in1, LOW);
    digitalWrite (in2, HIGH);
    digitalWrite (in3, LOW);
    digitalWrite (in4, HIGH);
  }
}

void autom(){
  Serial.print("Auto \n"); 
  forward();
  delay(timer);
  hold();
  delay(50);
  back();
  delay(timer);
  hold();

  }
