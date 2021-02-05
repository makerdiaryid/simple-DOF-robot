#include <ESP8266WiFi.h>
#include <string.h>
#include "Servo.h"
 
const char* ssid = "nama_wifi";
const char* password = "password_wifi";
 
int ledPin = 2;
WiFiServer server(80);

//servo
int clawpin = 13;
int arm4pin = 12;
int arm3pin = 14;
int arm2pin = 0;
int arm1pin = 4;
int pivotpin = 15;
Servo clawservo,arm4servo,arm3servo,arm2servo,arm1servo,pivotservo;
int clawpos = 90;
int arm4pos = 90;
int arm3pos = 90;
int arm2pos = 90;
int arm1pos = 90;
int pivotpos = 91;
int hclawpos = clawpos;
int harm4pos = arm4pos;
int harm3pos = arm3pos;
int harm2pos = arm2pos;
int harm1pos = arm1pos;
int hpivotpos = pivotpos;
 
void setup() {
  Serial.begin(115200);
  delay(10);
  clawservo.attach(clawpin);
  arm4servo.attach(arm4pin);
  arm3servo.attach(arm3pin);
  arm2servo.attach(arm2pin);
  arm1servo.attach(arm1pin);
  pivotservo.attach(pivotpin);
  
 
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  //getvalue
  //Serial.println(getValue(request, 'x', 1));
  if (request.indexOf("x") != -1){
    hclawpos = clawpos;
    harm1pos = arm1pos;
    harm2pos = arm2pos;
    harm3pos = arm3pos;
    harm4pos = arm4pos;
    hpivotpos = pivotpos;
    clawpos = getValue(request, 'x', 1).toInt();
    arm1pos = getValue(request, 'x', 2).toInt();
    arm2pos = getValue(request, 'x', 3).toInt();
    arm3pos = getValue(request, 'x', 4).toInt();
    arm4pos = getValue(request, 'x', 5).toInt();
    pivotpos = getValue(request, 'x', 6).toInt();
    String logtempos=String("CLAW : ")+clawpos+", ARM1 : "+arm1pos+", ARM2 : "+arm2pos+", ARM3 : "+arm3pos+", ARM4 : "+arm4pos+", PIVOT : "+pivotpos;
    Serial.println(logtempos);
   // clawservo.write(clawpos);
    clawservomove(clawpos);
    delay(100);
    //arm4servo.write(arm4pos);
    arm4servomove(arm4pos);
    delay(100);
    //arm3servo.write(arm3pos);
    arm3servomove(arm3pos);
    delay(100);
    //arm2servo.write(arm2pos);
    arm2servomove(arm2pos);
    delay(100);
    //arm1servo.write(arm1pos);
    arm1servomove(arm1pos);
     delay(100);
    //pivotservo.write(pivotpos);
    pivotservomove(pivotpos);
  }

  if(request.indexOf("y") != -1){
    neutralpos();
  }

  if(request.indexOf("z") != -1){
    demo1();
  }

   if(request.indexOf("q") != -1){
    demo2();
  }

 
 
 
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
 client.println("<script type='text/javascript'>");
  client.println("function go(){");
  client.println("location = 'okx'+document.getElementById('clawpos').value+'x'+document.getElementById('arm1pos').value+'x'+document.getElementById('arm2pos').value+'x'+document.getElementById('arm3pos').value+'x'+document.getElementById('arm4pos').value+'x'+document.getElementById('pivotpos').value;");
  client.println("}");
  client.println("function neutralpos(){");
  client.println("location = 'okyaction1';");
  client.println("}");
  client.println("function demo1(){");
  client.println("location = 'okzaction1';");
  client.println("}");
   client.println("function demo2(){");
  client.println("location = 'okqaction1';");
  client.println("}");
  
  client.println("function updateval(){");
  client.println("  document.getElementById('clawcurrentval').innerHTML=document.getElementById('clawpos').value;");
  client.println("  document.getElementById('arm1currentval').innerHTML=document.getElementById('arm1pos').value;");
  client.println("  document.getElementById('arm2currentval').innerHTML=document.getElementById('arm2pos').value;");
  client.println("  document.getElementById('arm3currentval').innerHTML=document.getElementById('arm3pos').value;");
  client.println("  document.getElementById('arm4currentval').innerHTML=document.getElementById('arm4pos').value;");
  client.println("  document.getElementById('pivotcurrentval').innerHTML=document.getElementById('pivotpos').value;");
  client.println("}");
  client.println("</script>");

  
  
  client.println("<body style='font-family:  Arial'>");
  client.println("CLAW :<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>CLOSE<input type='range' min='70' max='130' value='")+clawpos+"' style='width: 80%;' id='clawpos' onchange='updateval();'>OPEN</td><td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='clawcurrentval'>"+clawpos+"</td></tr></table>");
  client.println("<br><br>ARM 1 :<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>UP<input type='range' min='30' max='130' value='")+arm1pos+"' style='width: 80%;' id='arm1pos' onchange='updateval();'>DOWN</td><td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='arm1currentval'>"+arm1pos+"</td></tr></table>");
  client.println("<br><br>ARM 2 :<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>DOWN<input type='range' min='1' max='180' value='")+arm2pos+"' style='width: 80%;' id='arm2pos' onchange='updateval();'>UP<td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='arm2currentval'>"+arm2pos+"</td></tr></table>");
  client.println("<br><br>ARM 3:<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>DOWN<input type='range' min='1' max='180' value='")+arm3pos+"' style='width: 80%;' id='arm3pos' onchange='updateval();'>UP<td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='arm3currentval'>"+arm3pos+"</td></tr></table>");
  client.println("<br><br>ARM 4 :<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>DOWN<input type='range' min='10' max='180' value='")+arm4pos+"' style='width: 80%;' id='arm4pos' onchange='updateval();'>UP<td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='arm4currentval'>"+arm4pos+"</td></tr></table>");
  client.println("<br><br>PIVOT :<br>");
  client.println(String("<table width='100%'><tr><td width='90%'>RIGHT<input type='range' min='40' max='155' value='")+pivotpos+"' style='width: 80%;' id='pivotpos' onchange='updateval();'>LEFT<td width='10%' style='background-color:#000000;color:#FFFFFF;' align='center' id='pivotcurrentval'>"+pivotpos+"</td></tr></table>");
  client.println("<br><input type='button' value='EXECUTE' style='font-size:40px' onclick='go();'><br><br><input type='button' value='NEUTRAL' style='font-size:40px' onclick='neutralpos();'><br><br><input type='button' value='DEMO1' style='font-size:40px' onclick='demo1();'><br><br><input type='button' value='DEMO2' style='font-size:40px' onclick='demo2();'>");
  client.println("</body>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void arm1servomove(int servogotopos){
  if(harm1pos<servogotopos){
    for(int i=harm1pos ; i<=servogotopos ;i++){
      arm1servo.write(i);
      delay(30);
    }
  }if(harm1pos>servogotopos){
    for(int i=harm1pos ; i>=servogotopos ;i--){
       arm1servo.write(i);
      delay(30);
    }
  }if(harm1pos==servogotopos){
       arm1servo.write(servogotopos);
  }
  harm1pos=servogotopos;
}

