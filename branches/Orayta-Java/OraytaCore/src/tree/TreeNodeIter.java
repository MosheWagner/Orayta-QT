package tree;

import java.util.Iterator;

public class TreeNodeIter<T> implements Iterator<TreeNode<T>> {

	//TODO: Create setNext(), and make next() work without hasNext(), which should be a read only function.
	
    enum ProcessStages {
            ProcessParent, ProcessChildCurNode, ProcessChildSubNode
    }

    private TreeNode<T> treeNode;

    private ProcessStages doNext;
    private TreeNode<T> next;
    private Iterator<TreeNode<T>> childrenCurNodeIter;
    private Iterator<TreeNode<T>> childrenSubNodeIter;
    
    public TreeNodeIter(TreeNode<T> treeNode) {
            this.treeNode = treeNode;
            this.doNext = ProcessStages.ProcessParent;
            this.childrenCurNodeIter = treeNode.children.iterator();
    }

    public boolean hasNext() {

            if (this.doNext == ProcessStages.ProcessParent) {
                    this.next = this.treeNode;
                    this.doNext = ProcessStages.ProcessChildCurNode;
                    return true;
            }

            if (this.doNext == ProcessStages.ProcessChildCurNode) {
                    if (childrenCurNodeIter.hasNext()) {
                            TreeNode<T> childDirect = childrenCurNodeIter.next();
                            childrenSubNodeIter = childDirect.iterator();
                            this.doNext = ProcessStages.ProcessChildSubNode;
                            return hasNext();
                    }

                    else {
                            this.doNext = null;
                            return false;
                    }
            }
            
            if (this.doNext == ProcessStages.ProcessChildSubNode) {
                    if (childrenSubNodeIter.hasNext()) {
                            this.next = childrenSubNodeIter.next();
                            return true;
                    }
                    else {
                            this.next = null;
                            this.doNext = ProcessStages.ProcessChildCurNode;
                            return hasNext();
                    }
            }

            return false;
    }

    /*
     * NOTE: You must call hasNext() before you use this!
     */
    public TreeNode<T> next() {
            return this.next;
    }

    @Override
    public void remove() {
            this.treeNode.removeElement(this.next);
    }

}