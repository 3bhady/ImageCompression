// C++ implementation of Hopcroft Karp algorithm for
// maximum matching and getting minimum vertex cover

#include<iostream>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;

#define MAX 10001
#define NIL 0
#define INF (1<<28)

// A class to represent Bipartite graph for Hopcroft
// Karp implementation
class BipGraph
{
	// G = NIL[0] ∪ G1[G[1---n]] ∪ G2[G[n+1---n+m]] (contains graph edges)
	vector<int> G[MAX];

	// n: number of nodes on left side of Bipartite Graph, nodes are numbered 1 to n
	// m: number of nodes on right side of Bipartite Graph, nodes are numbered n+1 to n+m
	int n;
	int m;

	// match => matching set (each vertex is matched to which vertex)
	int match[MAX];

	// dist stores distance of left side vertices
	int dist[MAX];

	//minimum vertex cover rows and columns
	vector<int> minRows;
	vector<int> minCols;

	//visited array of graph nodes
	bool* visited;

public:

	// Constructor
	BipGraph(int rows, int cols); 

	// Destructor
	BipGraph::~BipGraph();

	void addEdge(int u, int v);

	// Returns true if there is an augmenting path
	bool bfs();

	// Adds augmenting path if there is one beginning with u
	bool dfs(int u);

	// Returns size of maximum matcing & minimum vertex cover cols amd rows
	int hopcroftKarp(vector<int>& Rows, vector<int>& Cols);

	//Gets minimum vertex cover
	void  getMinVertexCover();

	//Gets vertex cover rows and cols
	void vertexCoverDFS( int index,  vector<int>& Rows, vector<int>& Cols);

	//Prints the vertex cover cols and rows
	void BipGraph::printMinVertexCover();
};

// Constructor
BipGraph::BipGraph(int rows, int cols)
{
	this->m = rows;
	this->n = cols;

	//initialize visited array used for vertexCoverDFS
	visited = new bool[n + m + 1];
	for (int i = 0; i <= n + m; i++)
	{
		visited[i] = false;
	}
}

//Destructor
BipGraph::~BipGraph()
{
	delete visited;
}

// Returns size of maximum matching
int BipGraph::hopcroftKarp(vector<int>& Rows,vector<int>& Cols)
{

	int matching = 0;

	// match[] is assumed NIL for all vertex in G
	// Keep updating the result while there is an augmenting path.
	while (bfs()) {

		// Find a free vertex
		for (int i = 1; i <= n; i++) {

			// If current vertex is free and there is
			// an augmenting path from current vertex
			if (match[i] == NIL && dfs(i))
				matching++;
		}
	}

	getMinVertexCover();

	Rows = minRows;
	Cols = minCols;

	//printMinVertexCover();

	return matching;
}
//int BipGraph::hopcroftKarp()
//{
//	// pairU[u] stores pair of u in matching where u
//	// is a vertex on left side of Bipartite Graph.
//	// If u doesn't have any pair, then pairU[u] is NIL
//	pairU = new int[m + 1];
//
//	// pairV[v] stores pair of v in matching. If v
//	// doesn't have any pair, then pairU[v] is NIL
//	pairV = new int[n + 1];
//
//	// dist[u] stores distance of left side vertices
//	// dist[u] is one more than dist[u'] if u is next
//	// to u'in augmenting path
//	dist = new int[m + 1];
//
//	//initialize visited array used for vertexCoverDFS
//	visited = new bool*[max(n,m) + 1];
//	for (int i = 0; i <= max(n, m); i++)
//	{
//		visited[i] = new bool[2];
//		for (int j = 0; j < 2; j++)
//		{
//			visited[i][j] = false;
//		}
//	}
//
//	// Initialize NIL as pair of all vertices
//	for (int u = 0; u <= m; u++)
//		pairU[u] = NIL;
//	for (int v = 0; v <= n; v++)
//		pairV[v] = NIL;
//
//	// Initialize result
//	int result = 0;
//
//	// Keep updating the result while there is an
//	// augmenting path.
//	while (bfs())
//	{
//		// Find a free vertex
//		for (int u = 1; u <= m; u++)
//
//			// If current vertex is free and there is
//			// an augmenting path from current vertex
//			if (pairU[u] == NIL && dfs(u))
//				result++;
//	}
//	return result;
//}

