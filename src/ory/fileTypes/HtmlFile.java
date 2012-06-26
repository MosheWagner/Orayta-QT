package ory.fileTypes;

import java.io.File;
import java.io.FileNotFoundException;

import ory.Filename;
import ory.HtmlToOryExtractor;
import ory.Main;
import ory.OryConfFile;

import ory.OryFiles;

public class HtmlFile extends MsFile implements FileType {

	/**
	 * 
	 */
	private static final long serialVersionUID = -4002842816545890189L;
	private final String myType = "Html";
	
	
	public HtmlFile(String filename) {
		super(filename);
		setFileType(myType);
	}

	public HtmlFile(String path, String name) {
		super(path, name);
		setFileType(myType);
	}

	public HtmlFile(String path, String name, String extn) {
		super(path, name, extn);
		setFileType(myType);
	}

	public HtmlFile(Filename filename) {
		super(filename);
		setFileType(myType);
	}

	public HtmlFile(File file) {
		super(file);
		setFileType(myType);
	}

	public HtmlFile(File parent, String child) {
		super(parent, child);
		setFileType(myType);
	}
	
	//TODO: create option for old ooo conversion.
	
	
	public String[] convertToOrayta() throws FileNotFoundException, Exception {
		// TODO make this more elegant.
		OryFiles oryFiles;
		if (ory.Main.parameters.isDaat()) {
			oryFiles = DaatHtmlToOryExtractor.walkThrough(this);
			Main.ui.dbgLog("processing daat");
		}
		else {
//			oryFiles = HtmlToOryExtractor.walkThrough(this);
			return super.convertToOrayta();
		}
		
		String[] fileNames = oryFiles.save();
		return fileNames;
		
	}
	

}
