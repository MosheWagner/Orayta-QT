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
			fileNames[i] = fileList.get(i).save().getAbsolutePath();
		}
		
		return fileNames;
	}
	
	
	
}
