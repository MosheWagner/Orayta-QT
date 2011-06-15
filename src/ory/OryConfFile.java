package ory;

import java.io.IOException;
import java.util.Date;

import org.apache.commons.io.FileUtils;

/**
 * class for creating a .conf (configuration) file
 * for an orayta book.
 * @author izar00@gmail.com
 *
 */
public class OryConfFile {
	private Filename filename;
	private StringBuffer text;
	private final int UID_MIN = 0;
	private final int UID_MAX = 40000;
	
	/**
	 * 
	 * @param file - the Filename object for the ory file for which this conf is meant.
	 */
	OryConfFile (Filename file) {
		
		String path = file.getFullPath();
		String baseName = file.getBaseName();
		String extension = "conf";
		text = new StringBuffer();
		
		filename = new Filename (path, baseName, extension);
		
		setAutoUid();
	}
	
	/**
	 * creates a unique id entry
	 * with a number generated from the path
	 * of this conf file.
	 * NOTICE: you may get a negative uid.
	 */
	private void setAutoUid() {
		String str = (filename.getFilename() + new Date()); // using date to decrease probability of books with the same uid.
		int num = str.hashCode();
		
		//make sure our number isn't in the range that is in use.
		if (num > UID_MIN && num < UID_MAX)
			num += UID_MAX;
		
		setUID(num);
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

	public void setUID(int num) {
		String uid = Integer.toString(num);
		setUID(uid);
	}

	public Filename getFilename() {
		return filename;
	}

	public void setFilename(Filename filename) {
		this.filename = filename;
	}

	public String getText() {
		return text.toString();
	}

	public void save() throws IOException {
		
//		File file = new File(filename.getFilename());
		String encoding = "utf-8";
//		System.out.println("the text is:");
//		System.out.println(text);
		FileUtils.writeStringToFile(filename, text.toString(), encoding);
		
	}
	

}