void arm2servomove(int servogotopos){
  if(harm2pos<servogotopos){
    for(int i=harm2pos ; i<=servogotopos ;i++){
      arm2servo.write(i);
      delay(30);
    }
  }if(harm2pos>servogotopos){
    for(int i=harm2pos ; i>=servogotopos ;i--){
       arm2servo.write(i);
      delay(30);
    }
  }if(harm2pos==servogotopos){
       arm2servo.write(servogotopos);
  }
  harm2pos=servogotopos;
}

void arm3servomove(int servogotopos){
  if(harm3pos<servogotopos){
    for(int i=harm3pos ; i<=servogotopos ;i++){
      arm3servo.write(i);
      delay(30);
    }
  }if(harm3pos>servogotopos){
    for(int i=harm3pos ; i>=servogotopos ;i--){
       arm3servo.write(i);
      delay(30);
    }
  }if(harm3pos==servogotopos){
       arm3servo.write(servogotopos);
  }
  harm3pos=servogotopos;
}

void arm4servomove(int servogotopos){
  if(harm4pos<servogotopos){
    for(int i=harm4pos ; i<=servogotopos ;i++){
      arm4servo.write(i);
      delay(30);
    }
  }if(harm4pos>servogotopos){
    for(int i=harm4pos ; i>=servogotopos ;i--){
       arm4servo.write(i);
      delay(30);
    }
  }if(harm4pos==servogotopos){
       arm4servo.write(servogotopos);
  }
  harm4pos=servogotopos;
}

