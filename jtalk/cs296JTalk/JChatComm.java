package cs296JTalk;
import java.util.*;
import java.io.*;

public class JChatComm{

	JChatSession session;
	Com com;
	Console console = System.console();
	public JChatComm(String hostName, JChatSession csession){
		if (hostName.equals("0")) com=new JServer();
		else com =new JClient(hostName);
		session=csession;
	}
	public void sendMessage() throws Exception{
		String userInput;
		userInput = console.readLine();
		JPacket packet=new JPacket(userInput);
		com.sendMessage(packet);
		if (userInput.equals("End Chat")){
			endChat();
			System.out.println("Chat Over");
		}
		else logMessage(packet, "You");
	}
	private void logMessage(JPacket packet, String sender){
		session.addMsg(packet,sender);
	}
	public void readMessage() {
		JPacket packet= com.receiveMessage();
		if(packet.jmsg.msg.equals("End Chat")){
			endChat();
			System.out.println("Chat Over");
		}
		else {
			logMessage(packet, "OtherGuy");
			System.out.println("OtherGuy: "+packet.jmsg.msg);
		}
		
	}
	public void endChat(){
		com.closeConnection();
		System.out.println("\nLog");
		System.out.println("-----------------------------------------------");
		session.printLog();
		System.exit(1);
	}
}
