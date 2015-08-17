Group Number:20

Group Members
-------------------------------------------------
Jaseem Umar M 	120050081	jaseemumar@gmail.com
Sai Kiran M		120050068 	sai.mudulkar@cse.iitb.ac.in
Aman Gour  		120050030	frndlyaman@gmail.com

Disclaimer
------------------------------------------------
We have done this assignment by ourselves and have not plagiarized. We have used the below given websites for help.

http://docs.oracle.com/javase/tutorial/
http://www.tutorialspoint.com/java/io/objectinputstream_readobject.htm
http://www.tutorialspoint.com/java/java_date_time.htm
docs.oracle.com/javase/tutorial/uiswing/
http://stackoverflow.com/

Class Relationships
-----------------------------------------------
The two classes JClient and JServer are declared as implementations of  Com interface.
The Com interface contains three functions closeConnection, sendMessage and readMessage.

JMultiServer class creates a window for the Server using JServerWindow class. It creates JChatComm and JChatSession objects for each client that connects with server.


The main function in jtalkG20 calls only JChatComm, JChatSession and JMultiServer classes.
JChatComm is a subclass of Thread and its run method listen for input from the other end of chat.
JChatComm uses the JChatSession instance created by main, to log all the messages. It creates a JServer/JClient instance as per need. It also creates a JChatWindow object for each chat that starts.
Functions in JChatComm, JServer and JClient creates instances of JPacket which in turn has an instance of JMessage as a data variable.


