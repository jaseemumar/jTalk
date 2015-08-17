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
http://www.coderanch.com/t/205325/sockets/java/send-java-Object-socket

Class Relationships
-----------------------------------------------
The two classes JClient and JServer are declared as implementations of  Com interface.
The Com interface contains three functions closeConnection, sendMessage and readMessage.

The main function in jtalkG20 calls only two classes in the cs296jtalk package : JChatComm and JChatSession.
JChatComm uses the JChatSession instance created by main, to log all the messages. JChatComm also create a JServer/JClient instance as per need.
Functions in JChatComm, JServer and JClient creates instances of JPacket which in turn has an instance of JMessage as a data variable.
