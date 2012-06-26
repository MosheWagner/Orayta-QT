package ory;

import java.awt.Dimension;
import java.io.File;

import javax.swing.ImageIcon;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTextArea;
//import javax.swing.filechooser.FileFilter;
import javax.swing.JPanel;
import javax.swing.text.IconView;
import javax.swing.BoxLayout;
import javax.swing.JScrollPane;
import java.awt.Component;
import javax.swing.JButton;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

public class Gui extends Utils {

	private JFrame frame;
	private JPanel mainPanel;
	private JTextArea textArea;
	private JPanel panel;
	private JButton btnNewButton;
	
	Gui () {
		
		frame = new JFrame("Giur - a converter for Orayta");
		mainPanel = new JPanel();
		ImageIcon oraytaIcon = new ImageIcon("images/Orayta.png");
		textArea = new JTextArea();
		textArea.setEditable(false);
		textArea.setAlignmentY(Component.BOTTOM_ALIGNMENT);
		textArea.append("main text Area" + "\n");
		textArea.setAutoscrolls(true);
		mainPanel.setLayout(new BoxLayout(mainPanel, BoxLayout.Y_AXIS));
		
		panel = new JPanel();
		panel.setAlignmentY(Component.TOP_ALIGNMENT);
		mainPanel.add(panel);
		panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
		JLabel bg = new JLabel(oraytaIcon);
		panel.add(bg);
		mainPanel.add(textArea);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setPreferredSize(new Dimension(500, 400));
		frame.setLocation(200, 150);
		frame.setIconImage(oraytaIcon.getImage());
		frame.getContentPane().add(mainPanel);
		
		btnNewButton = new JButton("Exit");
		btnNewButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		btnNewButton.setAlignmentX(Component.CENTER_ALIGNMENT);
		mainPanel.add(btnNewButton);
		frame.pack();
		frame.setVisible(true);
		
		
	} 
	
	public String getInputFile() {

		//		String defaultPath = OryFile.getOraytaDir();
		String defaultPath = System.getProperty("user.dir");
		JFileChooser fileChooser = new JFileChooser(defaultPath);
		String title = "בחר קובץ להמרה";
		fileChooser.setDialogTitle(title);
		//		FileFilter filter =  FileFilter();
		//		fileChooser.setFileFilter(filter ); //TODO: create file filter.
		
		fileChooser.setFileSelectionMode(JFileChooser.FILES_AND_DIRECTORIES);
		if (Main.parameters.getInputFilename() != null)
			fileChooser.setSelectedFile(new File(Main.parameters.getInputFilename()));
		
		mainPanel.add(fileChooser);
		int option = fileChooser.showOpenDialog(mainPanel);
		//mainPanel.repaint();

		if (option != JFileChooser.APPROVE_OPTION) {
			System.exit(0); 
		}


		String inputFilename = fileChooser.getSelectedFile().getAbsolutePath();

		return inputFilename;

	}

	public String getUserInput(String string) {
		return JOptionPane.showInputDialog(string);
	}
	
	@Override
	public void message (String str){
		JOptionPane.showMessageDialog(mainPanel,str);
	}
	
	@Override
	public void errorMessage (String str, Exception e){
		JOptionPane.showMessageDialog(mainPanel,str + "\n" + e);
	}
	
	@Override
	public void errorMessage (String str){
		JOptionPane.showMessageDialog(mainPanel,str);
	}

//	@Override
//	void launchInfo(String inputFilename){}
	public void log(String string) {
		textArea.append(string + "\n");
		textArea.repaint();
		
	}

}
