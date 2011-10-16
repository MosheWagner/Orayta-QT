package ory.fileTypes;

import java.io.FileNotFoundException;

public interface FileType {

	String[] convertToOrayta() throws FileNotFoundException, Exception;
	
}
