package cs296JTalk2;
import java.util.*;
import java.io.*;

class JPacket implements Serializable{
	JMessage jmsg;
	Date date;
	protected JPacket(String msg){
		jmsg = new JMessage(msg);
		date=new Date();
	}
}
