package ory.fileTypes;

import java.io.File;

import ory.Filename;

public class RtfFile extends MsFile implements FileType {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -3121945460882682744L;
	private final String myType = "Rtf";

	public RtfFile(String filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public RtfFile(String path, String name) {
		super(path, name);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public RtfFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public RtfFile(Filename filename) {
		super(filename);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public RtfFile(File file) {
		super(file);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

	public RtfFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
		// TODO Auto-generated constructor stub
	}

}
