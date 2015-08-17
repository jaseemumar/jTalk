package cs296JTalk2;
import java.util.*;
import java.io.*;
import java.net.*;
import javax.swing.*;
public class JChatComm  extends Thread{
	protected JButton button;
	JServerWindow serverWindow;
	JChatSession session;
	public Com com;
	String hostName;
	private volatile boolean isRunning; 
	JChatWindow chatwindow;
	public JChatComm(String hostName, JChatSession csession){
		this.hostName=hostName;
		com =new JClient(hostName);
		session=csession;
		button=null;
		serverWindow=null;
		isRunning=true; 
	}
	protected JChatComm(Socket socket, JChatSession csession, JServerWindow window){
		com =new JServer(socket);
		hostName=socket.getRemoteSocketAddress().toString();
		session=csession;
		serverWindow=window;
		isRunning=true;
	}
	public boolean createWindow() throws Exception{
		chatwindow=new JChatWindow(this, hostName);
		start();
		return true;
		};
	public void run(){
		try{
			while(isRunning){
				readMessage();
			}
		} catch(Exception e) {
              System.out.println(e.getMessage());
		}
	}
	protected void sendMessage() throws Exception{
		String userInput;
		userInput = chatwindow.textfield.getText();
		chatwindow.textarea.append("You: "+userInput+"\n");
		chatwindow.textfield.setText("");
		JPacket packet=new JPacket(userInput);
		com.sendMessage(packet);
		if (userInput.equals("End Chat")){
			System.out.println("Ending chat");
			endChat();
		}
		else logMessage(packet, "You");
	}
	
	private void logMessage(JPacket packet, String sender){
		session.addMsg(packet,sender);
	}
	
	protected void readMessage() {
		
			JPacket packet= com.receiveMessage();
			if(packet.jmsg.msg.equals("End Chat")){
				chatwindow.closeFrame();
				System.out.println("Chat Over");
				endChat();
				
			}
			else {
				logMessage(packet, hostName);
				chatwindow.textarea.append(hostName+": "+packet.jmsg.msg+"\n");
			}
		
	}
	protected void endChat(){
		isRunning=false;
		com.closeConnection();
		session.printLog();
		
		if(button==null)System.exit(1);
		else {
			serverWindow.removeButton(button);chatwindow.closeFrame();}
	}

	protected void sendEnd(){
		JPacket packet=new JPacket("End Chat");
		com.sendMessage(packet);
		System.out.println("Ending chat");
		endChat();
	}
}
