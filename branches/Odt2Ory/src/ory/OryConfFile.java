package ory;

import java.io.IOException;
//import java.util.Date;

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
	private static int uid ;
//	private boolean publicFile; //if this is a public file we want to use a uid of our own range.
	
	/**
	 * 
	 * @param file - the Filename object for the ory file for which this conf is meant.
	 */
	OryConfFile (Filename file) {
		
		String path = file.getFullPath();
		String baseName = file.getBaseName();
		String extension = "conf";
		
		filename = new Filename (path, baseName, extension);
		
		text = new StringBuffer();
		
//		setAutoUid();
	}
	
	void addEntry (String label, String value){
		String str = new String (label + "=" + value + "\n");
		text.append(str);
	}
	
	void addEntry (String label, int num){
		String value = Integer.toString(num);
		addEntry(label, value);
		
	}
	
	/**
	 * adds a secondary index menu in the beginning of the file when displayed in oryata.
	 */
	public void addSecondaryIndex(int level){
		String label = "CosmeticsType";
		String cosmetics = "#sil=" + level2TE(level);
		addEntry(label, cosmetics);
	}
	
	/**
	 * translates from my heading levels to torat emet style level. i.e. my level 1 is TE level 5, my level 5 is TE level 1.
	 * @param in - level to translate
	 * @return Torat Emet style level
	 */
	public static int level2TE (int in) {
		if (in < 1 || in > 5 ) { //make sure we have a valid level
			Odt2Ory.dbgLog("wrong level: " + in);
			return 0;
		}
		
		return 6 - in;
	}
	
	public void setBookName(String bookName){
		addEntry("DisplayName", bookName);
	}
	
	public void setBookSource(String source){
		addEntry("TextSource", source);
	}
	
	/**
	 * creates a unique id entry
	 * with a number generated from the path
	 * of this conf file.
	 * NOTICE: you may get a negative uid.
	 */
	public void setAutoUid() {
		String str = (filename.getAbsolutePath()); 
		int num = str.hashCode();
		
		//make sure our number isn't in the range that is in use.
		if (num > UID_MIN && num < UID_MAX)
			num += UID_MAX;
		
		setUID(num);
	}

	public void setAutoUid(String uid){
		setAutoUid( Integer.parseInt(uid));
		
	}
	
	/**
	 * this will set the uid for the first file processed, then increase the uid by 1 for every following file.
	 * @param num - uid for first file in batch.
	 */
	public void setAutoUid(int num) {
		
		if(OryConfFile.uid == 0)
			OryConfFile.uid = num;
		else {
			num = OryConfFile.uid;
		}
		OryConfFile.uid++; //increase uid by 1 for every input file.
		
		
		
		setUID (num);
		
	}

	
	private void setUID(int num) {
		
		if (num >= UID_MIN && num <= UID_MAX){
			Odt2Ory.log ("warning: pussible violation of uid bounderies\n" +
					"using uid: " + num);
		}
			
		addEntry("UniqueId", num);
		
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
