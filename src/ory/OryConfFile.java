package ory;

import java.io.File;
import java.io.IOException;

import org.apache.commons.io.FileUtils;

public class OryConfFile {
	private Filename filename;
	private StringBuffer text;
	
	OryConfFile (Filename file) {
		
		String path = file.getFullPath();
		String baseName = file.getBaseName();
		String extension = "conf";
		text = new StringBuffer();
		
		filename = new Filename (path, baseName, extension);
	}
	
	void addEntry (String label, String value){
		String str = new String (label + "=" + value + "\n");
//		System.out.println("to add: " + str);
		text.append(str);
//		System.out.println("added.");
	}
	
	public void setBookName(String bookName){
		addEntry("DisplayName", bookName);
	}
	
	public void setBookSource(String source){
		addEntry("TextSource", source);
	}
	
	public void setUID(String uid){
		addEntry("UniqueId", uid);
	}

	public void save() throws IOException {
		
		File file = new File(filename.getFilename());
		String encoding = "utf-8";
//		System.out.println("the text is:");
//		System.out.println(text);
		FileUtils.writeStringToFile(file, text.toString(), encoding);
		
	}
	

}
