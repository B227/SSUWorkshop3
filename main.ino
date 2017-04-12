/*************************************/
/******General Overview of Code*******/
/*************************************/
#define TEMPPIN 0
#define CAPACITY 200

unsigned long time = 0;

void setup() {
  /* General Arduino Setup*/
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

}

// For menu function
bool    menucheck   =   true;

// Menu function
void menu() {
  if (menucheck == true) {
    Serial.println("Welcome to the weather station");
    Serial.println("Type the appropriate number to choose one of the following options:");
    Serial.println("1 - Show Mean");
    Serial.println("2 - Show Varians");
    Serial.println("3 - View the buffer length");
    Serial.println("4 - Change the buffer length");
    menucheck = false;
  }
}

int Tempuratur() {
  int tempReading = analogRead(TEMPPIN);
  float voltage = tempReading * 5.0;
  voltage /= 1024.0;
  float temp = (voltage - 0.5) * 100;
  return temp;
}

/* Struct Start */
struct ring_buffer
{
  int buff[CAPACITY];
  int head = 0;
  int tail = 0; //pointer to head
  int soft = CAPACITY; //pointer to max (user setting)
} ring;
/* Struct End */

/* RingBuffer */
void writeToBuffer(int sensorValue, struct ring_buffer *cir) {
  int temTail = cir->tail;
  int temSoft = cir->soft;
  if (temTail < temSoft)
  {
    cir->buff[temTail] = sensorValue;
    cir->tail++;
  }
  else
  {
    cir->tail = temSoft;
    int i;
    for (i = 0; i < temSoft - 1; i++)
    {
      cir->buff[i] = cir->buff[temTail - temSoft + i + 1];
    }
    cir->buff[temSoft - 1] = sensorValue;
  }
}
/* end RingBuffer */

// For request function
int     input   =   0;
int     arput   =   0;
bool    check   =   false;
bool    valid   =   false;
bool    arval   =   false;

// Request function
int request() {
  String inputTemp = "";
  check = false;
  valid = false;
  arval = false;
  while (Serial.available() != 0) {
    
    check = true;
    inputTemp += char(Serial.read());
    Serial.println(inputTemp);
    delay(5);
    
    if (Serial.available() == 0) {
      
      //Serial.println(inputTemp);
      input = inputTemp.toInt();
      //check if valid for menu
      if (input > 0 && input < 5) {
        valid = true;
        menucheck = true;
      }
      //invalid input for menu
      else {
        valid = false;
        input = 0;
      }
      //check if valid for array altering
      if (input > 0 && input < 201) {
        arval = true;
        arput = input;
      }
    }
  }
  return input;
}



/*  Mean Function */
int Mean(struct ring_buffer meannumber) {
  // Compute  Mean
  int addup = meannumber.buff[0];
  for (int i = 1; i < meannumber.tail; i++) {
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
  //Serial.println(mean);
  //  Compute  Variance
  for (int i = 0; i < std.tail; i++)
  {
    sum1 += pow((std.buff[i] - mean), 2);
  }
  sum1 /= std.tail;
  sum1 = sqrt(sum1);
  return sum1;
}

void printData(int data) {
  switch (data) {
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
      Serial.println("You have chosen to alter the buffer length. Insert new value!");
      arval=false;
      while (arval == false) {
        request();
        
        if (check == true) {
          
          if (arval == false) {
            Serial.println("Invalid buffer length");

          }
        }
      }
      Serial.println("Your new value has been accepted");
      ring.soft = arput;

      break;
      /* Default */
      // default: Serial.print("Something went wrong");
  }
}

void loop() {
  menu();
  if (time < millis()) {
    writeToBuffer(Tempuratur(), &ring);
    time = millis();
  }
  request();
  if (valid==true) {
    printData(input);
  }

}

