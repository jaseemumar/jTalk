package cs296JTalk;
import java.util.*;
import java.io.*;

class JMessage implements Serializable{
	protected String msg; //make non public later
	protected JMessage(String cmsg){
		msg=cmsg;
	}
}
