package tester;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import xml.DOMBuilder;

/*
 * This class initiates code tests, as needed while developing.
 *  DON'T USE THIS FOR PRODUCTION  
 */
public class TestRunner 
{
	public static void main(String[] args)
	{
		runTest();
	}
	
	private static void runTest()
	{
		ITest stupidTest = new ITest()
		{
			String s = "";
			
			public void Run()
			{

			}

			public String getResults() 
			{
				return s;
			}
		};
		
		stupidTest.Run();
		System.out.println(stupidTest.getResults());
	}

}
