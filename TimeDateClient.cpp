/**The MIT License (MIT)
Copyright (c) 2015 by Daniel Eichhorn
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
See more at http://blog.squix.ch
*/

#include "TimeDateClient.h"

String parsedWeek = "  ";
String parsedMonth = "  ";
int parsedDay = 0;
int parsedYear = 0;

TimeDateClient::TimeDateClient(float utcOffset) {
  myUtcOffset = utcOffset;
}

void TimeDateClient::updateTime() {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("www.google.com", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // This will send the request to the server
  client.print(String("GET / HTTP/1.1\r\n") +
               String("Host: www.google.com\r\n") +
               String("Connection: close\r\n\r\n"));
  int repeatCounter = 0;
  while(!client.available() && repeatCounter < 10) {
    delay(1000);
    Serial.println(".");
    repeatCounter++;
  }

  String line;

  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    while((size = client.available()) > 0) {
      line = client.readStringUntil('\n');
      line.toUpperCase();
      // example:
      // date: Thu, 19 Nov 2015 20:25:40 GMT
      // 1234567890123456789012345678901234567890
      //          1         2         3         4
      if (line.startsWith("DATE: ")) {
        Serial.println(line.substring(23, 25) + ":" + line.substring(26, 28) + ":" +line.substring(29, 31));
        int parsedHours = line.substring(23, 25).toInt();
        int parsedMinutes = line.substring(26, 28).toInt();
        int parsedSeconds = line.substring(29, 31).toInt();
        Serial.println(line.substring(6, 9) + "-" +line.substring(11, 13));
        parsedWeek = line.substring(6, 9);
        Serial.println("if statment in cpp");
        parsedDay = line.substring(11, 13).toInt();
        parsedMonth = line.substring(14, 17);
        parsedYear = line.substring(18, 22).toInt();
        Serial.println(String(parsedHours) + ":" + String(parsedMinutes) + ":" + String(parsedSeconds));
        //this works example "SUN3SEP2017"
        Serial.println(parsedWeek + ":" + String(parsedDay) + ":" + parsedMonth + ":" + String(parsedYear));

        localEpoc = (parsedHours * 60 * 60 + parsedMinutes * 60 + parsedSeconds);
        Serial.println(localEpoc);
        localMillisAtUpdate = millis();
      }
    }
  }

}
String TimeDateClient::getWeek() {
      Serial.println("this is parsedWeek");
      return parsedWeek;
}
String TimeDateClient::getMonth() {
      return parsedMonth;
}
String TimeDateClient::getDay() {
    return String(parsedDay);
}
String TimeDateClient::getYear() {
    return String(parsedYear);
}

String TimeDateClient::getHours() {
    if (localEpoc == 0) {
      return "--";
    }
    int hours = ((getCurrentEpochWithUtcOffset()  % 86400L) / 3600) % 24;
    if (hours < 10) {
      return "0" + String(hours);
    }
    return String(hours); // print the hour (86400 equals secs per day)

}
String TimeDateClient::getMinutes() {
    if (localEpoc == 0) {
      return "--";
    }
    int minutes = ((getCurrentEpochWithUtcOffset() % 3600) / 60);
    if (minutes < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      return "0" + String(minutes);
    }
    return String(minutes);
}
String TimeDateClient::getSeconds() {
    if (localEpoc == 0) {
      return "--";
    }
    int seconds = getCurrentEpochWithUtcOffset() % 60;
    if ( seconds < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      return "0" + String(seconds);
    }
    return String(seconds);
}

String TimeDateClient::getFormattedTime() {
  return getHours() + ":" + getMinutes() + ":" + getSeconds();
}

long TimeDateClient::getCurrentEpoch() {
  return localEpoc + ((millis() - localMillisAtUpdate) / 1000);
}

long TimeDateClient::getCurrentEpochWithUtcOffset() {
  return round(getCurrentEpoch() + 3600 * myUtcOffset + 86400L) % 86400L;
}
