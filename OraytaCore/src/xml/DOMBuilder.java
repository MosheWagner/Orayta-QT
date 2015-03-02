package xml;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

/*
 * Reads an XML file, and returns a org.w3c.dom.Document .
 * This class expects a valid XML file. Fooling with it doesn't give impressive results.
 */

public class DOMBuilder 
{
	public Document buildDocFromFile(String filePath, boolean validate) throws ParserConfigurationException, SAXException, IOException
	{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		
		factory.setValidating(validate);
		factory.setIgnoringElementContentWhitespace(true);
		
		javax.xml.parsers.DocumentBuilder builder = factory.newDocumentBuilder();
		
		File file = new File(filePath);
		Document doc = builder.parse(file);

		return doc;
	}
}
