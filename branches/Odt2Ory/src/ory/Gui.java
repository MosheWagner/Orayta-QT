package ory;

import java.io.FileNotFoundException;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
//import javax.swing.filechooser.FileFilter;

public class Gui {
	
	private String inputFilename;

	Gui () throws Exception {
		
//		String defaultPath = OryFile.getOraytaDir();
		String defaultPath = ".";
		JFileChooser fileChooser = new JFileChooser(defaultPath);
		String title = "בחר קובץ להמרה";
		fileChooser.setDialogTitle(title);
//		FileFilter filter =  FileFilter();
//		fileChooser.setFileFilter(filter ); //TODO: create file filter.
		
		fileChooser.setSelectedFile(null);
		int option = fileChooser.showOpenDialog(null);
		
		if (option != JFileChooser.APPROVE_OPTION) {
			return; //TODO: create an exit() method?
		}
		
		try {
			inputFilename = fileChooser.getSelectedFile().getAbsolutePath();
			new Odt2Ory(inputFilename);
		} catch (FileNotFoundException e) {
			/* TODO: handle exception
			 * should look like this: */
			String message = ("File not found" + "\n" + "הקובץ לא נמצא" + "\n");
			JOptionPane.showMessageDialog(null,message + e);
			return;
			
		} catch (Exception e) {
			String message = ("an error occured" + "\n" + "אירעה שגיאה" + "\n");
			JOptionPane.showMessageDialog(null,message + e);
			return;
			
		}
		
		success();		
	}
	
	
	
	
	private void success() {
		String message = ("oporation completed successfully" + "\n" + "הפעולה הסתיימה בהצלחה" + "\n");
		JOptionPane.showMessageDialog(null,message);
		return;
		
	}




	public String getFilename() {
		return inputFilename;
	}
	

}
