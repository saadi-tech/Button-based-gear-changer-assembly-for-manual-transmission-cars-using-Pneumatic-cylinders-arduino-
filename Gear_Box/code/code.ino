
int gear_leds[] = {7,8,9,10,11,12,13};
float X = 0;
float Y = 0; 


float neutral_pt_y = 25;
float neutral_pt_x = 25;
float high_x = 40;
float low_x = 10;
float high_y = 40;
float low_y = 10; 

int tolerance = 2;
int up = 2;
int down = 3;
int option = 4;

int set_button = A5;


void setup() {
  // put your setup code here, to run once:
for (int i = 0; i < 7 ; i++){
  pinMode(gear_leds[i],OUTPUT);
}

pinMode(up,INPUT);
pinMode(down,INPUT);
pinMode(option,INPUT);
pinMode(A0,INPUT);   //X sensor
pinMode(A1,INPUT);   //Y sensor
Serial.begin(9600);
}



void x_up(){
  Serial.println("X++");
}
void x_down(){
  Serial.println("X--");
}

void y_up(){
  Serial.println("Y++");
}
void y_down(){
  Serial.println("Y--");
}



///========================================================
float get_reading(char axis){
  int reading = 0;

  if (axis == 'x'){
    reading = analogRead(A0);
  }
  else{
    reading = analogRead(A1);
  }

  float result = float(reading)*75.0 / 1024.0;
  return result;
}

float get_target(int gear,int xy){

  if (xy == 0){
    //X

    if (gear == 1 || gear == 2){
      return high_x;
    }
    if (gear == 3 || gear == 4){
      return neutral_pt_x;
    }

    if (gear == 5 || gear == 6){
      return low_x;
    }
    if (gear == 0){
      return neutral_pt_x;
    }
  }

  if (xy == 1){
    //Y

     if (gear == 0 ){
      return neutral_pt_y;
     }
     if (gear == 1 || gear == 3 || gear == 5 ){
      return high_y;
    }
    if (gear == 2 || gear == 4 || gear == 6){
      return low_y;
    }

    
  }
}
//=============================================================
void move_gear(int gear){
 //setting Y
  float tx = get_target(gear,0);
  float ty = get_target(gear,1);

  
  //setting Y first...
  Serial.println("Changing Y..");
  while( abs(Y - neutral_pt_y) > tolerance){
    
    if(Y< neutral_pt_y){
      //Y++;
      Serial.println(neutral_pt_y - Y);
      y_up();
      
    }
    if(Y>neutral_pt_y){
      //Y--;
      Serial.println(neutral_pt_y - Y);
      y_down();
      
    }

    
Y = get_reading('y');
  }

  
  Serial.println("");

 //setting X..
Serial.println("Changing X..");
  while( abs(X - tx) > tolerance ){
    if(X < tx){
      //X++;
      Serial.println(tx - X);
      x_up();
      
    }
    if(X > tx){
      //X--;
      Serial.println(tx - X);
      x_down();
      
    }

  X = get_reading('x');  

  }
    Serial.println("");
    //setting Y....

    Serial.println("Changing Y..");
  while( abs(Y - ty) > tolerance ){
    if(Y < ty){
      //Y++;
      Serial.println(ty - Y);
      y_up();
      
    }
    if(Y > ty){
      //Y--;
      Serial.println(ty - Y);
      y_down();
      
    }
Y = get_reading('y');
   
  }

   Serial.println("");
 
 

  }
//=============================================================
int get_gear(float X,float Y){

  if ( abs(Y - neutral_pt_y) < tolerance){
    //NEUTRAL GEAR
    return 0;
  }


  if ( abs(X - high_x)< tolerance){

    //left most COLUMN
    if( abs(Y - high_y) < tolerance){
      //upper...
      return 1;
    }

    if (abs(Y - low_y)< tolerance){
      //lower..
      return 2;
    }
  }



  if ( abs(X - neutral_pt_x)< tolerance){

    //middle COLUMN
    if( abs(Y - high_y) < tolerance){
      //upper...
      return 3;
    }

    if (abs(Y - low_y)< tolerance){
      //lower..
      return 4;
    }
  }




  if ( abs(X - low_x)< tolerance){

    //right most COLUMN
    if( abs(Y - high_y) < tolerance){
      //upper...
      return 5;
    }

    if (abs(Y - low_y)< tolerance){
      //lower..
      return 6;
    }
  }
  
}

//==============================================================================
void light_up(int gear){
  for (int i = 0;i<7;i++){
    digitalWrite(gear_leds[i],LOW);
  }
  digitalWrite(gear_leds[gear],HIGH);

Serial.print("Current Gear: ");

if (gear!=0 || gear !=6){
  Serial.println(gear);
}
else{

  if (gear == 0){
    Serial.println(0);
  }
  else{
    Serial.println("R");
  }
}

  
}




void set_neutral(){
  Serial.println("Setting neutral...");
  move_gear(0);
}

//==============================================================================
void loop() {
  // put your main code here, to run repeatedly:


   //reading both axis reading...
   X = get_reading('x');
   Y = get_reading('y');
   Serial.print("Current position: (X,Y) : ");
   Serial.print(X);
   Serial.print(",");
   Serial.println(Y);
   set_neutral();
   int gear = get_gear(X,Y);
   light_up(gear);
   
   


while(1){
  if (digitalRead(up) && gear <5){
    gear++;
    move_gear(gear);
    light_up(gear);
   Serial.print("Current X,Y: ");
    Serial.print(X);
    Serial.print(",");
    Serial.println(Y);
   delay(250); 
  }

    if (digitalRead(down) && gear >=0){
    if (gear <= 0){
      gear = 6;
    }
    else{
    gear--;
    }
    move_gear(gear);
    light_up(gear);
    Serial.print("Current X,Y: ");
    Serial.print(X);
    Serial.print(",");
    Serial.println(Y);
    delay(250);
  }
    
    
    
    
  }

}
