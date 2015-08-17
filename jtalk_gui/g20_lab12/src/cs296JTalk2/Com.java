package cs296JTalk2;
import java.io.*;

interface Com{
	public void closeConnection();
	public void sendMessage(JPacket packet);
	public JPacket receiveMessage();
}
