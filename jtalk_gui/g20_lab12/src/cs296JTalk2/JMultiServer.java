package cs296JTalk2;
import java.util.*;
import java.io.*;
import java.net.*;
public class JMultiServer {
	JServerWindow window;
	public boolean acceptConnection() throws IOException {
		int portNumber = 5123;
		boolean listening = true;
		window=new JServerWindow();
		try {
			
			ServerSocket serverSocket = new ServerSocket(portNumber);
			//System.out.println("socket made");
			while (listening) {
				JChatSession chatSession = new JChatSession();
				Socket clientSocket=serverSocket.accept();
				//System.out.println(inputSocket.getRemoteSocketAddress());
				JChatComm chatComm=new JChatComm(clientSocket, chatSession, window);
				window.addButton(chatComm,clientSocket.getRemoteSocketAddress() );
			}
		} catch (IOException e) {
			System.err.println("Could not listen on port ");
			System.exit(-1);
		}
		return true;
	}
}
