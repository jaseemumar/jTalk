package cs296JTalk;
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
