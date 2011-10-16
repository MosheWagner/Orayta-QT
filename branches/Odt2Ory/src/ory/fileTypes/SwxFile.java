package ory.fileTypes;

import java.io.File;

import ory.Filename;

public class SwxFile extends MsFile implements FileType {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -6840682361652130557L;
	private final String myType = "Swx";

	public SwxFile(String filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public SwxFile(String path, String name) {
		super(path, name);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public SwxFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public SwxFile(Filename filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public SwxFile(File file) {
		super(file);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public SwxFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

}
