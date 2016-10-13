/* 
  This code is for an led matrix clock. The leds should be in a square with then lenth of the size being odd. The realtime clock used here is set up as a 24 hour clock.
  note 45 degree angles on the matrix are skiped on the clock. this is to evenly ditribute leds and have each tick actually represent a second
  08/29/2016
*/
#include <Adafruit_NeoPixel.h>
#include <Wire.h>// allows comunication over I2C
#include "RTClib.h"//Real time clock

#define SIZE 13 //Demention of led matrix
#define LED_COUNT pow(SIZE,2)
#define PIN 4//pin that the matrix reads
#define BACKGROUND_COLOR  0x00000//off
#define HOUR_COLOR    0x000064
#define MINUTE_COLOR  0x003200
#define SECONDS_COLOR 0x640000
#define SMILE_COLOR  0x281700
 
RTC_DS1307 RTC;
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800); // Create an instance of the Adafruit_NeoPixel class called "leds".

void setup()
{

    Serial.begin(57600);
    Wire.begin();
    RTC.begin();
 
    
    // following line sets the RTC to the date & time when this sketch was compiled 
    //It must be commented out after setting the time then reuploaded, otherwise the clock will reset if the arduino loses power
   // RTC.adjust(DateTime(__DATE__, __TIME__));

    leds.begin();  // Call this to start up the LED strip. 
}

void loop()
{
  DateTime now = RTC.now();
 /* the following section of code displays the date and time according to the real time clock.
 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
 */
   clr(); // The clr function set up the matrix. It also has to be called before any changes to the matrix to clear out the exhisting display
   sec_hand(now.second()); //The sec_hand function displays the second hand of the anolog clock
   min_hand(now.minute()); //The min_hand function displays the minute hand of the anolog clock
   hour_hand(now.hour());  //The hour_hand function displays the hour hand of the anolog clock
   if(now.minute()==0&&now.second()==0) //at the start of every hour the popup function is called
   {
     popups(now.hour());//Pop up function shows a display, curently fed hours to change the eyes based on the time
   }
    leds.show(); //The display will not change until this call is made
  
}

void sec_hand(double seconds)
{    
  int x=0, y=0, i=0, size_changed=0;
  double led_dim=0;
  for(i=0;i<SIZE-1;i=i+2) // this loop runs a total of the amount of leds from the center of the outer most square. 
  {
   size_changed=SIZE-i; // size_change is the parimiter of the square and every square inside of it
   led_dim=(size_changed-1)/2; //led_dim is the number of leds in the square to go from the center out at a 90 degree angle
    /*
      The next block of if statements handle all the parts of time. The time is divided up into the 4 15 second position (0, 15, 30, 45) each of those are purpendicular 
      lines from the center of the square to the edge. There are 8 more section (1-7, 8-14, 16-22, 23-29, 31-37, 38-44, 46-52, 53-59) The matrix is a cordinate plane when 
      the hand is between 0 and 7 y is a constant value of the highest y value possible on that particular square. This also applies for x between 8 and 22, y is the negative 
      of this value between 23 to 37, x is a negative of this value for 38 to 52 seconds, and y is this value for 53 to 59 seconds. For the value that is not a constant in each section the 
      equation is round((led_dim-1)/7*(seconds-offset)) offset is the one less than the lowest value of seconds for each section, the rest of the funtion is a ratio of leds to seconds
      per section of the square multiplied by the amount of seconds passed in that particular section. There is also a modifier of the led_dim which starts the the hand position in the right spot
      based on its last position.   
    */
    if (seconds<=7){
      x=round((led_dim-1)/7*seconds);
      y=led_dim;
    }  
    if (seconds>=8&&seconds<=14){
      x=led_dim;
      y=-round((led_dim-1)/7*(seconds-7))+led_dim; 
    }
    if (seconds==15){
      x=led_dim;
      y=0;
    }
    if (seconds>=16&&seconds<=22){
      x=led_dim;
      y=-round((led_dim-1)/7*(seconds-15)); 
    }         
     if (seconds>=23&&seconds<=29){
      x=-round((led_dim-1)/7*(seconds-22))+led_dim;
      y=-led_dim; 
    } 
    if (seconds==30){
      x=0;
      y=-led_dim;
    }
     if (seconds>=31&&seconds<=37){
      x=-round((led_dim-1)/7*(seconds-30));
      y=-led_dim; 
    }
    if (seconds>=38&&seconds<=44){
      x=-led_dim; 
      y=round((led_dim-1)/7*(seconds-37))-led_dim;
    }
     if (seconds==45){
      x=-led_dim;
      y=0;
    }
    if (seconds>=46&&seconds<=52){
      x=-led_dim;
      y=round((led_dim-1)/7*(seconds-45));
    }
    if (seconds>=53){
      x=round((led_dim-1)/7*(seconds-52))-led_dim;
      y=led_dim; 
    }
    leds.setPixelColor(plane(x,y), SECONDS_COLOR);       
 }
}

