#include "STD_type.h"
#include "BIT_math.h"
#include "FunHelp.h"

#include "USART_interface.h"
#include "MQTT.h"

u16 PacketId = 1 ;

u8 Packet[200] = {0} ;
u8 PacketIndex = 0 ;

const u8 u8protocolName[] = "MQTT" ;
void MQTT_Connect(u8 *Copyu8ClientId) {
u8 Local_u8Remlen 	= (12 + STR_LEN(Copyu8ClientId)) ;

	PacketIndex = 0 ;
	Packet[PacketIndex++] = 0x10 ;   //connect type
	Packet[PacketIndex++] = Local_u8Remlen ;
	Packet[PacketIndex++] = 0x00 ;
	Packet[PacketIndex++] =  STR_LEN(u8protocolName) ;
	/*Send MQTT*/
	STR_COPY((Packet+PacketIndex) , u8protocolName);
	PacketIndex+= STR_LEN(u8protocolName) ;

	Packet[PacketIndex++] = 0x04 ;
	Packet[PacketIndex++] = 0x02 ;
	Packet[PacketIndex++] = 0xFF ;
	Packet[PacketIndex++] = 0xFF ;

	Packet[PacketIndex++] = 0x00 ;
	Packet[PacketIndex++] =  STR_LEN(Copyu8ClientId) ;
	/*Send Client id*/
	STR_COPY((Packet+PacketIndex) , Copyu8ClientId);
	PacketIndex+= STR_LEN(Copyu8ClientId) ;
	/*send packet */
	MUSART1_VidSendStringSynch(Packet);

}
void MQTT_Publish(u8 * Topic , u8 *Msg , u32 len , u8 Qos) {
	u8 Local_u8msgindex = 0 ;
	u8 Local_u8Remlen 	= (12 + STR_LEN(Topic)+ len) ;

		PacketIndex = 0 ;

		if (Qos > 0 ){

			Local_u8Remlen+=2 ;
			Packet[PacketIndex++] = 0x32 ;   //connect type
		}
		else {
			Packet[PacketIndex++] = 0x30 ;   //connect type
		}
		Packet[PacketIndex++] = Local_u8Remlen ;
		Packet[PacketIndex++] = 0x00 ;
		Packet[PacketIndex++] =  STR_LEN(Topic) ;
		/*Send MQTT*/
		STR_COPY((Packet+PacketIndex) , Topic);
		PacketIndex+= STR_LEN(Topic) ;
		if (Qos > 0 ){

				Packet[PacketIndex++] =  ( (u8)PacketId >> 8) ;
				Packet[PacketIndex++] =  ((u8)PacketId) ;
				PacketId++ ;
		}

		for (Local_u8msgindex = 0 ; Local_u8msgindex  < len ; Local_u8msgindex++)
		{
			Packet[PacketIndex++] =  Msg[Local_u8msgindex] ;

		}
		/*send packet */
		MUSART1_VidSendStringSynch(Packet);
}
void MQTT_Subscribe(u8 *Topic) {
	u8 Local_u8Remlen 	= (5 + STR_LEN(Topic)) ;

		PacketIndex = 0 ;
		Packet[PacketIndex++] = 0x82 ;   //connect type
		Packet[PacketIndex++] = Local_u8Remlen ;

		Packet[PacketIndex++] =  ( (u8)PacketId >> 8) ;
		Packet[PacketIndex++] =  ((u8)PacketId) ;
		PacketId++ ;


		Packet[PacketIndex++] = 0x00 ;
		Packet[PacketIndex++] =  STR_LEN(Topic) ;
		/*Send MQTT*/
		STR_COPY((Packet+PacketIndex) , Topic);
		PacketIndex+= STR_LEN(Topic) ;



		Packet[PacketIndex++] = 0x01 ;

		/*send packet */
		MUSART1_VidSendStringSynch(Packet);
}

