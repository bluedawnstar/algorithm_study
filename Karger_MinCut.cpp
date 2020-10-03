// Karger's algorithm to find Minimum Cut in a graph
#include<bits/stdc++.h>
#define INF 1000000000
#define BILLION 1000000000L;
using namespace std;

//To execute clock_gettime
double total_time;
struct timespec start, stop;

//a structure to represent a edges 
struct Edge 
{ 
	int src_node;
	int dest_node; 
}; 

//a structure to represent a graph as a collection of edges. 
struct Graph 
{ 
	// no_of_vertices-> Number of vertices, no_of_edges-> Number of edges 
	int no_of_vertices, no_of_edges; 

	 /* Since the graph is undirected, the edge 
	 from src to dest is also edge from dest 
	 to src. Both are counted as 1 edge here*/ 
	   struct Edge* edge; 
}; 


//a subset structure for union-find method
struct subset 
{ 
	int parent; 
	int rank; 
}; 

// Functions for union-find
int find_set(struct subset subsets[], int node); 
void Union(struct subset subsets[], int u, int v); 

/*karger's algorithm function to find the min-cut and loop is 
  iterated for nC2log(n) to find minimum of all iteration and
  improve accuracy*/
int kargerMinCut(struct Graph* graph) 
{ 
	// defining no of edges and vertices 
	int V = graph->no_of_vertices, E = graph->no_of_edges; 
	struct Edge *edge = graph->edge; 

	// Allocate memory for creating V nodes subsets. 
	struct subset *subsets = new subset[V]; 

	// Create V subsets with single elements 
	for (int vertex = 0; vertex < V; vertex++) 
	{ 
		subsets[vertex].parent = vertex; 
		subsets[vertex].rank = 0; 
	} 

	// initially No of vertices in a contracting graph
	int vertices = V; 

	/*Keep contracting vertices until 2 vertices 
	  are left in the graph*/ 
	while (vertices > 2) 
	{ 
	// Pick a random edge 
	int random_edge = rand() % E; 

	// Find sets of two corners of current edge 
	int set1 = find_set(subsets, edge[random_edge].src_node); 
	int set2 = find_set(subsets, edge[random_edge].dest_node); 

	// If two corners belong to same subset
	if (set1 == set2) 
		    continue; 

	// Else contract the edge
	else
	{ 
		//printf("Contracting edge %d-%d\n",edge[random_edge].src_node, edge[random_edge].dest_node); 
		vertices--; 
		Union(subsets, set1, set2); 
	} 
	} 

	// Now we have two vertices left in 
	// the contracted graph 
	int curr_cut = 0; 
	for (int i=0; i<E; i++) 
	{ 
		int set1 = find_set(subsets, edge[i].src_node); 
		int set2 = find_set(subsets, edge[i].dest_node); 
		if (set1 != set2) 
		curr_cut++; 
	} 

	return curr_cut; 
} 

// A utility function to find set of an element i 
// (uses path compression technique) 
int find_set(struct subset subsets[], int node) 
{ 
	// find root and make root as parent of i 
	// (path compression) 
	if (subsets[node].parent != node) 
	    subsets[node].parent = find_set(subsets, subsets[node].parent); 

	return subsets[node].parent; 
} 

//Union of two sets of node u and node v(Union by rank method) 
void Union(struct subset subsets[], int u, int v) 
{ 
	int xroot = find_set(subsets, u); 
	int yroot = find_set(subsets, v); 

	// smaller rank tree under root of high rank tree 
	if (subsets[xroot].rank < subsets[yroot].rank) 
		subsets[xroot].parent = yroot; 
	else if (subsets[xroot].rank > subsets[yroot].rank) 
		subsets[yroot].parent = xroot; 

	// If ranks are same, then make one as root 
	else
	{ 
		subsets[yroot].parent = xroot; 
		subsets[xroot].rank++; 
	} 
} 

// Creates a graph
struct Graph* createGraph(int V, int E) 
{ 
	Graph* graph = new Graph; 
	graph->no_of_vertices = V; 
	graph->no_of_edges = E; 
	graph->edge = new Edge[E]; 
	return graph; 
} 



int Runtime_analysis() 
{
    //No of nodes in graph
    int n;
    cin >> n; 
    int x,E=0,k=0;
    //graph_mat to store adj matrix
    int graph_mat[n][n];
  
    //Finding no of Edges in a Graph
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> x;
            graph_mat[i][j]=x;
            if(x!=0)    
                E++;	
        }
    }
	
	//Each edge counted twice b/c of undirected Graph			
	E=E/2;

    //createGraph with n-Vertices and E-egdes
    struct Graph* graph = createGraph(n, E);
   
    for (int u = 0; u < n; u++)
    {
        for (int v = u+1; v < n; v++)
        {
          
            if(graph_mat[u][v]!=0)
            {		
      			graph->edge[k].src_node = u; 
				graph->edge[k].dest_node = v; 
            	k++;	
	        }
        }
    }
  

  

  // calculating runtime for karger's multi iterations
  if (clock_gettime(CLOCK_REALTIME, &start) == -1) 
  {
    perror("clock gettime");
    return EXIT_FAILURE;
  }

    // calling kargerMinCut algo nc2 logn times
    int mini = INT_MAX;
    for(int iter = 0; iter < ((n * (n-1))/2)*(log2(n)) ; iter++)
    {
        mini = min(mini,kargerMinCut(graph));
    }
 

  
  

  if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
    perror("clock gettime");
    return EXIT_FAILURE;
  }

  
  total_time = (stop.tv_sec - start.tv_sec) +
          (double)(stop.tv_nsec - start.tv_nsec) / (double)BILLION;



  printf("%d %lf\n",n,total_time/ ((n * (n-1))/2)*(log2(n)));
  return 0;
}

// Driver program to test above functions 
int main() 
{ 
	
    

  	for(int i = 3; i <=50 ; i++)
  	{
    	Runtime_analysis();
     }
	
	return 0; 
} 
