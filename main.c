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
bool    check   =   false;
bool    valid   =   false;

/* Request function */
int request() {
  String inputTemp = "";
  check = false;
  valid = false;
  while (Serial.available() != 0) {
    check = true;
    inputTemp += char(Serial.read());
    delay(5);
    if (Serial.available() == 0) {
      //Serial.println(inputTemp);
      input = inputTemp.toInt();
      if (input>0&&input<5){
        valid = true;
      }
      else{
        valid = false;
            input=0;
      }
    }
  }
  return input;
}


  /*  Mean Function */
int Mean(struct ringbuffer mean_number){
   /*  Compute  Mean */
  int addup=0;
  for(int i = mean_number.head; i <= mean_number.tail; i++){
    addup += mean_number.buffer[i];
  }
  addup /= mean_number.count;
  return addup;
}

  /*  Variance Function */
int Std(struct ringbuffer std) {
  /*  Define for Variance */
  int sum1 = 0;
  /*  Compute  Variance */
  for (int i = std.head; i < std.tail; i++)
  {
    sum1 += pow((std.buffer[i] - Mean(ring)), 2);
  }
  sum1 /= std.count;
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
    	
    	break;
    /* Default */
    default: Serial.print("Something went wrong");
  }
}

void loop() {
  /* Loop Stuff Here*/
}
/**/
