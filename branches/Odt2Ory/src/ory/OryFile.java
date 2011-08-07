package ory;




import java.io.IOException;
//import java.util.Arrays;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;



public class OryFile {

	// variables:
	private Filename input; //filename;
//	private OryConfFile myConf;
//	StringBuffer TextBuffer, tempString;
	private String bookTitle, inputFilename;
	private StringBuffer fileText;
	private final static char pathSeparator = Filename.PATH_SEPARATOR;
	private int[] headings;
	
	public OryFile (Filename path) {
		
		
		input = new Filename (path);
		
		fileText = new StringBuffer(); //set for empty file.
		
		headings = new int[1]; //set for empty file.
//		
//		OryFileExtractor extractor = OryFileExtractor.newOryFileExtractor(input);
//		fileText.append(extractor.getText()) ;
//		
//		String notes = extractor.getNotes();
//		if (! notes.isEmpty()){
//			int highestHeading = extractor.getHighestHeading(); 
//			
//			// make sure we have a valid value.
//			if (highestHeading < 1 || highestHeading > 4)
//				highestHeading = 4;
//			
//			fileText.append("\n");
//			fileText.append(OryFileExtractor.headingSymbol(highestHeading));
//			
//			fileText.append("הערות שוליים" + "\n"); 
//			fileText.append(notes);
//		}
//		
//		bookTitle = extractor.getBookTitle();
		
		
		
	}
	
	
	/**
	 * set up the conf file for this ory file.
	 * @throws IOException 
	 */
	private void makeConf(Filename filename) throws IOException {
		OryConfFile myConf = new OryConfFile(filename); 
		
		
		myConf.setBookName(this.getBookTitle());
		
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
		
		//if we have three levels of headings or more, or if we have two level of which the lower level appears many times, we want a secondary index table.
		if (numberOfLevelsInUse() > 2 ||
				(numberOfLevelsInUse() == 2 && headings[highestHeading()] > 20))
			myConf.addSecondaryIndex(highestHeading());

		myConf.save();

	}


	private void save(Filename path) throws IOException {
				
//		File file = new File(path.getFilename());
		String encoding = "utf-8";
		FileUtils.writeStringToFile(path, fileText.toString(), encoding);
		
	
	}
	
	public Filename save() throws IOException{
		if (Main.parameters.getOutputPath() == null){
			Main.parameters.setOutputPath(getOraytaDir());
		}
		Filename filename = new Filename(Main.parameters.getOutputPath(), input.getBaseName(), "txt");
		save(filename);
		
		makeConf(filename);
		return filename;
	}
	
	public String getBookTitle() {
		return bookTitle;
	}

	public void setBookTitle(String bookTitle) {
		this.bookTitle = bookTitle;
	}

	/**
	 * @deprecated
	 * @return
	 * @throws IOException
	 */
	public Filename saveToHere()throws IOException {
		String outPath = input.getFullPath();
//		String baseName = bookTitle; //maybe we should switch to this later.
		String baseName = input.getBaseName();
		Filename myFilename = new Filename (outPath, baseName, "txt");
		
		save(myFilename);
		
		/*
		 * this is a temporary workaround, we get a problem of aliasing
		 * objects instead of copying.
		 * TODO: fix this.
		 */
		return myFilename;
		
	}
	
	/**
	 * @deprecated
	 * @return
	 * @throws IOException
	 */
	public Filename saveToOryDir()throws IOException {
		String outPath =  getOraytaDir();
//			FileUtils.getUserDirectoryPath() + pathSeparator + ".Orayta" +
//			pathSeparator + "Books" + pathSeparator;
//		String baseName = bookTitle; 
		
		
		String baseName = input.getBaseName();
		Filename myFilename = new Filename (outPath, baseName, "txt");

		save(myFilename);
		
//		setFilename(myFilename);
		
		/*
		 * this is a temporary workaround, we get a problem of aliasing
		 * objects instead of copying.
		 * TODO: fix this.
		 */
		return myFilename;
		
	}

	public static String getOraytaDir() {
		String home = FileUtils.getUserDirectoryPath();
		String oraytaDir = ".Orayta"+ pathSeparator + "Books" + pathSeparator;
		String path = FilenameUtils.concat(home, oraytaDir);
		return path;
	}

	public Filename getInput() {
		return input;
	}

	public void setInput(Filename input) {
		this.input = input;
	}

	public String getInputFilename() {
		return inputFilename;
	}

	public void setInputFilename(String inputFilename) {
		this.inputFilename = inputFilename;
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
	}
	
	int numberOfLevelsInUse() {
		int count = 0;
		
		for (int level : headings){
			if (level > 0)
				count++;
		}
		
		return count;
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


	
	
	

