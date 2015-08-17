import cs296JTalk.*;
import java.util.*;
import java.io.*;

class jtalkG20{
	
	public static void main(String[] args) throws Exception{
		
		JChatComm chatComm = null;
		JChatSession chatSession = new JChatSession();
		if (args.length == 1) {
			chatComm=new JChatComm(args[0], chatSession);
			//System.out.println("Init done");
		}
		else if (args.length==0){
			//System.out.println("Server");
			chatComm=new JChatComm("0",chatSession);
			//System.out.println("Init done");
		}
		else{
			System.err.println(
				"Usage: java jtalkG20 [host_name]");
			System.exit(1);
		}

		System.out.println("Start chatting");
		while(true){
		chatComm.sendMessage();
		chatComm.readMessage();
		}

	}
}
