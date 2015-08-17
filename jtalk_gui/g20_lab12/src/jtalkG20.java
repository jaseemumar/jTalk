import cs296JTalk2.*;
import java.util.*;
import java.io.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
class jtalkG20{
	
	public static void main(String[] args) throws Exception{
		if (args.length == 1) {
			
			JChatSession chatSession = new JChatSession();
			JChatComm chatComm=new JChatComm(args[0], chatSession);
			chatComm.createWindow();

		}
		else if (args.length==0){
			JMultiServer server=new JMultiServer();
			server.acceptConnection();
		}
		else{
			System.err.println(
				"Usage: java jtalkG20 [host_name]");
			System.exit(1);
		}

		//System.out.println("Start chatting");
		
	}
}
