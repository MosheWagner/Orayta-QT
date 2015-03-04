package bookTree;

import java.util.TreeMap;

import book.ABook;
import book.BookID;
import tree.TreeNode;

/*
 * Simply a tree of Book classes, with an added option to find a node by BookID
 */

public class BookTree extends TreeNode<ABook>
{
	TreeMap<BookID, TreeNode<ABook>> idMap = new TreeMap<BookID, TreeNode<ABook>>();
	
	public BookTree(ABook data) 
	{
		super(data);
	}

	
	@Override
	public TreeNode<ABook> addChild(ABook child) 
	{
		TreeNode<ABook> childNode = super.addChild(child);
		addToSearchMap(childNode);
		return childNode;
	}
	
	private void addToSearchMap(TreeNode<ABook> node) 
	{
		if (node.data == null) return;
		
		idMap.put(node.data.getID(), node);
		if (parent != null)
		{
			((BookTree) parent).addToSearchMap(node);
		}
	}


	public TreeNode<ABook> findBook(BookID bookid)
	{
		return idMap.get(bookid);
	}
}
