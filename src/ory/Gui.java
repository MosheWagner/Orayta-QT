package ory;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
//import javax.swing.filechooser.FileFilter;

public class Gui extends Odt2Ory {

	private String inputFilename;

	Gui () throws Exception {

		//		String defaultPath = OryFile.getOraytaDir();
		String defaultPath = System.getProperty("user.dir");
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


		inputFilename = fileChooser.getSelectedFile().getAbsolutePath();

		this.process(inputFilename);

	}


	public String getFilename() {
		return inputFilename;
	}

	@Override
	void message (String str){
		JOptionPane.showMessageDialog(null,str);
	}
	
	@Override
	void errorMessage (String str, Exception e){
		JOptionPane.showMessageDialog(null,str + "\n" + e);
	}

	@Override
	void launchInfo(String inputFilename){}


}
