package cs296JTalk2;
import java.util.*;
import java.io.*;
import java.text.*;

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
	public void printLog() {
		Date date = new Date();
		 SimpleDateFormat ft = new SimpleDateFormat ( "dd-MM-yy");
		 String filename="chat_log_"+ft.format(date)+".txt";
		try{
		PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(filename, true)));
		for(Message message:log){
			out.println(message.packet.date.toString()+"\t"+ message.sender + " : "+ message.packet.jmsg.msg);
		}
		out.close();
		} catch(Exception e){
			System.out.println("IO Exception");
		}
		
	}
	
} 
