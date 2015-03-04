package tester;

import book.ABook;
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
				
				//if (t!= null) s = t.toString();
				TreeIter<ABook> it = (TreeIter<ABook>) t.iterator();
				for (int i=0; i<55;i++)it.next();
				
				s += t.findTreeNode(it.current()).data.getUID();
				
				IDSearcher<ABook> se = new IDSearcher<ABook>(t);
				s += se.findById(1590);
				//s = t.findTreeNodeByData(it.current().data).toString();
				//s = t.findTreeNodeByData("אורות");
				//s = t.toString();
				/*
				//Tree test:
                TreeNode <String> tree = new TreeNode <String>("A");
                tree.addChild("AA").addChild("AAA");
                TreeNode <String> sub = tree.addChild("AB");
                sub.addChild("ABA");
                sub.addChild("ABB");
                TreeNode <String> sub2 = tree.addChild("AC");
                TreeNode <String> sub3 = sub2.addChild("ACA").addChild("ACAA");
                TreeNode <String> sub4 = sub2.addChild("ACB");
                sub4.addChild("ACBA");
                TreeNode <String> sub5 = sub4.addChild("ACBB");
                
                //tree.removeElement(sub3);
                
                s = tree.toString();
                
                //Walk back demo:
                TreeIter<String> it = (TreeIter<String>) sub5.iterator();
                s += "\n\n" + it.current().toString();
                
                while(it.hasPrevius())
                {
                	s += it.previous().data.toString() + "\n";
                }
                */
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
