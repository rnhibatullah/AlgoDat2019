import java.util.*;


public class FlowApplications {

    /**
     * cloneFlowNetwork() makes a deep copy of a FlowNetwork
     * (FlowNetwork has unfortunately no copy constructor)
     *
     * @param flowNetwork the flow network that should be cloned
     * @return cloned flow network (deep copy) with same order of edges
     */
    private static FlowNetwork cloneFlowNetwork(FlowNetwork flowNetwork) {
        int V = flowNetwork.V();
        FlowNetwork clone = new FlowNetwork(V);

//        Simple version (but reverses order of edges)
//        for (FlowEdge e : flowNetwork.edges()) {
//            FlowEdge eclone = new FlowEdge(e.from(), e.to(), e.capacity());
//            clone.addEdge(eclone);
//        }

        for (int v = 0; v < flowNetwork.V(); v++) {
            // reverse so that adjacency list is in same order as original
            Stack<FlowEdge> reverse = new Stack<>();
            for (FlowEdge e : flowNetwork.adj(v)) {
                if (e.to() != v) {
                    FlowEdge eclone = new FlowEdge(e.from(), e.to(), e.capacity());
                    reverse.push(eclone);
                }
            }
            while (!reverse.isEmpty()) {
                clone.addEdge(reverse.pop());
            }
        }
        return clone;
    }



    /**
     * numberOfEdgeDisjointPaths() returns the (maximum) number of edge-disjoint paths that exist in
     * an undirected graph between two nodes s and t using Edmonds-Karp.
     *
     * @param graph the graph that is to be investigated
     * @param s     node on one end of the path
     * @param t     node on the other end of the path
     * @return number of edge-disjoint paths in graph between s and t
     */

    public static int numberOfEdgeDisjointPaths(Graph graph, int s, int t) {
        // TODO

        FlowNetwork newFlowGraph = new FlowNetwork(graph.V());

        for (int i = 0; i <graph.V() ; i++) {


            if(i==t)
                continue;

                for (int edge: graph.adj(i)) {
                    FlowEdge inEdge = new FlowEdge(i,edge,1);
                    newFlowGraph.addEdge(inEdge);
                }


        }


        FordFulkerson newFordFullkerson = new FordFulkerson(newFlowGraph, s, t);

        int maxPaths = (int) newFordFullkerson.value();

        return maxPaths;
    }

    /**
     * edgeDisjointPaths() returns a maximal set of edge-disjoint paths that exist in
     * an undirected graph between two nodes s and t using Edmonds-Karp.
     *
     * @param graph the graph that is to be investigated
     * @param s     node on one end of the path
     * @param t     node on the other end of the path
     * @return a {@code Bag} of edge-disjoint paths in graph between s and t
     * Each path is stored in a {@code LinkedList<Integer>}.
     */

    public static Bag<LinkedList<Integer>> edgeDisjointPaths(Graph graph, int s, int t) {
        // TODO
        Bag<LinkedList<Integer>> bagEdgeDisjointPaths = new Bag<LinkedList<Integer>>();
        int value=0;


        FlowNetwork newFlowGraph = new FlowNetwork(graph.V());
        for (int i = 0; i <graph.V() ; i++) {
            if (i == t)
                continue;

            for (int edge : graph.adj(i)) {
                FlowEdge inEdge = new FlowEdge(i, edge, 1);
                newFlowGraph.addEdge(inEdge);
            }

        }

        /*




        LinkedList paths = new LinkedList();



        bagEdgeDisjointPaths.add(paths);


         */
        for (int i = numberOfEdgeDisjointPaths(graph, s, t); i >0 ; i--) {
            FlowEdge  edgeTo[] = new FlowEdge[newFlowGraph.V()];
            boolean  marked[] = new boolean[newFlowGraph.V()];

            // breadth-first search
            LinkedList<Integer> queue = new LinkedList<Integer>();
            queue.addFirst(s);
            marked[s] = true;
            while (!queue.isEmpty() && !marked[t]) {
                int v = queue.remove();

                for (FlowEdge e : newFlowGraph.adj(v)) {
                    int w = e.other(v);

                    // if residual capacity from v to w
                    if (e.residualCapacityTo(w) > 0) {
                        if (!marked[w]) {
                            edgeTo[w] = e;
                            marked[w] = true;
                            queue.addFirst(w);
                        }
                    }
                }
            }
            // augment flow
            LinkedList <Integer> path = new LinkedList<>();
            for (int v = t; v != s; v = edgeTo[v].other(v)) {

                edgeTo[v].addResidualFlowTo(v, 1);
                path.addFirst(v);
            }
            path.addFirst(s);
            bagEdgeDisjointPaths.add(path);
        }



        return bagEdgeDisjointPaths;

    }


