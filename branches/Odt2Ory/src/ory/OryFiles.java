package ory;

import java.io.IOException;
import java.util.ArrayList;

public class OryFiles {
	
	private ArrayList<OryFile> fileList;
	
	public OryFiles() {
		fileList = new ArrayList<OryFile>();
	}
	
	public void add (OryFile oryFile) {
		fileList.add(oryFile);
	}
	
	public String[] save() throws IOException{
		
		String[] fileNames = new String[fileList.size()];
		for (int i=0; i<fileList.size(); i++){
			Filename file = fileList.get(i).save();
			if (file == null)
				fileNames[i] = "<this file was skipped>";
			else
				fileNames[i] = file.getAbsolutePath();
		}
		
		return fileNames;
	}

	public ArrayList<OryFile> getFileList() {
		return fileList;
	}

	public void setFileList(ArrayList<OryFile> fileList) {
		this.fileList = fileList;
	}
	
	
	
}
