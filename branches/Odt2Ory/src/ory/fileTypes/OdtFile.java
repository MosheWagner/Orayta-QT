package ory.fileTypes;



import java.io.File;
import java.io.FileNotFoundException;

import ory.Filename;
import ory.OryFileExtractor;
import ory.OryFiles;

public class OdtFile extends Filename implements FileType {
	
	private final String myType = "Odt";
	/**
	 * 
	 */
	private static final long serialVersionUID = 626494139307112155L;

	public OdtFile(String filename){
		super (filename);
		setFileType(myType);
	}

	public OdtFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
	}

	public OdtFile(File file) {
		super(file);
		setFileType(myType);
	}

	public OdtFile(Filename filename) {
		super(filename);
		setFileType(myType);
	}

	public OdtFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
	}

	public OdtFile(String path, String name) {
		super(path, name);
		setFileType(myType);
	}

	public String[] convertToOrayta() throws FileNotFoundException, Exception {
		// TODO make this more elegant.
		OryFiles oryFiles = OryFileExtractor.walkThrough(this);
		String[] fileNames = oryFiles.save();
		 return fileNames;
		
	}
	

}
