package bookTree;

import java.util.TreeMap;

import book.Book;
import book.BookID;
import tree.TreeNode;

public class BookTree extends TreeNode<Book>
{
	TreeMap<BookID, TreeNode<Book>> idMap = new TreeMap<BookID, TreeNode<Book>>();
	
	public BookTree(Book data) 
	{
		super(data);
	}

	
	@Override
	public TreeNode<Book> addChild(Book child) 
	{
		TreeNode<Book> childNode = super.addChild(child);
		addToSearchMap(childNode);
		return childNode;
	}
	
	private void addToSearchMap(TreeNode<Book> node) 
	{
		if (node.data == null) return;
		
		idMap.put(node.data.getUID(), node);
		if (parent != null)
		{
			((BookTree) parent).addToSearchMap(node);
		}
	}


	public TreeNode<Book> findBook(BookID bookid)
	{
		return idMap.get(bookid);
	}
}
