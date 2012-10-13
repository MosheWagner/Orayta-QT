package ory.fileTypes;

import java.io.File;

import ory.Filename;

public class DocFile extends MsFile implements FileType {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = 394873275655594137L;
	private final String myType = "Doc";

	public DocFile(String filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public DocFile(String path, String name) {
		super(path, name);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public DocFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public DocFile(Filename filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public DocFile(File file) {
		super(file);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public DocFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}


}
