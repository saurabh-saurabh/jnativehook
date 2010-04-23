/* Copyright (c) 2007-2010 - Alex Barker (alex@1stleg.com)
 * 
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
package org.jnativehook.example;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.FocusTraversalPolicy;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseEvent;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import org.jnativehook.GlobalScreen;
import org.jnativehook.keyboard.NativeKeyListener;
import org.jnativehook.mouse.NativeMouseInputListener;

// TODO: Auto-generated Javadoc
/**
 * The Class NativeHookDemo.
 */
public class NativeHookDemo extends JFrame implements NativeKeyListener, NativeMouseInputListener, ActionListener, WindowListener {
	/** The Constant serialVersionUID. */
	private static final long serialVersionUID = -5549783775591314629L;

	/** The txt event info. */
	private JTextArea txtEventInfo;
	
	/**
	 * Instantiates a new native hook demo.
	 */
	public NativeHookDemo() {
		setTitle("JNativeHook Demo");
		setLayout(new BorderLayout());
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setSize(300, 400);
		setFocusTraversalPolicy(new NoFocusTraversalPolicy());
		addWindowListener(this);
		
		txtEventInfo = new JTextArea();
		txtEventInfo.setEditable(false);
		txtEventInfo.setBackground(new Color(0xFF, 0xFF, 0xFF));
		txtEventInfo.setForeground(new Color(0x00, 0x00, 0x00));
		JScrollPane scrollPane = new JScrollPane(txtEventInfo);
		scrollPane.setPreferredSize(new Dimension(375, 125));
		
		add(scrollPane, BorderLayout.CENTER);
		
		setVisible(true);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.ActionListener#actionPerformed(java.awt.event.ActionEvent)
	 */
	public void actionPerformed(ActionEvent e) {
		//Clear the text components.
		txtEventInfo.setText("");
		
		//Return the focus to the window.
		this.requestFocusInWindow();	
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyTyped(java.awt.event.KeyEvent)
	 */
	public void keyTyped(KeyEvent e) {
		displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyPressed(java.awt.event.KeyEvent)
	 */
	public void keyPressed(KeyEvent e) {
		displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.KeyListener#keyReleased(java.awt.event.KeyEvent)
	 */
	public void keyReleased(KeyEvent e) {
		displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseClicked(java.awt.event.MouseEvent)
	 */
	public void mouseClicked(MouseEvent e) {
		displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mousePressed(java.awt.event.MouseEvent)
	 */
	public void mousePressed(MouseEvent e) {
			displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseListener#mouseReleased(java.awt.event.MouseEvent)
	 */
	public void mouseReleased(MouseEvent e) {
		displayEventInfo(e);
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.MouseMotionListener#mouseMoved(java.awt.event.MouseEvent)
	 */
	public void mouseMoved(MouseEvent e) {
		displayEventInfo(e);
	}
	
	/**
	 * Display event info.
	 *
	 * @param e the e
	 */
	private void displayEventInfo(KeyEvent e) {
		String sEventText = txtEventInfo.getText() + "\n\n";;
		
		switch (e.getID()) {
			case KeyEvent.KEY_PRESSED:
				sEventText += "KEY_PRESSED:\n";
			break;
			
			case KeyEvent.KEY_RELEASED:
				sEventText += "KEY_RELEASED:\n";
			break;
			
			case KeyEvent.KEY_TYPED:
				sEventText += "KEY_PRESSED:\n";
			break;
			
			default:
				sEventText += "KEY_UNKNOWN:\n";
		}
		
		sEventText += "\tKeyCode:\t" + e.getKeyCode() + "\n";
		sEventText += "\tKeyText:\t" + KeyEvent.getKeyText(e.getKeyCode()) + "\n";
		sEventText += "\tLocation:\t" + e.getKeyLocation() + "\n";
		sEventText += "\tModifiers:\t" + KeyEvent.getKeyModifiersText(e.getModifiers()) + "\n";
		sEventText += "\n";
		
		txtEventInfo.setText(sEventText);
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
	}
	
	/**
	 * Display event info.
	 *
	 * @param e the e
	 */
	private void displayEventInfo(MouseEvent e) {
		String sEventText = txtEventInfo.getText() + "\n\n";
		
		switch (e.getID()) {
			case MouseEvent.MOUSE_PRESSED:
				sEventText += "MOUSE_PRESSED:\n";
			break;
			
			case MouseEvent.MOUSE_RELEASED:
				sEventText += "MOUSE_RELEASED:\n";
			break;
			
			case MouseEvent.MOUSE_CLICKED:
				sEventText += "MOUSE_CLICKED:\n";
			break;
			
			default:
				sEventText += "MOUSE_UNKNOWN:\n";
		}
		
		sEventText += "\tButton:\t" + e.getButton() + "\n";
		sEventText += "\tModifiers:\t" + KeyEvent.getKeyModifiersText(e.getModifiers()) + "\n";
		sEventText += "\n";
		
		txtEventInfo.setText(sEventText);
		txtEventInfo.setCaretPosition(txtEventInfo.getDocument().getLength());
    }
    
	/**
	 * The main method.
	 *
	 * @param args the arguments
	 */
	public static void main(String[] args) {
		new NativeHookDemo();
	}
	
	/**
	 * The Class NoFocusTraversalPolicy.
	 */
	private class NoFocusTraversalPolicy extends FocusTraversalPolicy {
		
		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getComponentAfter(java.awt.Container, java.awt.Component)
		 */
		public Component getComponentAfter(Container container, Component component) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getComponentBefore(java.awt.Container, java.awt.Component)
		 */
		public Component getComponentBefore(Container container, Component component) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getDefaultComponent(java.awt.Container)
		 */
		public Component getDefaultComponent(Container container) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getFirstComponent(java.awt.Container)
		 */
		public Component getFirstComponent(Container container) {
			return null;
		}

		/* (non-Javadoc)
		 * @see java.awt.FocusTraversalPolicy#getLastComponent(java.awt.Container)
		 */
		public Component getLastComponent(Container container) {
			return null;
		}
	}

	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowActivated(java.awt.event.WindowEvent)
	 */
	public void windowActivated(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowClosing(java.awt.event.WindowEvent)
	 */
	public void windowClosing(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowDeactivated(java.awt.event.WindowEvent)
	 */
	public void windowDeactivated(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowDeiconified(java.awt.event.WindowEvent)
	 */
	public void windowDeiconified(WindowEvent e) { /* Do Nothing */ }
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowIconified(java.awt.event.WindowEvent)
	 */
	public void windowIconified(WindowEvent e) { /* Do Nothing */ }

	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowOpened(java.awt.event.WindowEvent)
	 */
	public void windowOpened(WindowEvent e) {
		GlobalScreen.getInstance();
		GlobalScreen.getInstance().addNativeKeyListener(this);		
	}
	
	/* (non-Javadoc)
	 * @see java.awt.event.WindowListener#windowClosed(java.awt.event.WindowEvent)
	 */
	public void windowClosed(WindowEvent e) {
		System.runFinalization();
		System.exit(0);
	}
}