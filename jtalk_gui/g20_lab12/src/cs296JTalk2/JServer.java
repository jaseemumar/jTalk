package cs296JTalk2;
import java.util.*;
import java.io.*;
import java.net.*;

class JServer implements Com{
	Socket clientSocket = null;
	ObjectOutputStream out;
	ObjectInputStream in;

	protected JServer(Socket inputSocket){
		clientSocket=inputSocket;
		//System.out.println(inputSocket.getRemoteSocketAddress());
		acceptConnection();
	}
	
	protected void acceptConnection() { 
		
		try{
		
			out = new ObjectOutputStream(new BufferedOutputStream(clientSocket.getOutputStream()));
			out.flush();                 
			in = new ObjectInputStream (new BufferedInputStream(clientSocket.getInputStream()));

			JPacket inputobj;
			
			while ((inputobj = (JPacket) in.readObject()) != null) {
				
				String inputLine=inputobj.jmsg.msg;
				
				if(inputLine.equals("Free for a chat?")) {
					
					JPacket np=new JPacket("Sure. Let us begin.");
					out.writeObject(np);
					out.flush();
					
					break;
				}
			}
		} catch (IOException e) {
			System.out.println("Exception caught when trying to listen on port or listening for a connection");
			System.out.println(e.getMessage());
			System.exit(1);
		}catch (ClassNotFoundException e){
			System.err.println("Class Not Found");
			System.exit(1);
		}
	}
	public void closeConnection(){
		try{
		//System.out.println("Closing Connection");
		clientSocket.close();
		//in.close();
		//out.close();}
	}
		catch (IOException e){
			System.err.println("Couldn't get I/O for the connection while closing");
			System.exit(1);
		}
	}

	public void sendMessage(JPacket packet){
		try{
			out.writeObject(packet);
			out.flush();
		} catch (IOException e){
			System.err.println("Couldn't get I/O for the connection for sending ");
			System.exit(1);
		}
			
	}

	public JPacket receiveMessage(){
		try{
			//System.out.println("Reading Message");
			JPacket packet= (JPacket) in.readObject();
			
			return packet;
			
		} catch (IOException e){
			//System.err.println("Couldn't get I/O for the connection for receiving blah");
			//System.exit(1);
		}catch (ClassNotFoundException e){
			System.err.println("Class Not Found");
			System.exit(1);
		}
		
		return null;
	}
}