    /**
     * isUnique determines for a given Flow Network that has a guaranteed minCut,
     * if that one is unique, meaning it's the only minCut in that network
     *
     * @param flowNetworkIn the graph that is to be investigated
     * @param s             source node s
     * @param t             sink node t
     * @return true if the minCut is unique, otherwise false
     */

    public static boolean isUnique(FlowNetwork flowNetworkIn, int s, int t) {
        // TODO
        boolean start[] = new boolean[flowNetworkIn.V()];
        boolean end[] = new boolean[flowNetworkIn.V()];

        int counter =0;
        FordFulkerson newFordfulkerson = new FordFulkerson(flowNetworkIn,s,t);

        for (int i = 0; i <flowNetworkIn.V() ; i++) {


            if(i==t)
                continue;

            for (FlowEdge edge: flowNetworkIn.adj(i)) {
                System.out.println(edge.toString());

            }


        }



        for (int i = 0; i <flowNetworkIn.V() ; i++) {
            start[i] = newFordfulkerson.inCut(i);
        }

        LinkedList queue = new LinkedList();
        for (FlowEdge r: flowNetworkIn.adj(t)) {
            queue.add(r.to());
        }


        for (FlowEdge d: flowNetworkIn.adj(t)) {
            counter++;
        }

        LinkedList<FlowEdge> marked_edge = new LinkedList<>();
        //System.out.println(queue);
        for(int i =0;i <counter; i++){
            int v = t;
            for (FlowEdge e: flowNetworkIn.adj(v)) {
                System.out.println("e from : "+e.from());
                System.out.println(marked_edge);
                if(!marked_edge.contains(e)){
                    marked_edge.add(e);
                    System.out.println("rest : " + (e.capacity()-e.flow()));
                    if (e.capacity()-e.flow() >0){

                        end[e.from()] = true;
                        System.out.println("v : "+v);
                        v= e.from();
                        System.out.println("e.from : "+v);
                        System.out.println("e.other : "+e.other(v));
                        System.out.println("----------------------");


                    }else {
                        System.out.println("else");
                        System.out.println("----------------------");
                    }

                }
                else{
                    continue;
                }

            }
        }

        for (int i = 0; i <flowNetworkIn.V() ; i++) {
            System.out.println(start[i]);
            System.out.println("-----------"+end[i]);
        }

        for (int i = 0; i <flowNetworkIn.V() ; i++) {
            if (start[i]!=end[i]){
                return false;
            }

        }
/*

        FlowNetwork reverse = new FlowNetwork(flowNetworkIn.V());

        for (int v=0; v<flowNetworkIn.V(); v++) {
            if (v==t) continue;
            for (FlowEdge edge: flowNetworkIn.adj(v)) {

                FlowEdge addE = new FlowEdge(edge.to(), edge.from(), edge.capacity(), edge.flow());
                System.out.println("from : "+addE.from());
                System.out.println("to : "+addE.to());
                System.out.println("flow:"+addE.flow());
                System.out.println("cap:"+addE.capacity());
                System.out.println("--------------------");
                reverse.addEdge(addE);
            }
        }

        for (int i = 0; i <start.length ; i++) {
            start[i] = newFordfulkerson.inCut(i);
            System.out.println(start[i]);
        }

        FordFulkerson reverseFord = new FordFulkerson(reverse,t,s);

        for (int i = 0; i <end.length ; i++) {
            end[i]= reverseFord.inCut(i);
            System.out.println("---------------"+end[i]);
        }

        for (int i = 0; i <flowNetworkIn.V() ; i++) {
            boolean isuni = end[i]!=start[i];
            if (isuni) return false;
        }

*/

        return true;
        //return true;
    }


