package cs296JTalk2;
import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.io.*;
import java.net.*;

public class JServerWindow extends JFrame{
	JPanel contentPane = new JPanel();
	protected JServerWindow(){
		super("JTalk");
		contentPane.setSize(new Dimension(250, 500));
		setSize(new Dimension(250, 500));
		JLabel emptyLabel = new JLabel("Connected Clients");
		//emptyLabel.setPreferredSize(new Dimension(250,30));
		contentPane.add(emptyLabel, BorderLayout.NORTH);
		pack();
		setVisible(true);
		setContentPane(contentPane);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}

	protected boolean addButton(final JChatComm chatComm, final SocketAddress hostName){
		SwingUtilities.invokeLater(new Runnable() {
			public void run() {
				//System.out.println("Creating button");
				JButton button = new JButton(hostName.toString());
				button.setPreferredSize(new Dimension(250, 25));
				//final Tab tab = new Tab(jst);
				button.addActionListener(new ActionListener() {
					public void actionPerformed(ActionEvent e) {
						 
						 try{chatComm.createWindow(); }
					catch(Exception er) {er.printStackTrace();}
						 //new JChatWindow(chatComm,hostName.toString());
					}
				});

				//chatComm.button=button;
				contentPane.add(button);
				contentPane.repaint();
				contentPane.revalidate();
				//System.out.println("Done creating button");
				chatComm.button=button;
				}
		});
		return true;
	}

	protected void removeButton(JButton button){
		//System.out.println("Removing Button");
		contentPane.remove(button);
		contentPane.repaint();
		contentPane.revalidate();
	}
}
