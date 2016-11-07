/**
   Edit By Callor 2016. 11. 1

   ElECREAK ���� esp8266 Serial Wifi Module��
   Nucleo F103RB���� ����ϱ� ���� ���̺귯��
  
      1. Serial1�� ���� �ø��� ����͸�
      

   ELECREAK���� esp8266 Serial Wifi Module��
      1. �⺻ �ӵ��� 9600���� ���õǾ� �ִ�.
*/

#include "Arduino.h"
#include "ELECFREAKS_ESP8266_STM.h"


// ����� �޽��� �ø��� ����ͷ� ���
#define DBT(message)    Serial1.print(message)
#define DBG(message)    Serial1.println(message)
#define DBGW(message)   Serial1.write(message)


// Ŭ���� ������
ELEC_Wifi::ELEC_Wifi(void) {

}

void ELEC_Wifi::begin(void) {
      boolean result = false;
      

	  Serial1.begin(115200);   // ����͸� ��Ʈ �ӵ�
      Serial2.begin(9600);	// Serial2 ��Ʈ �ӵ�, ELECFREAKS 8266�� �⺻ �ӵ��� 9600���� ���� �Ǿ� �ִ�.

	  Serial1.setTimeout(10000);

	  Serial2.flush();
	  Serial2.setTimeout(10000);

      // Serial2 Reboot
      Serial2.println("AT+RST");
      Serial1.println("AT+RST\r\n");
      result = Serial2.find((char*)"OK");
      if(result) Serial1.println("Module is ready\r\n");
      else {
            Serial1.println("Module have no response\r\n");
            while(1);
      }
}

String ELEC_Wifi::checkFirm(){
    String data = "";
    Serial2.println(F("AT+GMR"));

    unsigned long start;
    start = millis();
    while (millis()-start<2000) {
      if(Serial2.available()>0) {
          char a = Serial2.read();
          data=data+a;
      }
    }

    return data;
}



/**
 *  ����� ������ī�� ���� ����
 */
bool ELEC_Wifi::setStation()
{
    String data;

    // ����� Station Mode�� ����
    Serial2.println("AT+CWMODE=1");  
    
    unsigned long start;
    start = millis();
    while (millis()-start<2000) {
      if(Serial2.available()>0) {
          char a = Serial2.read();
          data=data+a;
      }
      // ��� ������ �����ϰų�, �̹� Station ����ϰ�� true, �ƴϸ� false
      if (data.indexOf("OK")!=-1 || data.indexOf("no change")!=-1) return true;
      if (data.indexOf("ERROR")!=-1 || data.indexOf("busy")!=-1) return false;
   }
  return true;
}

/**
 * �����⿡ ���� �õ�
 */
 boolean ELEC_Wifi::joinAP(String ssid , String pwd) {
 
    Serial2.print("AT+CWJAP=");

    //"ssid"
    Serial2.print("\"");    
    Serial2.print(ssid);  
    Serial2.print("\"");

    Serial2.print(",");
    
    //"pwd"
    Serial2.print("\"");      
    Serial2.print(pwd); 
    Serial2.println("\"");

	String data;
    unsigned long start;
    start = millis();
    while (millis()-start<3000) {
       if(Serial2.available()>0) {
             char a =Serial2.read();
             data=data+a;
       }
	   
       if (data.indexOf("OK")!=-1) return true;
    }
	DBG(data); // ������ �߻��ϸ� ����ͷ� ����
    return false;
}


String ELEC_Wifi::showJAP(void)
{
    Serial2.flush();
    Serial2.println("AT+CWJAP?");  
    
    String data;
    unsigned long start;
    start = millis();
    while (millis()-start<3000) {
       if(Serial2.available()>0) {
             char a =Serial2.read();
             data=data+a;
       }
       if (data.indexOf("OK")!=-1 || data.indexOf("ERROR")!=-1 ) break;
       
    }
    
    char head[4] = {0x0D,0x0A};   
    char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
    data.replace("AT+CWJAP?","");
    data.replace("+CWJAP","AP");
    data.replace("OK","");
    data.replace(tail,"");
    data.replace(head,"");
      
    return data;
}


/**
 * ��� ���� üũ
 * 
 * return
 *    2: Got IP
 *    3: Connected
 *    4: Disconnected
 */

String ELEC_Wifi::showStatus(void) {
    Serial2.println("AT+CIPSTATUS");  

    String data;
    unsigned long start;
    start = millis();
    while (millis()-start<3000) {
       if(Serial2.available()>0) {
          char tc = Serial2.read();
          data=data+tc;
       }
       if (data.indexOf("OK")!=-1) break;
    }

    char head[4] = {0x0D,0x0A};   
    char tail[7] = {0x0D,0x0A,0x0D,0x0A};        
    data.replace("AT+CIPSTATUS","");
    data.replace("OK","");
    data.replace(tail,"");
    data.replace(head,"");
    data.replace("STATUS:2","Got IP");
    data.replace("STATUS:3","Connect");
    data.replace("STATUS:4","DisConnect");
              
    return data;
}

/**
 * ����� �ڵ����� ���� IP ����
 */
