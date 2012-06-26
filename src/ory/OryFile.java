package ory;




import java.io.IOException;
import java.util.zip.ZipFile;
//import java.util.Arrays;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;

import ory.fileTypes.ObkFile;



public class OryFile {

	// variables:
	private Filename inputFilename, myFilename; //filename;
//	private OryConfFile myConf;
//	StringBuffer TextBuffer, tempString;
	private String bookTitle;
	private final String EXTENSION= "txt";
	private StringBuffer fileText;
	private final static char pathSeparator = Filename.PATH_SEPARATOR;
	private int[] headings;
	private OryConfFile myConf;
	
	public OryFile (Filename path) {
		
		
		inputFilename = new Filename (path);
		myFilename = new Filename (path.getFullPath(), path.getBaseName(), EXTENSION);
		
		fileText = new StringBuffer(); //set for empty file.
		
		headings = new int[1]; //set for empty file.

		makeConf(myFilename);
		
		
	}
	
	
	/**
	 * set up the conf file for this ory file.
	 * @throws IOException 
	 */
	private void makeConf(Filename filename) {
		myConf = new OryConfFile(filename); 
		
		
		if (Main.parameters.getUid().isEmpty()){
			myConf.setAutoUid();
		}
		else {
			myConf.setAutoUid(Main.parameters.getUid());
		}
		
		if (Main.parameters.getSource() != null)
			myConf.setBookSource(Main.parameters.getSource());
		
		if (Main.parameters.getBookTitle() != null)
			myConf.setBookName(Main.parameters.getBookTitle());
		
		
	}

//	@Deprecated
//	private void save(Filename path) throws IOException {
//				
//		
//		String encoding = "utf-8";
//		FileUtils.writeStringToFile(path, fileText.toString(), encoding);
//		
//	
//	}
	
	/**
	 * saves the file to an automatically generated location.
	 * @return the Filename object of the saved file. 
	 * @throws IOException
	 */
	public Filename save() throws IOException{
	
		if (Main.parameters.isOutAsIn()){
			Main.parameters.setOutputPath(inputFilename.getFullPath());
		}
		else if (Main.parameters.getOutputPath() == null){
			Main.parameters.setOutputPath(getOraytaDir());
		}

		/* TODO: save to a ziped .obk archive:
		 * generate search database;
		 * save filetext and DB to tempfiles;
		 * get conf as a string. (myConf.getText())
		 * create zipfile with three temp files.
		 * create zipnote wite conf text.
		 * save zip arcive. (-9)
		 * remove tempfiles.
		 */
		
		//check if filename is available
		Filename savedFile = Main.ui.checkExistance(myFilename);
		if (savedFile== null)
			return null;
		
		ObkFile obk = new ObkFile(savedFile);
		obk.setText(fileText.toString());
		obk.setConfText(myConf.getText());
		
		obk.save();
		
		return savedFile;
		
		/* original code:
		Filename savedFile = Main.ui.saveStringToFile(fileText.toString(), myFilename);
		if (savedFile== null)
			return null;
		
		setPath(savedFile.getFullPath());
		setName(savedFile.getBaseName());
		
		myConf.save();
		
		
		return myFilename;*/
	}
	
	


	public String getBookTitle() {
		return bookTitle;
	}

	public void setBookTitle(String bookTitle) {
		this.bookTitle = bookTitle;
		myConf.setBookName(this.getBookTitle());
	}
	
	public static String getOraytaDir() {
		String home = FileUtils.getUserDirectoryPath();
		String oraytaDir = ".Orayta"+ pathSeparator + "Books" + pathSeparator;
		String path = FilenameUtils.concat(home, oraytaDir);
		return path;
	}

	public Filename getInputFilename() {
		return inputFilename;
	}

	public void setInputFilename(Filename input) {
		this.inputFilename = input;
	}

//	public String getInputFilename() {
//		return inputFilename;
//	}
//
//	public void setInputFilename(String inputFilename) {
//		this.inputFilename = inputFilename;
//	}
	
	/**
	 * set the directory for this book.
	 */
	public void setPath(String path){
		myFilename = new Filename(path, myFilename.getBaseName(), EXTENSION);
		if (myConf != null)
			myConf.setPath(path); //make sure the conf is in the same place as the book.
	}
	
	/**
	 * set the file base name for this book, without changing the directory or extension.
	 * @param name
	 */
	public void setName(String name){
		myFilename = new Filename(myFilename.getFullPath(), name , EXTENSION);
		if (myConf != null)
			myConf.setName(name); //make sure the conf has the same name as the book.
	}
	
	public void setFileText(CharSequence text)  {
		fileText = new StringBuffer(text);
	}

	public String getFileText() {
		return fileText.toString();
	}


	/**
	 * @return the headings
	 */
	public int[] getHeadings() {
		return headings;
	}


	/** set the heading levels use in this file.
	 * @param headings the headings to set
	 */
	public void setHeadings(int[] headings) {
		this.headings = headings;
		
		//make the conf file match the headings.
		//if we have three levels of headings or more, or if we have two level of which the lower level appears many times, we want a secondary index table.
		if (numberOfLevelsInUse() > 2 ||
				(numberOfLevelsInUse() == 2 && headings[highestHeading()] > 20))
			myConf.addSecondaryIndex(highestHeading());
	}
	
	int numberOfLevelsInUse() {
		int count = 0;
		
		for (int level : headings){
			if (level > 0)
				count++;
		}
		
		return count;
	}
	
	public OryConfFile getMyConf() {
		return myConf;
	}


	public void setMyConf(OryConfFile myConf) {
		this.myConf = myConf;
	}


	private int highestHeading (){
		int i = 0, level = headings[i];
		while (i < headings.length && level < 1){
			i++;
			level = headings[i];
			
//			Odt2Ory.dbgLog("i=" + i + " level=" + level);
			
		}
//		Odt2Ory.dbgLog("i=" + i + " level=" + level);
		int result = i+1;
//		Odt2Ory.dbgLog("highestHeading=" + result);
//		Odt2Ory.dbgLog(Arrays.toString(headings));
		return result;
	}

//	public void setFileText(String fileText) {
//		this.fileText = fileText;
//	}
	
//	/**
//	 * WORNINIG! you must use one of the save methods before using this.
//	 * @return the Filename object where this was saved.
//	 */
//	public Filename getFilename() {
//		return filename;
//	}
//	
//	public void setFilename(Filename filename) {
//		this.filename = filename;
//	}
	
	

}


	
	
	

