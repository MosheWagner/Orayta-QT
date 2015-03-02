package settings;

import javax.xml.bind.annotation.*;

@XmlRootElement
public class GeneralSettings 
{
	String BOOKLIST_PATH;
	String BOOK_LIST_DOWNLOAD_PATH;
	String BOOKS_DOWNLOAD_PATH;

	public String getBOOKLIST_PATH() {
		return BOOKLIST_PATH;
	}

	public void setBOOKLIST_PATH(String iBOOKLIST_PATH) {
		BOOKLIST_PATH = iBOOKLIST_PATH;
	}
	
}
