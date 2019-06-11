import java.util.*;

public class RandomDepthFirstPaths {
  	private boolean[] marked;    // marked[v] = is there an s-v path?
    private int[] edgeTo;        // edgeTo[v] = last edge on s-v path
    private final int s;         // source vertex

    public RandomDepthFirstPaths(Graph G, int s) {
        this.s = s;
        edgeTo = new int[G.V()];
        marked = new boolean[G.V()];
        validateVertex(s);
    }
	    
	public void randomDFS(Graph G) {
		randomDFS(G,s);
	}
	    
    // depth first search from v
    private void randomDFS(Graph G, int v) {
		// TODO
        marked[v] = true;
        Collections.shuffle(G.adj(v));
        for (int w : G.adj(v)) {
            if (!marked[w]) {
                randomDFS(G, w);
            }else{

                edgeTo[w]=v;
            }

        }

    }
    
    public void randomNonrecursiveDFS(Graph G) {
		// TODO
        marked = new boolean[G.V()];
        // to be able to iterate over each adjacency list, keeping track of which
        // vertex in each adjacency list needs to be explored next
        Iterator<Integer>[] adj = (Iterator<Integer>[]) new Iterator[G.V()];


        for (int v = 0; v < G.V(); v++){
            List<Integer> shuffle  = G.adj(v);
            Collections.shuffle(shuffle);
            adj[v] = shuffle.iterator();
        }

        // depth-first search using an explicit stack
        Stack<Integer> stack = new Stack<Integer>();
        marked[s] = true;
        stack.push(s);
        while (!stack.isEmpty()) {
            int v = stack.peek();
            if (adj[v].hasNext()) {
                int w = adj[v].next();
                if (!marked[w]) {

                    // discovered vertex w for the first time
                    marked[w] = true;
                    stack.push(w);
                }
            }
            else {
                stack.pop();
            }

        }
    }

    /**
     * Is there a path between the source vertex {@code s} and vertex {@code v}?
     * @param v the vertex
     * @return {@code true} if there is a path, {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    public boolean hasPathTo(int v) {
        validateVertex(v);
        return marked[v];
    }

    /**
     * Returns a path between the source vertex {@code s} and vertex {@code v}, or
     * {@code null} if no such path.
     * @param  v the vertex
     * @return the sequence of vertices on a path between the source vertex
     *         {@code s} and vertex {@code v}, as an Iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     * 
     * This method is different compared to the original one.
     */
    public List<Integer> pathTo(int v) {
		// TODO: Kopieren Sie diese Methode aus DepthFirstPaths Klasse
        if (!hasPathTo(v))
            return null;
        if (v<0)
            throw new IllegalArgumentException("invalid vertex number");

        Stack<Integer> path = new Stack<>();
        for (int w = v; w != s; w = edgeTo[w])
            path.push(w);
        path.push(s);
        return path;
    }
    
    public int [] edge() {
    	return edgeTo;
    }  

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    private void validateVertex(int v) {
        int V = marked.length;
        if (v < 0 || v >= V)
            throw new IllegalArgumentException("vertex " + v + " is not between 0 and " + (V-1));
    }

}

