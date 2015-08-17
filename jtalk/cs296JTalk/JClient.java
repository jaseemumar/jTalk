package cs296JTalk;
import java.util.*;
import java.io.*;
import java.net.*;

class JClient implements Com{
	Socket socket;
	ObjectOutputStream out;
	ObjectInputStream in;
	protected JClient(String hostName) {
		callServer(hostName);
	}
	private void callServer(String hostName) {
		int portNumber = 5123;
		try {
			socket = new Socket(hostName, portNumber);
			socket.setSoTimeout(10000);
			out = new ObjectOutputStream(new BufferedOutputStream(socket.getOutputStream()));
			out.flush();             
			in = new ObjectInputStream (new BufferedInputStream(socket.getInputStream()));
			String userInput;
			String inputLine;
			JPacket np=new JPacket("Free for a chat?");
			//System.out.println("making first contact");
			out.writeObject(np);
			out.flush();
			//System.out.println("Client:Free for a chat?");
			JPacket inputobj;
			while ((inputobj = (JPacket) in.readObject()) != null) {
				inputLine=inputobj.jmsg.msg;
				//System.out.println(inputLine);
				if(inputLine.equals("Sure. Let us begin.")) {
					//System.out.println("Server:Sure. Let us begin.");
					//System.out.println("Chat Session Starts");
					break;
				}
			}
			
		}catch (SocketTimeoutException e){
			System.err.println("Server is not responding");
			System.exit(1);
		} catch (UnknownHostException e) {
			System.err.println("Don't know about host " + hostName);
			System.exit(1);
		} catch (IOException e) {
			System.err.println("Couldn't get I/O for the connection to " +
				hostName);
			System.exit(1);
		} catch (ClassNotFoundException e){
			System.err.println("Class Not Found");
			System.exit(1);
		}
		//System.out.println("Exitting");
		//return new ObjStreams(in,out);
	}
	public void closeConnection(){
		System.out.println("Closing Connection");
		try{
		socket.close();
		in.close();
		out.close();}
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
			JPacket packet= (JPacket) in.readObject();
			return packet;
		} catch (IOException e){
			System.err.println("Couldn't get I/O for the connection for receiving");
			System.exit(1);
		}catch (ClassNotFoundException e){
			System.err.println("Class Not Found");
			System.exit(1);
		}
		
		return null;
	}
}
