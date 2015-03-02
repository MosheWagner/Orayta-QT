package fileManager;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class SimpleFileReader implements IFileReader
{

	public String readContents(String path) throws IOException, FileNotFoundException 
	{
		String contents = "";

    	BufferedReader br = new BufferedReader(new FileReader(path));
        StringBuilder sb = new StringBuilder();
        String line = br.readLine();

        while (line != null) {
            sb.append(line);
            sb.append(System.lineSeparator());
            line = br.readLine();
        }
        contents = sb.toString();
        
        br.close();

	    return contents;
	}
}
