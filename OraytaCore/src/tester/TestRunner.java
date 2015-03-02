package tester;

import java.io.IOException;

import javax.xml.bind.JAXBException;

import book.ChapterList;
import book.DChapter;
import book.ChapterID;
import book.IChapter;

import settings.GeneralSettings;
import tree.TreeNode;
import tree.TreeNodeIter;
import xml.ClassIO;

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
