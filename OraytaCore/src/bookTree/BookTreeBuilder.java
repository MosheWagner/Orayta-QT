package bookTree;

import java.io.File;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import tree.TreeNode;

import book.ABook;
import book.BookFolder;
import book.OBK_Book;

/*
 * Builds the bookTree from the file system
 */

public class BookTreeBuilder
{
	TreeNode<ABook> tree;

	public TreeNode<ABook> buildTree(String rootPath)
	{
		//Root is a folder called 'OraytaBooks'
		tree = new TreeNode<ABook>(new BookFolder(rootPath, "OraytaBooks"));
		
		List<File> files = Arrays.asList(new File(rootPath).listFiles());
		sortByLeadingNumber(files);
		addFilesToTreeNode((files), tree);

		return tree;
	}

	private final String OBK_SUFFIX = ".obk";
	private final String FOLDER_CONF_SUFFIX = ".folder";
	
	private void addFilesToTreeNode(List<File> files, TreeNode<ABook> treeNode)
	{
		for (File f:files)
		{
			ABook book;

			if (f.getName().endsWith(FOLDER_CONF_SUFFIX))
			{
				String folderPath = f.getAbsolutePath().replace(FOLDER_CONF_SUFFIX, "");
				
				book = new BookFolder(folderPath);
				TreeNode<ABook> branch = treeNode.addChild(book);
				
				List<File> children = Arrays.asList(new File(folderPath).listFiles());
				sortByLeadingNumber(children);
				addFilesToTreeNode(children, branch);
			}
			else if (f.getName().endsWith(OBK_SUFFIX))
			{
				book = new OBK_Book(f.getAbsolutePath());
				treeNode.addChild(book);
			}
		}
	}
	
	private void sortByLeadingNumber(List<File> files)
	{
		Collections.sort(files, new Comparator<File>()
		{
		     public int compare(File f1, File f2)
		     {
		    	 String s1 = f1.getName().split("_")[0];
		    	 String s2 = f2.getName().split("_")[0];
		    	 
		    	 return s1.compareTo(s2);
		     }
		});
	}
}
