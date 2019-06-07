import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.NoSuchElementException;
import java.util.Stack;

public class Maze{
    private final int N;
    private Graph M;
    public int startnode;
        
	public Maze(int N, int startnode) {
		
        if (N < 0) throw new IllegalArgumentException("Number of vertices in a row must be nonnegative");
        this.N = N;
        this.M= new Graph(N*N);
        this.startnode= startnode;
        buildMaze();
	}
	
    public Maze (In in) {
    	this.M = new Graph(in);
    	this.N= (int) Math.sqrt(M.V());
    	this.startnode=0;
    }

    public static double squareRoot(int number) {
        double temp;

        double sr = number / 2;

        do {
            temp = sr;
            sr = (temp + (number / temp)) / 2;
        } while ((temp - sr) != 0);

        return sr;
    }

	
    /**
     * Adds the undirected edge v-w to the graph M.
     *
     * @param  v one vertex in the edge
     * @param  w the other vertex in the edge
     * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
     */
    public void addEdge(int v, int w) {
		// TODO
        M.adj(v).add(w);
        M.adj(w).add(v);

    }
    
    /**
     * Returns true if there is an edge between 'v' and 'w'
     * @param v
     * @param w
     * @return true or false
     */
    public boolean hasEdge( int v, int w){
		// TODO
        boolean edgeV;
        boolean edgeW;

        edgeV = M.adj(v).contains(w);

        edgeW = M.adj(w).contains(v);

        if (edgeV == edgeW){
            return true;
        }

        return  false;

    }	
    
    /**
     * Builds a grid as a graph.
     * @return Graph G -- Basic grid on which the Maze is built
     */
    public Graph mazegrid() {
		// TODO

        int v = this.M.V();

        Graph G = new Graph(N*N);

        int sr = (int) squareRoot(v);


        //connect horizontal nodes
        for (int j = 0 ; j <N ; j++) {

            for (int i = 1; i < N ; i++) {

                G.addEdge((i-1)+j*N, i+j*N);
            }

        }

        //connect vertical
        for (int j = 0 ; j <N ; j++) {

            for (int i = 1; i < N ; i++) {

                G.addEdge(i+j*N, (i-1)+j*N);
            }

        }

        return G;
    }
    
    private void buildMaze() {
		// TODO

        Graph G;
        G = this.mazegrid();

        RandomDepthFirstPaths DFS = new RandomDepthFirstPaths(G,startnode);

        DFS.randomDFS(G);

        for (int w: G.adj(startnode)) {
            M.adj(startnode).add(w);
        }




    }

    
    public List<Integer> findWay(int v, int w){
		// TODO
        LinkedList way = new LinkedList();

        Graph G = new Graph(M.V());
        RandomDepthFirstPaths DFS = new RandomDepthFirstPaths(G,v);
        Stack<Integer> path = new Stack<Integer>();

        DFS.pathTo(w);

        while (!path.isEmpty()){
            way.add(path.pop());
        }

        return way;

    }
    
    public Graph M() {
    	return M;
    }

    public static void main(String[] args) {
		// FOR TESTING
        Maze test = new Maze(20,0);
        //Graph G =  test.mazegrid();
        System.out.println(test.M.toString());
        test.findWay(2,8);
        GridGraph gridGraph = new GridGraph(test.M, test.findWay(0,20*20-1));
    }


}

