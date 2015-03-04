package tree;

import java.util.TreeMap;

public class IDSearcher <T extends IHasID>
{
	private TreeNode<T> mDataTree;
	private TreeMap<Integer, TreeNode<T>> mDataMap;
	
	public IDSearcher (TreeNode<T> tree)
	{
		mDataTree = tree;
		mDataMap = new TreeMap<Integer, TreeNode<T>>();
		buildSearchIndex();
	}
	
	private void buildSearchIndex() 
	{
		TreeIter<T> iter = (TreeIter<T>) mDataTree.iterator();
		
		//mDataMap.put(iter.current().data.getUID(), iter.current());
		while(iter.hasNext())
		{
			TreeNode<T> node = iter.next();
			
			mDataMap.put(node.data.getUID(), node);
			//mDataMap.put(new Integer(node.data.getUID()), (TreeNode<T>) node.data);
		}
	}

	public TreeNode<T> findById(Integer id)
	{
		return mDataMap.get(id);
	}
}
