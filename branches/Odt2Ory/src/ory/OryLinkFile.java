package ory;

public class OryLinkFile extends OryConfFile {
	
	private static final String EXTENSION = "link";
	
	public OryLinkFile(Filename inputFile){
		super (inputFile);
		String path = inputFile.getFullPath();
		String baseName =inputFile.getBaseName();
		
		
		myFilename = new Filename (path, baseName, EXTENSION);
	}
	
	public void setTarget(String target){
		addEntry("Link", target);
	}
	
	public void setTarget(int target){
		addEntry("Link", target);
	}
	
	/**
	 * where we want to save the link
	 * @param path - where we want to save the link
	 */
	public void setLocation(Filename path){
		Filename location = new Filename (path.getFullPath(), myFilename.getBaseName(), myFilename.getExtension());
		myFilename = location;
	}
	
	/**
	 * set the directory for this book.
	 */
	public void setPath(String path) {
		myFilename = new Filename(path, myFilename.getBaseName(), getExtension());
	}
	
	/**
	 * set the file base name for this book, without changing the directory or extension.
	 * @param name
	 */
	public void setName(String name){
		myFilename = new Filename(myFilename.getFullPath(), name , getExtension());
		
	}
	
	public static String getExtension() {
		return EXTENSION;
	}

}