void min_hand(int minutes)
{  
  int x=0, y=0, i=0, size_changed=0, shortener=0;
  double led_dim=0;
  for(i=0;i<SIZE-3;i=i+2)
  {
   size_changed=SIZE-i; // size_change is the parimiter of the square and every square inside of it
   shortener=size_changed-1;
   led_dim=(shortener-1)/2; //led_dim is the number of leds in the square to go from the center out at a 90 degree angle
    
    if (minutes<=7){
      x=round((led_dim-1)/7*minutes);
      y=led_dim;
    }  
    if (minutes>=8&&minutes<=14){
      x=led_dim;
      y=led_dim-round((led_dim-1)/7*(minutes-7)); 
    }
    if (minutes==15){
      x=led_dim;
      y=0;
    }
    if (minutes>=16&&minutes<=22){
      x=led_dim;
      y=-round((led_dim-1)/7*(minutes-15)); 
    }         
     if (minutes>=23&&minutes<=29){
      x=led_dim-round((led_dim-1)/7*(minutes-22));
      y=-led_dim; 
    } 
    if (minutes==30){
      x=0;
      y=-led_dim;
    }
     if (minutes>=31&&minutes<=37){
      x=-round((led_dim-1)/7*(minutes-30));
      y=-led_dim; 
    }
    if (minutes>=38&&minutes<=44){
      x=-led_dim; 
      y=-led_dim+round((led_dim-1)/7*(minutes-37));
    }
     if (minutes==45){
      x=-led_dim;
      y=0;
    }
    if (minutes>=46&&minutes<=52){
      x=-led_dim;
      y=round((led_dim-1)/7*(minutes-45));
    }
    if (minutes>=53){
      x=-led_dim+round((led_dim-1)/7*(minutes-52));
      y=led_dim; 
    }
    leds.setPixelColor(plane(x,y), MINUTE_COLOR);       
 }
}

void hour_hand(int hours) 
{  
  int x=0, y=0, n=0, dim=0, i=0, side_changed, fade;
  side_changed=SIZE-4;
  if(hours>12) //This takes a 24 hours clock and and turns it to a 12 hour clock.
  {
    hours=hours-12;
  }
  if(hours==0)
  {
    hours=12;
  }
  for(i=0;i<(side_changed+1)/2;i++)
  {
    dim=side_changed-i*2;
    n=(dim-1)/2;
     if (hours==1)
     {
       y=n;
       x=(hours)*(dim*4-2)/12;
     }
     if (hours>=2&&hours<=4)
     {
       x=n;
       y=-(hours-3)*(dim*4-2)/12;
     }
     if (hours>=5&&hours<=7)
     {
       x=-(hours-6)*(dim*4-2)/12;
       y=-n;
     }
       
     if (hours>=8&&hours<=10)
     {
       x=-n;
       y= (hours-9)*(dim*4-2)/12;
     }
     if (hours>=11)
     {
       y=n;
       x=(hours-12)*(dim*4-2)/12;
     }    
  leds.setPixelColor(plane(x,y), HOUR_COLOR);     

  }    
}



