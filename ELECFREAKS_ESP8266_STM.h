#ifndef ELECFREAKS_ESP8266_STM_h
#define ELECFREAKS_ESP8266_STM_h



class ELEC_Wifi {

	public :

		ELEC_Wifi(void) ;
		
		//  ESP8266 모듈 초기화
		void begin(void) ;

		// 펌웨어 버젼 체크
		String checkFirm() ;

		// 모듈을 랜카드 상태로 설정
		bool setStation() ;

		
 		// 공유기에 접속 시도
		bool joinAP(String ssid , String pwd) ;

		// 접속공유기 정보 리턴
		String showJAP(void) ;

		/**
		 * 모듈 상태 체크
		 * 
		 * return
		 *    2: Got IP
		 *    3: Connected
		 *    4: Disconnected
		 */
		String showStatus(void) ;


		//  모듈이 자동으로 받은 IP 리턴
		String showIP(void) ;

		// 원격서버에 접속
		// type = TCP:1, UDP:0
		// addr = IP address
		// port = 원격 포트
		boolean connectServer(byte type, String addr, int port) ;
		
		// 원격서버에 쿼리
		boolean Send(String str) ;
		// boolean Send(String str,String host) ;

		// 메시지 수신
		int ReceiveMessage(char *buf);
		String getTime() ;

	private :
		// SoftwareSerial wSerial ;

};
#endif

