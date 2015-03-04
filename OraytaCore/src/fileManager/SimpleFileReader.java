package fileManager;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;

public class SimpleFileReader implements IFileReader
{

	public String readContents(String path) throws IOException, FileNotFoundException 
	{
		String contents = "";

    	BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(path), "UTF8"));
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