int plane(int x, int y) //This function turns the matrix into a cartisean plane with the center led (0,0), n is the actual psosition of each led and led_array is the cordinate plane.
{
  int i=0, j=0, n=0, adder=0;
  int led_array[SIZE][SIZE];
    for(i=SIZE-1;i>=0;i--)
    {
       if(i%2==0) //this part acounds for stirps laid out in a z like patern (left to right then right to left)
        { 
           for(j=0;j<SIZE;j++)
           {
             led_array[i][j]=n;
             n++;
           } 
         }
       else
      { 
      
        for(j=SIZE-1;j>=0;j--)
        {
          led_array[i][j]=n;
          n++;
         }
       }
    }
    adder=(SIZE-1)/2;
    x=x+adder;
    y=y+adder;
    return (led_array[y][x]);
}

void clr()
{
  int i=0;
  for(i=0;i<LED_COUNT;i++)
  {
    leds.setPixelColor(i, BACKGROUND_COLOR);
  } 
}
void popups(int hour)
{
  clr();
  face(hour);
  leds.show();
  delay (10000);
}
void face(int hour)
{
  
  eyes( hour);
  nose();
  smile();
  
}
void smile()// upturned mouth section
{
  leds.setPixelColor(plane(0,-5),  SMILE_COLOR);
  leds.setPixelColor(plane(1,-5),  SMILE_COLOR);
  leds.setPixelColor(plane(-2,-5),  SMILE_COLOR);
  leds.setPixelColor(plane(2,-5),  SMILE_COLOR);
  leds.setPixelColor(plane(-3,-4),  SMILE_COLOR);
  leds.setPixelColor(plane(3,-4),  SMILE_COLOR);
  leds.setPixelColor(plane(-4,-3),  SMILE_COLOR);
  leds.setPixelColor(plane(4,-3),  SMILE_COLOR);
  leds.setPixelColor(plane(-1,-5),  SMILE_COLOR);
  leds.setPixelColor(plane(-5,-2),  SMILE_COLOR);
  leds.setPixelColor(plane(5,-2),  SMILE_COLOR);
}
void eyes(int hour)
{
  if(hour>7)
  {
    leds.setPixelColor(plane(-3,5),  SMILE_COLOR);
    leds.setPixelColor(plane(-3,4),  SMILE_COLOR);
    leds.setPixelColor(plane(-5,5),  SMILE_COLOR);
    leds.setPixelColor(plane(-4,4),  SMILE_COLOR);
    leds.setPixelColor(plane(-5,4),  SMILE_COLOR);
    leds.setPixelColor(plane(-4,5),  SMILE_COLOR);
    leds.setPixelColor(plane(5,5),  SMILE_COLOR);
    leds.setPixelColor(plane(4,4),  SMILE_COLOR);
    leds.setPixelColor(plane(5,4),  SMILE_COLOR);
    leds.setPixelColor(plane(4,5),  SMILE_COLOR);
    leds.setPixelColor(plane(3,5),  SMILE_COLOR);
    leds.setPixelColor(plane(3,4),  SMILE_COLOR);
   }
  leds.setPixelColor(plane(-3,3),  SMILE_COLOR);
  leds.setPixelColor(plane(-5,3),  SMILE_COLOR);
  leds.setPixelColor(plane(-4,3),  SMILE_COLOR);
  leds.setPixelColor(plane(3,3),  SMILE_COLOR);
  leds.setPixelColor(plane(5,3),  SMILE_COLOR);
  leds.setPixelColor(plane(4,3),  SMILE_COLOR);
  
}
void nose()
{
  leds.setPixelColor(plane(0,0),  SMILE_COLOR);
  leds.setPixelColor(plane(0,-1),  SMILE_COLOR);
  leds.setPixelColor(plane(1,1),  SMILE_COLOR);
  leds.setPixelColor(plane(1,0),  SMILE_COLOR);
  leds.setPixelColor(plane(1,-1),  SMILE_COLOR);
  leds.setPixelColor(plane(-1,-1),  SMILE_COLOR);
}

