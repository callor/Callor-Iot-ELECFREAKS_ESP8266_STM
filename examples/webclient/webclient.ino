#include <ELECFREAKS_ESP8266_STM.h>

/**
   Edit By Callor 2016. 11. 2

   ElECREAK 사의 esp8266 Serial Wifi Module을
   Nucle에서 사용하기
   
   ELECREAK사의 esp8266 Serial Wifi Module은
      1. 기본 속도가 9600으로 세팅되어 있다

*/

#define DBT(message)      Serial1.print(message)
#define DBG(message)      Serial1.println(message)
#define DBGW(message)     Serial1.write(message)

#define SSID "MyAP"
#define PASSWD "MyPasswd"

#define UDP 0
#define TCP 1

ELEC_Wifi wifi ;

void setup() {

  Serial1.begin(9600);
  Serial1.println("Start EPS8266 ...");

  delay(2000);

  wifi.begin() ;
  
  wifi.setStation() ;
  
  DBT("AP : ");
  while(!wifi.joinAP(SSID, PASSWD)) ;
  DBG(wifi.showJAP());
  
  DBT("Status : ");
  DBG(wifi.showStatus());
  
  DBT("IP : ");
  DBG(wifi.showIP());
  

}

int last = millis();
int interval = 0 ;
void loop() {

  String ip = "192.168.0.110";
  int port = 3000;
  wifi.connectServer(TCP, ip, port);

  
  if(millis() > (last + interval)) {
    DBG("Send..") ;
    
    String strValue = String(millis());
    DBG(strValue);
    
    // PUT으로 서버에 값을 넘겨 DB에 저장하도록 테스트
//    while(wifi.Send("PUT /insert/HT001/50/20/" + strValue + " HTTP/1.0\r\n\r\n")) {

   // HEAD method를 전송하여 날짜값 등을 알아보기 테스트
    while(!wifi.Send("HEAD /time/" + strValue + " HTTP/1.1\r\n\r\n")) {

      DBG("Re Connect..") ;
      wifi.connectServer(TCP, ip, port);
      DBG("Re Send..") ;
    }
    last = millis();
    interval = 100000;
  }

  char message[320];

  if (wifi.ReceiveMessage(message))
  {
    DBG(message);
  }

}