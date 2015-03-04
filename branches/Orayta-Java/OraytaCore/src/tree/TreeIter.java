package tree;

public class TreeIter<T> implements ITreeIterator<TreeNode<T>> 
{
	TreeNode<T> currentNode;
	
	public TreeIter(TreeNode<T> treeNode)
	{
		currentNode = treeNode;
	}
	
	public boolean hasNext() {
		return (NextNode(currentNode) != null);
	}

	public TreeNode<T> next() 
	{
		TreeNode<T> next = NextNode(currentNode);
		this.currentNode = next;
		return this.currentNode;
	}

	private TreeNode<T> NextNode(TreeNode<T> currentNode) 
	{
		if (currentNode == null) return null;
		
		//Simplest case. If this node has children - return the first one.
		if (currentNode.getChildren().size() != 0) return currentNode.getChildren().get(0);
		
		//Try other form of siblings
		return NextNonChildNode(currentNode);
	}
	
	private TreeNode<T> NextNonChildNode(TreeNode<T> currentNode) 
	{
		TreeNode<T> parentNode = currentNode.parent;
		int currentIndex = currentNode.getIndexInBranch();

		//No children and no parent. poor dude.
		if (parentNode == null) return null;
		
		//If at least he has a younger brother...
		if (parentNode.getChildren().size() > (currentIndex + 1)) return parentNode.getChildren().get(currentIndex + 1);
		
		//No dice. Let's try recursion.
		return NextNonChildNode(parentNode);
	}
	
	public boolean hasPrevius() {
		return (previousNode(currentNode) != null);
	}
	
	public TreeNode<T> previous() 
	{
		TreeNode<T> prev = previousNode(currentNode);
		this.currentNode = prev;
		return currentNode;
	}

	private TreeNode<T> previousNode(TreeNode<T> node) 
	{
		TreeNode<T> parentNode = node.parent;
		
		//No parent means no siblings
		if (parentNode == null) return null;
		
		int currentIndex = node.getIndexInBranch();
		
		//Check for older brothers
		if (currentIndex > 0) return LastChild(parentNode.getChildren().get(currentIndex - 1));
		
		//Well, in this case, the previous element is the parent.
		return parentNode;
	}

	private TreeNode<T> LastChild(TreeNode<T> treeNode) 
	{
		if (treeNode == null) return null;
		if (treeNode.children.size() == 0) return treeNode;
		return LastChild(treeNode.children.get(treeNode.children.size() - 1));
	}

	public void remove() {
		this.currentNode.parent.removeElement(this.currentNode);
	}

	public TreeNode<T> current() 
	{
		return currentNode;
	}

	public boolean hasPrevious() {
		// TODO Auto-generated method stub
		return false;
	}

}
