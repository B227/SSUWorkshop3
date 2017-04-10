/*************************************/
/******General Overview of Code*******/
/*************************************/
/****** *Line: setup();*************/
#define TEMPPIN 0

void setup() {
  /* General Arduino Setup*/
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

int Tempuratur(){
  int tempReading = analogRead(TEMPPIN);  
  float voltage = tempReading * 5.0;
  voltage /= 1024.0;
  float temp = (voltage - 0.5) * 100;
  return temp;
}

/* Temp struct Start */
typedef struct ringbuffer {
  int buffer[200];
  int head = 0;
  int tail = 0;
  int count = 0;
}ring;
/* Temp Struct End */

/* For request function */
int     input   =   0;
int     arput   =   0;
bool    check   =   false;
bool    valid   =   false;
bool    arval   =   false;

/* Request function */
int request() {
  String inputTemp = "";
  check = false;
  valid = false;
  arval = false;
  while (Serial.available() != 0) {
    check = true;
    inputTemp += char(Serial.read());
    delay(5);
    if (Serial.available() == 0) {
      //Serial.println(inputTemp);
      input = inputTemp.toInt();
      /*check if valid for menu*/
      if (input>0&&input<5){
        valid = true;
      }
      /*check if valid for array altering*/
      if (input>0&&input<201){
        arval = true;
        arput = input;
      }
      /*invalid input for menu*/
      else{
        valid = false;
            input=0;
      }
    }
  }
  return input;
}


  /*  Mean Function */
int Mean(struct ring_buffer meannumber){
   /*  Compute  Mean */
  int addup=0;
  for(int i = 0; i < meannumber.tail; i++){
    addup += meannumber.buff[i];
  }
  addup /= meannumber.tail;
  return addup;
}
  /*  Variance Function */
int Std(struct ring_buffer std) {
  //  Define for Variance
  int sum1 = 0;
  int mean = Mean(std);
  //  Compute  Variance
  
  for (int i = 0; i < std.tail; i++)
  {
    sum1 += pow((std.buff[i] - mean), 2);
  }
  sum1 /= std.tail;
  sum1 = sqrt(sum1);
  return sum1;
}

void printData(int data){
  switch(data){
    /* Show Mean Value */
    case 1: 
    	Serial.print("Mean is: "); 
    	Serial.println(Mean(ring)); 
    	break;
    /* Show Varians Value */
  	case 2: 
    	Serial.print("Varians is: "); 
    	Serial.println(Std(ring)); 
    	break;
    /* View the buffer length */
    case 3: 
    	Serial.print("The buffer length is: "); 
    	Serial.println(ring.soft); 
    	break;
    /* Change the buffer length */
    case 4:
    	Serial.print("You have chosen to alter the buffer length. Insert new value!")
      while (arVal==false) {
        request();
        if (check == 1) {
    			if (valid == 0){
      			Serial.print("Invalid buffer length");

          }
  			}
      }
    	Serial.println("Your new value has been accepted");
    	ring.soft=arput;
    	
    	break;
    /* Default */
    default: Serial.print("Something went wrong");
  }
}

void loop() {
  /* Loop Stuff Here*/
}
/**/