    /**
     * findBottlenecks finds all bottleneck nodes in the given flow network
     * and returns the indices in a Linked List
     *
     * @param flowNetwork the graph that is to be investigated
     * @param s           index of the source node of the flow
     * @param t           index of the target node of the flow
     * @return {@code LinkedList<Integer>} containing all bottleneck vertices
     * @throws IllegalArgumentException is flowNetwork does not have a unique cut
     */

    public static LinkedList<Integer> findBottlenecks(FlowNetwork flowNetwork, int s, int t) {
        // TODO
        if (isUnique(flowNetwork,s,t)){
            throw new IllegalArgumentException("Flownetwork in bottle neck falsch");
        }
        LinkedList findBottle = new LinkedList();
        FlowNetwork cloneFlow= cloneFlowNetwork(flowNetwork);

        /*
        FordFulkerson newFordFullkerson = new FordFulkerson(flowNetwork,s,t);

        LinkedList allPossibleEdges = new LinkedList();

        //search all possible dge in flow network
        for (FlowEdge edge: flowNetwork.edges()) {
            allPossibleEdges.add(edge);

        }


        //search the adj from source
        for (FlowEdge e: flowNetwork.adj(s)) {
            if (newFordFullkerson.inCut(e.to())){
                LinkedList<Integer> path = new LinkedList();
                while (e.to() !=t && e.flow()==1){
                    for (FlowEdge e1: flowNetwork.adj(e.to())) {
                        path.add(e.from());
                        e = e1;
                        allPossibleEdges.remove(e);
                    }

                    //paths.add(path);
                }
            }else{
                continue;
            }


        }
    */
        int[]  out = new int[flowNetwork.V()];
        FordFulkerson newFordFullkerson = new FordFulkerson(cloneFlow,s,t);
        for (int i = 0; i <flowNetwork.V() ; i++) {
            if (newFordFullkerson.inCut(i)){
                for(FlowEdge edge: flowNetwork.adj(i) ){
                    if (findBottle.contains(edge.to())){
                        continue;
                    }
                    findBottle.add(edge);
                }

            }
        }

        return findBottle;
    }

    public static void main(String[] args) {
/*
        // Test for Task 2.1 and 2.2 (useful for debugging!)
        Graph graph = new Graph(new In("src/Graph1.txt"));
        int s = 0;
        int t = graph.V() - 1;
        int n = numberOfEdgeDisjointPaths(graph, s, t);
        //System.out.println(n);
        System.out.println("#numberOfEdgeDisjointPaths: " + n);
        Bag<LinkedList<Integer>> paths = edgeDisjointPaths(graph, s, t);
        for (LinkedList<Integer> path : paths) {
            System.out.println(path);
        }
    }


*/

        // Example for Task 3.1 and 3.2 (useful for debugging!)
        FlowNetwork flowNetwork = new FlowNetwork(new In("Flussgraph2.txt"));
        int s = 0;
        int t = flowNetwork.V() - 1;
        boolean unique = isUnique(flowNetwork, s, t);
        System.out.println("Is mincut unique? " + unique);
        // Flussgraph1 is non-unique, so findBottlenecks() should be tested with Flussgraph2
        /*
        flowNetwork = new FlowNetwork(new In("Flussgraph2.txt"));
        boolean unique1 = isUnique(flowNetwork, s, t);
        System.out.println("Is mincut unique1? " + unique1);
        LinkedList<Integer> bottlenecks = findBottlenecks(flowNetwork, s, t);
        System.out.println("Bottlenecks: " + bottlenecks);
*/
    }}