void pivotservomove(int servogotopos){
  if(hpivotpos<servogotopos){
    for(int i=hpivotpos ; i<=servogotopos ;i++){
      pivotservo.write(i);
      delay(30);
    }
  }if(hpivotpos>servogotopos){
    for(int i=hpivotpos ; i>=servogotopos ;i--){
       pivotservo.write(i);
      delay(30);
    }
  }if(hpivotpos==servogotopos){
       pivotservo.write(servogotopos);
  }
  hpivotpos=servogotopos;
}

void clawservomove(int servogotopos){
  if(hclawpos<servogotopos){
    for(int i=hclawpos ; i<=servogotopos ;i++){
      clawservo.write(i);
      delay(30);
    }
  }if(hclawpos>servogotopos){
    for(int i=hclawpos ; i>=servogotopos ;i--){
       clawservo.write(i);
      delay(30);
    }
  }if(hclawpos==servogotopos){
       clawservo.write(servogotopos);
  }
  hclawpos=servogotopos;
}

void neutralpos(){
  clawpos = 90;
  arm4pos = 177;
  arm3pos = 7;
  arm2pos = 20;
  arm1pos = 38;
  pivotpos = 91;
    arm1servomove(arm1pos);
    delay(100);
    clawservomove(clawpos);
    delay(100);
    arm4servomove(arm4pos);
    delay(100);
    arm3servomove(arm3pos);
    delay(100);
    arm2servomove(arm2pos);
    delay(100);
    pivotservomove(pivotpos);
}

void setobj1(){
 clawservo.write(85);
}

void setobj2(){
  
}

void demo1(){
  clawpos = 120;
  arm4pos = 141;
  arm3pos = 45;
  arm2pos = 51;
  arm1pos = 73;
  pivotpos = 91;
    arm1servomove(arm1pos);
    delay(100);
    clawservomove(clawpos);
    delay(100);
    arm4servomove(arm4pos);
    delay(100);
    arm3servomove(arm3pos);
    delay(100);
    arm2servomove(arm2pos);
    delay(100);
    pivotservomove(pivotpos);
    delay(100);
    arm1pos = 55;
    arm1servomove(arm1pos);
    arm1pos = 73;
    arm1servomove(arm1pos);
     delay(5000);
    clawpos = 90;
    clawservomove(clawpos);
    delay(100);
    arm1pos = 55;
    arm1servomove(arm1pos);
    delay(100);
    pivotpos = 50;
    pivotservomove(pivotpos);
    pivotpos = 120;
    pivotservomove(pivotpos);
    delay(100);
    arm1pos = 73;
    arm1servomove(arm1pos);
    delay(1000);
    clawpos = 120;
    clawservomove(clawpos);
    delay(100);
    arm3pos = 25;
    arm3servomove(arm3pos);
    delay(100);
    arm2pos = 61;
    arm2servomove(arm2pos);
    delay(100);
    arm3pos = 9;
    arm3servomove(arm3pos);
    delay(100);
    arm2pos = 100;
    arm2servomove(arm2pos);
    delay(100);
    clawpos = 90;
    clawservomove(clawpos);
    delay(100);
    arm1pos = 46;
    arm1servomove(arm1pos);
    delay(100);
    pivotpos = 50;
    pivotservomove(pivotpos);
    delay(100);
    arm1pos = 88;
    arm1servomove(arm1pos);
    delay(100);
    clawpos = 120;
    clawservomove(clawpos);
    delay(100);
    arm1pos = 50;
    arm1servomove(arm1pos);
    delay(100);
    pivotpos = 80;
    pivotservomove(pivotpos);
    neutralpos();
    
 
}

