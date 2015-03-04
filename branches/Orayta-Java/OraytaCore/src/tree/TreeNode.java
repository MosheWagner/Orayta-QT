package tree;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

// http://stackoverflow.com/questions/3522454/java-tree-data-structure
// http://sujitpal.blogspot.co.il/2006/05/java-data-structure-generic-tree.html

public class TreeNode<T> implements ITree<T>, Iterable<TreeNode<T>> {

    public T data;
    public TreeNode<T> parent;
    public List<TreeNode<T>> children;

    private ArrayList<TreeNode<T>> elementsIndex;
    
    public boolean isRoot() {
        return parent == null;
    }

    public boolean isLeaf() {
        return children.size() == 0;
    }

    public TreeNode(T data) {
        this.data = data;
        this.children = new LinkedList<TreeNode<T>>();
        
        this.elementsIndex = new ArrayList<TreeNode<T>>();
        this.elementsIndex.add(this);
    }

    public TreeNode<T> addChild(T child) 
    {
        TreeNode<T> childNode = new TreeNode<T>(child);
        childNode.parent = this;
        this.children.add(childNode);
        this.registerChildForSearch(childNode);
        return childNode;
    }

    public int getLevel() 
    {
        if (this.isRoot())
            return 0;
        else
            return parent.getLevel() + 1;
    }

    private void registerChildForSearch(TreeNode<T> node) 
    {
        elementsIndex.add(node);
        
        if (parent != null)
            parent.registerChildForSearch(node);
    }
    
	private void unRegisterChildFromSearch(TreeNode<T> node) 
	{
		elementsIndex.remove(node);
		
        if (parent != null)
            parent.unRegisterChildFromSearch(node);
	}

    public TreeNode<T> findTreeNode(TreeNode<T> node) 
    {
    	return elementsIndex.get(elementsIndex.indexOf(node));
    }

    public ITreeIterator<TreeNode<T>> iterator() {
        //TreeNodeIter<T> iter = new TreeNodeIter<T>(this);
		TreeIter<T> iter =  new TreeIter<T>(this);
        return iter;
    }

	public List<TreeNode<T>> getChildren() {
		return children;
	}

	public int getIndexInBranch()
	{
		if (parent == null) return -1;
		return parent.children.indexOf(this);
	}
	
	/*
	 * Remove the given node from the tree. 
	 *  Obviously, it must be a child (to some level) of this branch, and not the root!
	 */
	public void removeElement(TreeNode<T> node) 
	{
		//Here I want '==' and not 'equals'
		if (node == this) return;
		
		TreeNode<T> parentNode = node.parent;
		parentNode.children.remove(node);
		parentNode.unRegisterChildFromSearch(node);
	}
	
    @SuppressWarnings("unchecked")
	@Override
	/**
	 * Returns a nice representation of the whole tree starting from this node.
	 * (With indentation and everything!)
	 */
	public String toString() 
    {
    	int level = this.getLevel();
		String s = "";
		
		ITreeIterator<T> iter = (ITreeIterator<T>) this.iterator();
		TreeNode<T> node = (TreeNode<T>) iter.current();
		
		do
		{
			String tabs = "";
			for (int i=0, n=node.getLevel(); i<n; i++) tabs += "\t";
			
			s += tabs + node.dataToString() + "\n";
			
			node = (TreeNode<T>) iter.next();
		}while(node != null && node.getLevel() > level);

    	return s;
	}
	
    //ToString of the nodes data, ignoring the fact that it's part of a tree
    public String dataToString() {
            return data != null ? data.toString() : "[data null]";
    }
}