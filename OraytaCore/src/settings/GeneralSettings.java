package settings;

import javax.xml.bind.annotation.*;

@XmlRootElement
public class GeneralSettings 
{
	String BOOKLIST_PATH;
	String foo="fhgfd";

	public String getFoo() {
		return foo;
	}

	public void setFoo(String foo) {
		this.foo = foo;
	}

	public String getBOOKLIST_PATH() {
		return BOOKLIST_PATH;
	}

	public void setBOOKLIST_PATH(String iBOOKLIST_PATH) {
		BOOKLIST_PATH = iBOOKLIST_PATH;
	}
	
}