// Returns true if there is an augmenting path, else returns false
bool BipGraph::bfs()
{
	int i, u, v, len;
	queue< int > Q;

	// First layer of vertices (set distance as 0)
	for (i = 1; i <= n; i++) {

		// If this is a free vertex, add it to queue
		if (match[i] == NIL) {
			dist[i] = 0;
			Q.push(i);
		}

		// Else set distance as infinite so that this vertex
		// is considered next time
		else dist[i] = INF;
	}

	// Initialize distance to NIL as infinite
	dist[NIL] = INF;

	while (!Q.empty()) {
		u = Q.front(); 
		Q.pop();

		// If this node is not NIL and can provide a shorter path to NIL
		if (u != NIL) {

			// Get all adjacent vertices of the vertex u
			for (i = 0; i < G[u].size(); i++) {
				v = G[u][i];

				// If v is not considered so far
				if (dist[match[v]] == INF) {
					dist[match[v]] = dist[u] + 1;
					Q.push(match[v]);
				}
			}
		}
	}

	// If we could come back to NIL using alternating path of distinct
	// vertices then there is an augmenting path
	return (dist[NIL] != INF);
}

//bool BipGraph::bfs()
//{
//	queue<int> Q; //an integer queue
//
//	// First layer of vertices (set distance as 0)
//	for (int u = 1; u <= m; u++)
//	{
//		// If this is a free vertex, add it to queue
//		if (pairU[u] == NIL)
//		{
//			// u is not matched
//			dist[u] = 0;
//			Q.push(u);
//		}
//
//		// Else set distance as infinite so that this vertex
//		// is considered next time
//		else dist[u] = INF;
//	}
//
//	// Initialize distance to NIL as infinite
//	dist[NIL] = INF;
//
//	// Q is going to contain vertices of left side only. 
//	while (!Q.empty())
//	{
//		// Dequeue a vertex
//		int u = Q.front();
//		Q.pop();
//
//		// If this node is not NIL and can provide a shorter path to NIL
//		if (dist[u] < dist[NIL])
//		{
//			// Get all adjacent vertices of the dequeued vertex u
//			list<int>::iterator i;
//			for (i = adj[u].begin(); i != adj[u].end(); ++i)
//			{
//				int v = *i;
//
//				// If pair of v is not considered so far
//				// (v, pairV[V]) is not yet explored edge.
//				if (dist[pairV[v]] == INF)
//				{
//					// Consider the pair and add it to queue
//					dist[pairV[v]] = dist[u] + 1;
//					Q.push(pairV[v]);
//				}
//			}
//		}
//	}
//
//	// If we could come back to NIL using alternating path of distinct
//	// vertices then there is an augmenting path
//	return (dist[NIL] != INF);
//}

// Returns true if there is an augmenting path beginning with free vertex u
bool BipGraph::dfs(int u)
{
	int i, v, len;
	if (u != NIL) {

		//loop on vertices adjacent to u
		for (i = 0; i < G[u].size(); i++) {

			v = G[u][i];

			if (dist[match[v]] == dist[u] + 1) {

				if (dfs(match[v])) {
					match[v] = u;
					match[u] = v;
					return true;
				}
			}
		}

		// If there is no augmenting path beginning with u.
		dist[u] = INF;
		return false;
	}
	return true;
}

