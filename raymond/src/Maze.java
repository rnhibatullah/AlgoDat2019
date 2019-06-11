import java.util.LinkedList;
import java.util.List;

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

	
    /**
     * Adds the undirected edge v-w to the graph M.
     *
     * @param  v one vertex in the edge
     * @param  w the other vertex in the edge
     * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
     */
    public void addEdge(int v, int w) {
		// TODO
        if(v < 0|| v > M.V()|| w < 0||w >M.V()){
            throw new IllegalArgumentException("invalid v or w");
        }
        M.addEdge(v,w);
    }
    
    /**
     * Returns true if there is an edge between 'v' and 'w'
     * @param v
     * @param w
     * @return true or false
     */
    public boolean hasEdge( int v, int w){
		// TODO
        if(v == w){
            return true;
        }
        else{
            if(M.adj(v).contains(w) && M.adj(w).contains(v)){
                return true;
            }
        }
        return false;
    }	
    
    /**
     * Builds a grid as a graph.
     * @return Graph G -- Basic grid on which the Maze is built
     */
    public Graph mazegrid() {
		// TODO
        Graph G = new Graph(N*N);
        /*int[] chance = new int[N];
        int[] savex = new int[(this.N+1)*(this.N+1)];
        int[] savey = new int[(this.N+1)*(this.N+1)];
        boolean picked = false;
        for(int i = 0;i < N;i++){
            chance[i] = i;
        }
        for(int i = 0; i < N*N-1;i++){
            for(int x = 0;x < N;x++){
                for(int y = 0;y < N;y++){
                    if((y*N+x)==i){
                        savex[iterate] = x;
                        savey[iterate] = y;
                        iterate++;
                    }
                }
            }
        }
        int[] sum = new int[(this.N+1)*(this.N+1)];
        for(int i = 0;i < (this.N+1)*(this.N+1);i++){
            sum[i] = savex[i] + savey[i];
        }
        for()*/
        int count = 0;
        for(int ix = 0;ix < N*N;ix++){
            if(count >= N-1){
                count = 0;
            }
            else{
                G.addEdge(ix,ix+1);
                count++;
            }
        }
        int iy = 0;
        for(int cy = 0;cy < N;cy++){
            for(int c = 0; c < N-1;c++) {
                G.addEdge(iy, iy + N);
                iy = iy + N;
            }
            iy = cy + 1;
        }
        return G;
    }

    private void buildMaze() {
		// TODO
        Graph grid = mazegrid();
        RandomDepthFirstPaths rdfs = new RandomDepthFirstPaths(grid, startnode);
        rdfs.randomDFS(grid);
        for(int v:grid.adj(startnode)){
            M.adj(startnode).add(v);
        }
    }


    public List<Integer> findWay(int v, int w){
		// TODO
        RandomDepthFirstPaths path = new RandomDepthFirstPaths(M, v);
        List<Integer> pathWay = new LinkedList<>();
        pathWay = path.pathTo(w);
        return pathWay;
    }
    
    public Graph M() {
    	return M;
    }

    public static void main(String[] args) {
		// FOR TESTING
        Maze maze = new Maze(3,0);
        GridGraph vis= new GridGraph(maze.mazegrid());
    }


}

