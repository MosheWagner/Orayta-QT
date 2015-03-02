package xml;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;
import javax.xml.bind.Unmarshaller;

/*
 * This class reads and writes any compatible class to and from XML.
 * To make a class compatible, simple add '@XmlRootElement' to the top of it,
 * 	and any value with a getter and setter will be included in the XML.
 */

public class ClassIO <T>
{
	JAXBContext context = null;

	public ClassIO(Class<T> typeParameterClass) throws JAXBException
	{
		// get instance of JAXBContext based on root class
		context = JAXBContext.newInstance(typeParameterClass);
	}
	
	public void saveClassToFile(T obj, String filePath) throws JAXBException, IOException
	{
	    if (context != null)
	    {
		    // Marshall into XML
		    Marshaller marshaller = context.createMarshaller();
		    marshaller.setProperty( Marshaller.JAXB_FORMATTED_OUTPUT, true );
		    
		    // Write to File
		    File f = new File(filePath);
		    marshaller.marshal(obj, f);
	    }
	}
	
	public T readClassFromFile(String filePath) throws JAXBException, IOException
	{
		T newclass = null;
		
		Unmarshaller um = context.createUnmarshaller();
		
		//This shouldn't fail, since we should get an 'Unmarshaller'ing exception way before that can happen,
		// but because it's so bad to cast without safety measures, so here we go.
		try
		{
			newclass = (T) um.unmarshal(new FileReader(filePath));
		}
		catch (ClassCastException e) { }

	    return newclass;
	}
}
