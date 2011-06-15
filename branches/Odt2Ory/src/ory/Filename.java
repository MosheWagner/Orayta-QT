package ory;

import java.io.File;

import org.apache.commons.io.FilenameUtils;
//import org.apache.commons.io.IOUtils;


/* we will use the following words to define 
 * our arguments and variables:
 * for example take: c:\foo\bar\something.txt
 * filename- c:\foo\bar\something.txt
 * name- something.txt
 * base name- something
 * extension- txt
 * prefix- c:\
 * path- foo\bar\
 * full path- c:\foo\bar\
 * 		
 */

///** class to handle filenames and paths.*/
/**
 * class to handle filenames and paths,
 * based on apache utilities.
 * @author izar00@gmail.com
 * 
 */
public class Filename extends File {
	
	/**
	 * 
	 */
	private static final long serialVersionUID = -2113093790162098477L;
//	private String fullPath, baseName, extension; //  @deprecated.
	public static final char PATH_SEPARATOR = separatorChar;
	public static final char EXTENSION_SEPARATOR = FilenameUtils.EXTENSION_SEPARATOR; 
	
	
	/**
	 * 
	 * @param filename - full path filename
	 */
	Filename (String filename){
		super (filename);
//		fullPath = FilenameUtils.getFullPath(filename);
//		baseName = FilenameUtils.getBaseName(filename);
//		extension = FilenameUtils.getExtension(filename);
		
//		setLocales();
		
	}
	
	/**
	 * 
	 * @param path - the path of the file
	 * @param name - the name of the file, without the path.
	 */
	Filename (String path, String name){
		super(path, name);
		
//		fullPath = FilenameUtils.normalize(path); 
//		baseName = FilenameUtils.getBaseName(name);
//		extension = FilenameUtils.getExtension(name);
				
//		setLocales();
		
	}
	
	/**
	 * 
	 * @param path - file path.
	 * @param name - the base name, without extension or path
	 * @param extn - file extension e.g "txt"
	 */
	Filename (String path, String name, String extn){
		super(path, name + EXTENSION_SEPARATOR + extn);
		
//		fullPath = FilenameUtils.normalize(path); 
//		baseName = name;
//		extension = extn;
		
//		setLocales();
		
	}
	


	Filename(Filename filename) {
		super(filename.getFilename());
		
//		fullPath = filename.getFullPath();
//		baseName = filename.getBaseName();
//		extension = filename.getExtension();
		
	}

	public final char getPathSeparator() {
		return PATH_SEPARATOR;
	}

	public final char getExtensionSeparator() {
		return EXTENSION_SEPARATOR;
	}
	
	/**
	 * 
	 * @return the full filename including path of this file.
	 */
	public String getFilename() {
		return getAbsolutePath();
	}
	
//	@Override
//	public String toString() {
//		return getFilename();
//	}
	
	/**
	 * 
	 * @return the path to this file (without the filename)
	 */
	public String getFullPath() {
		return FilenameUtils.getFullPath(getAbsolutePath());
	}

//	public void setFullPath(String fullPath) {
//		this.fullPath = fullPath;
//	}
	
	/**
	 * 
	 * @return file name without path or extension.
	 */
	public String getBaseName() {
		return FilenameUtils.getBaseName(getName());
	}

//	public void setBaseName(String baseName) {
//		this.baseName = baseName;
//	}
	
	/**
	 * 
	 * @return the file name without the path.
	 */
//	public String getName() {
//		return baseName + EXTENTION_SEPARATOR + extension;
//	}

	public String getExtension() {
//		return extension;
		return FilenameUtils.getExtension(this.getName());
	}

//	public void setExtension(String extension) {
//		this.extension = extension;
//	}
	

}
