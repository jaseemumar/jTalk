package cs296JTalk;
import java.util.*;
import java.io.*;

public class JChatSession{
	private class Message{
		JPacket packet;
		String sender;
		Message(JPacket cpacket, String csender){
			packet=cpacket;
			sender=csender;
		}
	}
	List<Message> log;
	public JChatSession(){
		log = new ArrayList<Message>();
	}
	protected void addMsg(JPacket packet, String sender){
		log.add(new Message(packet,sender));
	}
	public void printLog(){
		for(Message message:log){
			System.out.println(message.packet.date.toString()+"\t"+ message.sender + " : "+ message.packet.jmsg.msg);
		}
	}
	
} 
