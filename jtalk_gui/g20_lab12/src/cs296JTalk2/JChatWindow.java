package cs296JTalk2;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

public class JChatWindow extends JFrame{
	protected JTextField textfield;
	protected JTextArea textarea;
	protected JChatWindow( final JChatComm chatComm, String hostName) throws Exception{
		super(hostName);
		JPanel contentPane= new JPanel();
		contentPane.setLayout(new BoxLayout(contentPane, BoxLayout.Y_AXIS));
		setSize(new Dimension(300, 400));
		contentPane.setSize(new Dimension(300, 400));
		
		textarea = new JTextArea(300,300);
		textarea.setLineWrap(true);
		//textarea.setPreferredSize(new Dimension(300,300));
		textarea.setEditable(false);
		JScrollPane scroll = new JScrollPane (textarea);
		scroll.setSize(300,300);
		scroll.setVerticalScrollBarPolicy ( ScrollPaneConstants.VERTICAL_SCROLLBAR_AS_NEEDED );
    
		textfield = new JTextField();
		textfield.setMinimumSize(new Dimension(300, 50));
		textfield.setMaximumSize(new Dimension(300, 50));
		textfield.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {		
					try{chatComm.sendMessage();}
					catch(Exception er) {  System.out.println(er.getMessage());}
			}
		});
		
		JButton button = new JButton("Send");
		button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {		
					try{chatComm.sendMessage();}
					catch(Exception er) {  System.out.println(er.getMessage());}
			}
		});
		
		contentPane.add(scroll);
		contentPane.add(textfield);
		contentPane.add(button);
		setVisible(true);
		setContentPane(contentPane);

		addWindowListener(new java.awt.event.WindowAdapter() {
			@Override
			public void windowClosing(java.awt.event.WindowEvent windowEvent) {
				chatComm.sendEnd();
			}
		});
	}

	protected void closeFrame(){
		setVisible(false);
		dispose();
	}
	
}