String ELEC_Wifi::showIP(void) {
    String data;
    unsigned long start;

    for(int a=0; a<3;a++) {
        Serial2.println("AT+CIFSR");  
        start = millis();
        while (millis()-start<3000) {
            while(Serial2.available()>0) {
              char ct = Serial2.read();
              data=data+ct;
            }
            if(data.indexOf("AT+CIFSR")!=-1) break;
        }
        if(data.indexOf(".") != -1) break;
        data = "";
    }

    char head[4] = {0x0D,0x0A};   
    char tail[7] = {0x0D,0x0D,0x0A};        
    data.replace("AT+CIFSR","");
    data.replace("OK","");
    data.replace(tail,"");
    data.replace(head,"");
  
    return data;
}

boolean ELEC_Wifi::connectServer(byte type, String addr, int port) {
    String data;
    Serial2.print("AT+CIPSTART=");
    if(type>0) {
        Serial2.print("\"TCP\"");
    }else {
        Serial2.print("\"UDP\"");
    }
    Serial2.print(",");
    Serial2.print("\"");
    Serial2.print(addr);
    Serial2.print("\"");
    Serial2.print(",");
//    Serial2.print("\"");
    Serial2.println(String(port));
//    Serial2.println("\"");
    unsigned long start;
  start = millis();
  while (millis()-start<3000) { 
     if(Serial2.available()>0)
     {
		char a =Serial2.read();
		data=data+a;
     }
	 
     if (data.indexOf("OK")!=-1 || data.indexOf("ALREAY CONNECT")!=-1 || data.indexOf("ERROR")!=-1)
     {
         return true;
     }
  }
  Serial1.print("Conn:");
  DBG(data);
  return false;
}

boolean ELEC_Wifi::Send(String str) {
	int strLen = str.length() ;

    Serial2.print("AT+CIPSEND=");
    Serial2.println(strLen);

    Serial1.print("AT+CIPSEND=");
    Serial1.println(strLen);

    unsigned long start;
    start = millis();
    bool found;
    while (millis()-start<5000) {                            
        if(Serial2.find((char*)">")==true ) {
            found = true;
            break;
        }
     }
    if(found) {
		Serial1.print("Send > ");
		Serial1.println(str);

		Serial2.println(str);
//		Serial2.println(host);
//		Serial2.println("User-Agent: launchpad-wifi");
//		Serial2.println("Connection: close");
	} else {
      DBG("Not found");
      return false;
    }
	
	/*
	 * ���� �� �޽��� Ȯ��
	 */
    String data;
    start = millis();
    while (millis()-start<5000) {                            
		if(Serial2.available()>0){
			while(Serial2.available()>0) {
					char a =Serial2.read();
					data=data+a;
					if (data.indexOf("SEND OK")!=-1) {
							DBG("Send OK");
							return true;
					}
			 }
		}
	}

	 Serial1.print("Recv:");
	 DBG(data);
	 


     return false;


	/*
    start = millis();
    while (millis()-start<100) {
       if(Serial2.available()>0) {
	
            char a =Serial2.read();
            data=data+a;
        
			if (data.indexOf("SEND OK")!=-1) {
				DBG("Send OK");
				return true;
			}
	   }
     }
	 Serial1.print("Recv:");
	 DBG(data);
     return false;
	 */
	 
}




int ELEC_Wifi::ReceiveMessage(char *buf)
{
    String data;
    unsigned long start = millis();
    while (millis()-start<5000) {                            
		if(Serial2.available()>0){
			while(Serial2.available()>0) {
					char a =Serial2.read();
					data=data+a;
			 }
		}
	}

	 Serial1.print("Recv:");
	 DBG(data);
	
	strcpy(buf, data.c_str());
	return data.length();
	


	//+IPD,<len>:<data>
	//+IPD,<id>,<len>:<data>
	
	if (Serial2.available()>0) {
		unsigned long start;
		start = millis();
		char c0 = Serial2.read();
		if (c0 == '+') {
			while (millis()-start<5000) {
				if (Serial2.available()>0) {
					char c = Serial2.read();
					data += c;
				}
				if (data.indexOf("\nOK")!=-1) {
					break;
				}
			}
			
			int sLen = strlen(data.c_str());
			int i,j;
			for (i = 0; i <= sLen; i++) {
				if (data[i] == ':'){
					break;
				}
			}

			boolean found = false;
			for (j = 4; j <= i; j++){
				if (data[j] == ','){
					found = true;
					break;
				}
				
			}
			
			int iSize ;

			if(found == false) {
					String _size = data.substring(4, i);
					iSize = _size.toInt();
					String str = data.substring(i+1, i+1+iSize);
					strcpy(buf, str.c_str());
			}
			return iSize;
		}
	}
	return 0;
}


String ELEC_Wifi::getTime() {
    String data;
	char dd ;
    unsigned long start = millis();
    while (millis()-start<5000) {                            
		while(Serial2.available()>0) {
			//if (Serial1.read() == '\n') {    
					DBG("n");
			  if (Serial2.read() == 'D') {    
					  DBG("D");
				if (Serial2.read() == 'a') {    
						DBG("a");
				  if (Serial2.read() == 't') {    
						  DBG("t");
					if (Serial2.read() == 'e') {    
							DBG("e");
					  if (Serial2.read() == ':') {    
							  DBG(":");
							Serial2.read();
							String timeData = Serial2.readStringUntil('\r');
						return timeData;
					  }
					}
				  }
				}
			  }
			// }
		
		}
	}
	return "Not found";

} 