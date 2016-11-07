#ifndef ELECFREAKS_ESP8266_STM_h
#define ELECFREAKS_ESP8266_STM_h



class ELEC_Wifi {

	public :

		ELEC_Wifi(void) ;
		
		//  ESP8266 ��� �ʱ�ȭ
		void begin(void) ;

		// �߿��� ���� üũ
		String checkFirm() ;

		// ����� ��ī�� ���·� ����
		bool setStation() ;

		
 		// �����⿡ ���� �õ�
		bool joinAP(String ssid , String pwd) ;

		// ���Ӱ����� ���� ����
		String showJAP(void) ;

		/**
		 * ��� ���� üũ
		 * 
		 * return
		 *    2: Got IP
		 *    3: Connected
		 *    4: Disconnected
		 */
		String showStatus(void) ;


		//  ����� �ڵ����� ���� IP ����
		String showIP(void) ;

		// ���ݼ����� ����
		// type = TCP:1, UDP:0
		// addr = IP address
		// port = ���� ��Ʈ
		boolean connectServer(byte type, String addr, int port) ;
		
		// ���ݼ����� ����
		boolean Send(String str) ;
		// boolean Send(String str,String host) ;

		// �޽��� ����
		int ReceiveMessage(char *buf);
		String getTime() ;

	private :
		// SoftwareSerial wSerial ;

};
#endif