void demo2(){
 clawpos = 120;
  arm4pos = 141;
  arm3pos = 45;
  arm2pos = 51;
  arm1pos = 73;
  pivotpos = 91;
    arm1servo.write(arm1pos);
    delay(100);
    clawservo.write(clawpos);
    delay(100);
    arm4servo.write(arm4pos);
    delay(100);
    arm3servo.write(arm3pos);
    delay(100);
    arm2servo.write(arm2pos);
    delay(100);
    pivotservo.write(pivotpos);
    delay(100);
    arm1pos = 55;
    arm1servo.write(arm1pos);
    delay(100);
    arm1pos = 73;
    arm1servo.write(arm1pos);
     delay(5000);
    clawpos = 90;
    clawservo.write(clawpos);
    delay(100);
    arm1pos = 55;
    arm1servo.write(arm1pos);
    delay(100);
    pivotpos = 50;
    pivotservo.write(pivotpos);
    pivotpos = 120;
    pivotservo.write(pivotpos);
    delay(100);
    arm1pos = 73;
    arm1servo.write(arm1pos);
    delay(1000);
    clawpos = 120;
    clawservo.write(clawpos);
    delay(100);
    arm3pos = 25;
    arm3servo.write(arm3pos);
    delay(100);
    arm2pos = 61;
    arm2servo.write(arm2pos);
    delay(100);
    arm3pos = 9;
    arm3servo.write(arm3pos);
    delay(100);
    arm2pos = 100;
    arm2servo.write(arm2pos);
    delay(100);
    clawpos = 90;
    clawservo.write(clawpos);
    delay(100);
    arm1pos = 46;
    arm1servo.write(arm1pos);
    delay(100);
    pivotpos = 50;
    pivotservo.write(pivotpos);
    delay(100);
    arm1pos = 88;
    arm1servo.write(arm1pos);
    delay(100);
    clawpos = 120;
    clawservo.write(clawpos);
    delay(100);
    arm1pos = 50;
    arm1servo.write(arm1pos);
    neutralpos();
 
}

void demo3(){
  clawpos = 90;
  arm4pos = 180;
  arm3pos = 15;
  arm2pos = 20;
  arm1pos = 38;
  pivotpos = 91;
    arm1servomove(arm1pos);
    delay(100);
    clawservomove(clawpos);
    delay(100);
    arm4servomove(arm4pos);
    delay(100);
    arm3servomove(arm3pos);
    delay(100);
    arm2servomove(arm2pos);
    delay(100);
    pivotservomove(pivotpos);
     delay(4000);
    clawpos = 85;
    clawservomove(clawpos);
    delay(100);
    arm3pos = 41;
    arm3servomove(arm3pos);
    delay(100);
    arm4pos = 157;
    arm4servomove(arm4pos);
    delay(100);
    pivotpos = 50;
    pivotservomove(pivotpos);
    pivotpos = 120;
    pivotservomove(pivotpos);
    delay(1000);
    clawpos = 90;
    clawservomove(clawpos);
    delay(500);
    arm1pos = 34;
    arm1servomove(arm1pos);
    delay(500);
    clawpos = 85;
    clawservomove(clawpos);
    delay(100);
    pivotpos = 50;
    pivotservomove(pivotpos);
    delay(100);
    arm1pos = 57;
    arm1servomove(arm1pos);
    delay(500);
    clawpos = 90;
    clawservomove(clawpos);
    neutralpos();
 
}


void demo4(){
  clawpos = 90;
  arm4pos = 180;
  arm3pos = 15;
  arm2pos = 20;
  arm1pos = 38;
  pivotpos = 91;
    arm1servo.write(arm1pos);
    delay(100);
    clawservo.write(clawpos);
    delay(100);
    arm4servo.write(arm4pos);
    delay(100);
    arm3servo.write(arm3pos);
    delay(100);
    arm2servo.write(arm2pos);
    delay(100);
    pivotservo.write(pivotpos);
     delay(4000);
    clawpos = 85;
    clawservo.write(clawpos);
    delay(100);
    arm3pos = 41;
    arm3servo.write(arm3pos);
    delay(100);
    arm4pos = 157;
    arm4servo.write(arm4pos);
    delay(100);
    pivotpos = 50;
    pivotservo.write(pivotpos);
    pivotpos = 120;
    pivotservo.write(pivotpos);
    delay(1000);
    clawpos = 90;
    clawservo.write(clawpos);
    delay(500);
    arm1pos = 34;
    arm1servo.write(arm1pos);
    delay(500);
    clawpos = 85;
    clawservo.write(clawpos);
    delay(100);
    pivotpos = 50;
    pivotservo.write(pivotpos);
    delay(100);
    arm1pos = 57;
    arm1servo.write(arm1pos);
    delay(500);
    clawpos = 90;
    clawservo.write(clawpos);
    neutralpos();
 
}

