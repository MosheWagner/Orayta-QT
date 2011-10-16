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
	private static final String EXTENSION = "conf";
	protected Filename myFilename;
	private StringBuffer text;
	private final int UID_MIN = 0;
	private final int UID_MAX = 40000;
	private static int globalUid ;
	private int myUid;
	private OryLinkFile link;
	private String displayName = "";
//	private boolean publicFile; //if this is a public file we want to use a uid of our own range.
	
	/**
	 * 
	 * @param file - the Filename object for the ory file for which this conf is meant.
	 */
	OryConfFile (Filename file) {
		
		String path = file.getFullPath();
		String baseName = file.getBaseName();
//		String extension = "conf";
		
		myFilename = new Filename (path, baseName, EXTENSION);
		
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
			Main.ui.dbgLog("wrong level: " + in);
			return 0;
		}
		
		return 6 - in;
	}
	
	public void setBookName(String bookName){
		addEntry("DisplayName", bookName);
		displayName = bookName;
		//TODO: create a hash object with entries or some other method to be coherent.
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
		String str = (myFilename.getAbsolutePath()); 
		int num = str.hashCode();
		
		//make sure our number isn't in the range that is in use.
		if (num > UID_MIN && num < UID_MAX)
			num += UID_MAX;
		myUid = num;
		setUID();
	}

	public void setAutoUid(String uid){
		setAutoUid( Integer.parseInt(uid));
		
	}
	
	/**
	 * this will set the uid for the first file processed, then increase the uid by 1 for every following file.
	 * @param num - uid for first file in batch.
	 */
	public void setAutoUid(int num) {
		
		if(OryConfFile.globalUid == 0)
			OryConfFile.globalUid = num;
		else {
			num = OryConfFile.globalUid;
		}
		OryConfFile.globalUid++; //increase uid by 1 for every input file.
		
		
		myUid = num;
		setUID ();
		
	}

	
	private void setUID() {
		
		if (myUid >= UID_MIN && myUid <= UID_MAX){
			Main.ui.log ("warning: pussible violation of uid bounderies\n" +
					"using uid: " + myUid);
		}
			
		addEntry("UniqueId", myUid);
		Main.ui.dbgLog("uid: " + myUid + " in use");
		
	}

	public static int getGlobalUid() {
		return globalUid;
	}

	public static void setGlobalUid(int globalUid) {
		OryConfFile.globalUid = globalUid;
	}

	public int getMyUid() {
		return myUid;
	}

	public void setMyUid(int myUid) {
		this.myUid = myUid;
	}

	public void setText(StringBuffer text) {
		this.text = text;
	}

	public Filename getFilename() {
		return myFilename;
	}

	public void setFilename(Filename filename) {
		this.myFilename = filename;
	}

	public String getText() {
		return text.toString();
	}

	public void save() throws IOException {
		
		Main.ui.saveStringToFile(text.toString(), myFilename);
		
		if (link != null)
			link.save(); //TODO: what happens if conf name was changed.
	}

	/**
	 * create a link file for this book.
	 * @param path - where to create the link.
	 */
	public void createLink(String path) {
		link = new OryLinkFile(new Filename(path, myFilename.getBaseName(), myFilename.getExtension()));
		link.setTarget(myUid);
		
		if (displayName != null)
			link.setBookName(displayName);
	}
	
	/**
	 * set the directory for this book.
	 */
	public void setPath(String path) {
		myFilename = new Filename(path, myFilename.getBaseName(), this.getExtension());
	}
	
	/**
	 * set the file base name for this book, without changing the directory or extension.
	 * @param name
	 */
	public void setName(String name){
		myFilename = new Filename(myFilename.getFullPath(), name , this.getExtension());
		if (link != null)
			link.setName(name); //make sure the conf has the same name as the book.
	}

	public static String getExtension() {
		return EXTENSION;
	}

}
