package ory;

import javax.swing.JFileChooser;
import javax.swing.filechooser.FileFilter;

public class Gui {
	
	private String inputFilename;

	Gui () {
		
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
		else {
		inputFilename = fileChooser.getSelectedFile().getAbsolutePath();
		}
				
	}
	
	
	
	
	public String getFilename() {
		return inputFilename;
	}
	

}
