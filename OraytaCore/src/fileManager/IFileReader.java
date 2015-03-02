package fileManager;

import java.io.FileNotFoundException;
import java.io.IOException;

public interface IFileReader 
{
	public String readContents(String filePath) throws IOException, FileNotFoundException;
}
