package ory;




import java.io.IOException;

import org.apache.commons.io.FileUtils;
import org.apache.commons.io.FilenameUtils;



public class OryFile {

	// variables:
	private Filename input, filename; 
//	StringBuffer TextBuffer, tempString;
	private String bookTitle, inputFilename;
	private StringBuffer fileText;
	private final static char pathSeparator = Filename.PATH_SEPARATOR;
	
	OryFile (Filename path) throws Exception {
		
		
		input = new Filename (path);
		
		fileText = new StringBuffer();
		
		OryFileExtractor extractor = OryFileExtractor.newOryFileExtractor(input);
		fileText.append(extractor.getText()) ;
		
		String notes = extractor.getNotes();
		if (! notes.isEmpty()){
			int highestHeading = extractor.getHighestHeading(); 
			
			// make sure we have a valid value.
			if (highestHeading < 1 || highestHeading > 4)
				highestHeading = 4;
			
			fileText.append("\n");
			fileText.append(OryFileExtractor.headingSymbol(highestHeading));
			
			fileText.append("הערות שוליים" + "\n"); 
			fileText.append(notes);
		}
		
		bookTitle = extractor.getBookTitle();
		
	}
	
	
	private void save(Filename path) throws IOException {
				
//		File file = new File(path.getFilename());
		String encoding = "utf-8";
		FileUtils.writeStringToFile(path, fileText.toString(), encoding);
		
	
	}
	
	public void save() throws IOException{
		if (Main.parameters.getOutputPath() == null){
			Main.parameters.setOutputPath(getOraytaDir());
		}
		filename = new Filename(Main.parameters.getOutputPath(), input.getBaseName(), "txt");
		save(filename);
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
		/* this came to cause trouble, because bookTitle may contain characters that aren't acceptable by file system.
		 * TODO: fix?
		 */
		
		String baseName = input.getBaseName();
		Filename myFilename = new Filename (outPath, baseName, "txt");

		save(myFilename);
		
		setFilename(myFilename);
		
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

	public String getFileText() {
		return fileText.toString();
	}

//	public void setFileText(String fileText) {
//		this.fileText = fileText;
//	}
	
	/**
	 * WORNINIG! you must use one of the save methods before using this.
	 * @return the Filename object where this was saved.
	 */
	public Filename getFilename() {
		return filename;
	}
	
	public void setFilename(Filename filename) {
		this.filename = filename;
	}
	
	

}


	
	
	

