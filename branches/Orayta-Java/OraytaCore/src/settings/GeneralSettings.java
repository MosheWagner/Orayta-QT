package settings;

import javax.xml.bind.annotation.*;

@XmlRootElement
public class GeneralSettings 
{
	public String BOOKS_ROOT_DIR = "/home/moshe/Orayta/books/";
	String BOOK_LIST_DOWNLOAD_PATH;
	String BOOKS_DOWNLOAD_PATH;
}
