package cs296JTalk;
import java.io.*;

interface Com{
	public void closeConnection();
	public void sendMessage(JPacket packet);
	public JPacket receiveMessage();
}
