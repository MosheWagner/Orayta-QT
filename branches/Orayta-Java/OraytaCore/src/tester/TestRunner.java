package tester;

import book.ABook;
import book.OBK_Book;
import bookReader.IBookBuilder;
import bookReader.OBK_Builder;
import bookTree.BookTreeBuilder;
import settings.GeneralSettings;
import tree.IDSearcher;
import tree.TreeIter;
import tree.TreeNode;

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
				
				BookTreeBuilder treebuild = new BookTreeBuilder();
				TreeNode<ABook> t = treebuild.buildTree(new GeneralSettings().BOOKS_ROOT_DIR);

				TreeIter<ABook> itr = (TreeIter<ABook>) t.iterator();
				itr.next(); itr.next(); itr.next(); itr.next();
				
				//בראשית
				TreeNode<ABook> b = itr.current();
				IBookBuilder<OBK_Book> builder = new OBK_Builder();
				builder.buildContents((OBK_Book) b.data);
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
