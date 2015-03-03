package tester;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

import tree.TreeNode;
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
				TreeNode <String> tree = new TreeNode <String>("Root");
				tree.addChild("wombat");
				TreeNode <String> sub = tree.addChild("wombat2");
				sub.addChild("grandchildWombat");
				TreeNode <String> sub2 = sub.addChild("grandchildWombat2");
				sub.addChild("grandchildWombat3");
				sub.addChild("grandchildWombat4");
				sub2.addChild("greatgrandchildWombat1");
				sub2.addChild("greatgrandchildWombat2");
				
				s = tree.toString();
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
