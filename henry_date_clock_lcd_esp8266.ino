
/********************************/
// include the library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include "TimeDateClient.h"

/**********************************************************/
//char array1[]=" SunFounder               "; //the string to print on the LCD
//char array2[]="hello, world!             "; //the string to print on the LCD
//int tim = 500; //the value of delay time
// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

//#define PIN D5
long lastUpdate = millis();
long lastSecond = millis();

String hours, minutes, seconds, iweek, imonth, iyear, iday;
int currentSecond, currentMinute, currentHour;
int currentMonth, currentYear, currentDay;
String currentWeek;

char ssid[] = "maine coast";        //  your network SSID (name)
char pass[] = "abbyisahappygirl";   // your network password

const float UTC_OFFSET = 8;
TimeDateClient timeDateClient(UTC_OFFSET);

void updateTime() 
{
  hours = timeDateClient.getHours();
  minutes = timeDateClient.getMinutes();
  seconds = timeDateClient.getSeconds();
  iweek = timeDateClient.getWeek();
  Serial.println("this is iweek");
  Serial.println(iweek);
  iday = timeDateClient.getDay();
  imonth = timeDateClient.getMonth();
  iyear = timeDateClient.getYear();
  currentHour = hours.toInt();
  if (currentHour > 12) currentHour = currentHour - 12;
  currentMinute = minutes.toInt();
  currentSecond = seconds.toInt();
  lastUpdate = millis();
}

void setup()
{
  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) 
     {
    delay(500);
    Serial.print(".");
     }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeDateClient.updateTime();
  updateTime() ;
  lastUpdate = millis();
  lastSecond = millis();
}

void loop()
{
  if ((millis() - lastUpdate) > 1800000) updateTime();

  if ((millis() - lastSecond) > 1000)
  {
    lastSecond = millis();
    currentSecond++;
    if (currentSecond > 59)
    { currentSecond = 0;
      currentMinute++;
      if (currentMinute > 59) {
        currentMinute = 0;
        currentHour++;
        if (currentHour > 12) currentHour = 0;
      }
    }
    String currentTime = String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond);
    String currentDate = iweek + "-" + imonth + "-" + iday + "-" + iyear ;
    Serial.println(currentTime);
    Serial.println(currentDate);
    lcd.setCursor(0,0); //set cursor at column 0, line 0
    //lcd.print("It's a RED day");
    lcd.print(currentDate);
    lcd.setCursor(4,1); //sets cursor at column 4 line 1
    lcd.print(currentTime); 
    
  }
}