//bool BipGraph::dfs(int u)
//{
//	if (u != NIL)
//	{
//		list<int>::iterator i;
//		for (i = adj[u].begin(); i != adj[u].end(); ++i)
//		{
//			// Adjacent to u
//			int v = *i;
//
//			// Follow the distances set by BFS
//			if (dist[pairV[v]] == dist[u] + 1)
//			{
//				// If dfs for pair of v also returns
//				// true
//				if (dfs(pairV[v]) == true)
//				{
//					pairV[v] = u;
//					pairU[u] = v;
//					return true;
//				}
//			}
//		}
//
//		// If there is no augmenting path beginning with u.
//		dist[u] = INF;
//		return false;
//	}
//	return true;
//}

// To add edge from u to v and v to u
void BipGraph::addEdge(int u, int v)
{
	G[u].push_back(v + n);	// Add u to v’s list.
	//G[v + n].push_back(u);	// Add v to u’s list. 
}

//Get minimium vertex cover rows and columns
void BipGraph::getMinVertexCover()
{
	vector<int> Rows;
	vector<int> Cols;

	for (int v = 1; v <= n; v++)
	{
		if (match[v] == NIL)	//non-matched vertex
		{
			vertexCoverDFS(v, Rows, Cols);
		}
	}

	for (int i = 1; i <= m; i++)
	{
		if (find(Rows.begin(), Rows.end(), i) == Rows.end())
		{
			minRows.push_back(i);
		}
	}

	for (int i = 1; i <= n; i++)
	{
		if (find(Cols.begin(), Cols.end(), i) != Cols.end())
		{
			minCols.push_back(i);
		}
	}
}

//void BipGraph:: getMinVertexCover()
//{
//	vector<int> Rows;
//	vector<int> Cols;
//	for (int v = 1; v <= n; v++)
//	{
//		if (pairV[v] != 0)	//non-matched vertex
//		{
//			vertexCoverDFS(0, 0, v, Rows, Cols);
//		}
//	}
//	for (int i = 1; i <= m; i++)
//	{
//		if (find(Rows.begin(), Rows.end(), i) == Rows.end())
//		{
//			minRows.push_back(i);
//		}
//	}
//	for (int i = 1; i <= n; i++)
//	{
//		if (find(Cols.begin(), Cols.end(), i) != Cols.end())
//		{
//			minCols.push_back(i);
//		}
//	}
//}


//DFS for alternating path
//Matched => 1 if looking for edge in matching set, else 0
//Index of vertex in graph
// Rows and columns marked by the DFS
void BipGraph::vertexCoverDFS(int index, vector<int>& Rows, vector<int>& Cols)
{
	if (visited[index])
		return;

	visited[index] = true;

	if (index <= n)
		Rows.push_back(index);
	else Cols.push_back(index - n);

	for (int i = 0; i < G[index].size(); ++i)
	{

		//on left side go on unmatched edge - on right side go on matched edge
		if ((index<=n && match[G[index][i]] != index) || (index>n && match[G[index][i]] == index)) {
			vertexCoverDFS(G[index][i], Rows, Cols);
		}
	}
}

//void BipGraph::vertexCoverDFS(bool GraphPart, bool Matched, int index, vector<int>& Rows, vector<int>& Cols)
//{
//	if (visited[index][GraphPart])
//		return;
//
//	visited[index][GraphPart] = true;
//	int * v;
//	if (GraphPart == 0)
//	{
//		v = pairV;
//		Rows.push_back(index);
//	}
//	else {
//		v = pairU;
//		Cols.push_back(index);
//	}
//
//	for (auto i = adj[index].begin(); i != adj[index].end(); ++i)
//	{
//		if (!Matched || v[*i] != NIL) {
//			vertexCoverDFS(!GraphPart, !Matched, *i, Rows, Cols);
//		}
//	}
//}



//Prints the minimum vertex cover rows and columns
void BipGraph::printMinVertexCover()
{
	cout << "Minimum vertex cover :" << endl;
	cout << "Rows: ";
	for (int i = 0; i < minRows.size(); i++)
	{
		cout << minRows[i] << " ";
	}

	cout << endl << "Columns: ";
	for (int i = 0; i < minCols.size(); i++)
	{
		cout << minCols[i] << " ";
	}
	cout << endl;
}



