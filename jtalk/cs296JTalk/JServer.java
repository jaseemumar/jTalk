package cs296JTalk;
import java.util.*;
import java.io.*;
import java.net.*;
class JServer implements Com{
	ServerSocket serverSocket;
	ObjectOutputStream out;
	ObjectInputStream in;
	protected JServer(){
		acceptConnection();
	}
	
	private void acceptConnection() { 
		int portNumber = 5123;
		
		try{
			serverSocket = new ServerSocket(portNumber);
			//System.out.println("waiting to be accepted");
			Socket clientSocket = serverSocket.accept();
			
			//System.out.println("Accepted");
			out = new ObjectOutputStream(new BufferedOutputStream(clientSocket.getOutputStream()));
			//out = new ObjectOutputStream(clientSocket.getOutputStream());
				//System.out.println("Op stream Made");
			out.flush();                 
			in = new ObjectInputStream (new BufferedInputStream(clientSocket.getInputStream()));
			//System.out.println("I stream Made");
			//~ BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
			JPacket inputobj;
			//System.out.println("Waiting for input");
			while ((inputobj = (JPacket) in.readObject()) != null) {
				//System.out.println("InputRecieved");
				String inputLine=inputobj.jmsg.msg;
				//System.out.println(inputLine);
				if(inputLine.equals("Free for a chat?")) {
					//System.out.println("Client: Free for a chat?");
					JPacket np=new JPacket("Sure. Let us begin.");
					out.writeObject(np);
					out.flush();
					//System.out.println("Server: Sure. Let us begin.");
					break;
				}
			}
		} catch (IOException e) {
			System.out.println("Exception caught when trying to listen on port "
				+ portNumber + " or listening for a connection");
			System.out.println(e.getMessage());
			System.exit(1);
		}catch (ClassNotFoundException e){
			System.err.println("Class Not Found");
			System.exit(1);
		}
	}
	public void closeConnection(){
		try{
		System.out.println("Closing Connection");
		serverSocket.close();
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


