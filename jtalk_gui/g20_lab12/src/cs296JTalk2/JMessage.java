package cs296JTalk2;
import java.util.*;
import java.io.*;

class JMessage implements Serializable{
	protected String msg; //make non public later
	protected JMessage(String cmsg){
		msg=cmsg;
	}
}
