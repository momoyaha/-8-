#include<iostream>
#include <queue>
using namespace std;
const int MaxInt = 32767;//极大值
const int MVNum = 100;//最大顶点数
const int Max_Vertex_Num = 20;
bool visited[MVNum];
bool* S;
int* D;
int* Path;
int** rD;
int** rPath;
/*-----图的邻接矩阵存储表示-----*/
typedef struct AMGraph{
	char vexs[MVNum];//顶点表
	int arcs[MVNum][MVNum];//邻接矩阵
	int vexnum, arcnum;//图当前点数和边数
}AMGraph;
/*-----图的邻接表存储表示-----*/
typedef struct ArcNode{
	int adjvex;//该边指向的顶点的位置
	struct ArcNode* nextarc;//指向下一条边的指针
	int info;//和边相关的信息
}ArcNode;
typedef struct VNode{
	char data;
	ArcNode* firstarc;//指向第一条依附该顶点的边的指针
}VNode,AdjList[MVNum];
typedef struct ALGraph{
	AdjList vertices;
	int vexnum, arcnum;
}ALGraph;
int CreateUDN(AMGraph& G);//采用邻接矩阵表示法，创建无向网G
int LocateVex(AMGraph G, char a);//定位i在图G中的序号
void DFS_AM(AMGraph G, int v);//邻接矩阵图的深度遍历
void BFS_AM(AMGraph G, int v);//邻接矩阵图的广度遍历
//int FirstAdjVex(AMGraph G, int u);//依次检查u的所有邻接点w, FirstAdjVex(G,u)表示u的第一个邻接点
//int NextAdjVex(AMGraph G, int u,int w);//NextAdjVex(G,u,w)表示u相对于w的下一个邻接点，w>=0表示存在邻接点
void ShortestPath_DIJ(AMGraph G, int v0);//用Dijkstra算法求有向网G的vO顶点到其余顶点的最短路径
void ShortestPath_Floyd(AMGraph G);//用Floyd算法求有向网G中各对顶点1和）之间的最短路径
void print(AMGraph G);
int main()
{
	AMGraph G;
	CreateUDN(G);
	cout << "图的深度遍历为：" << endl;
	DFS_AM(G, 0);
	cout << endl<<"图的广度遍历为：" << endl;
	BFS_AM(G, 0);
}

int CreateUDN(AMGraph& G)//采用邻接矩阵表示法，创建无向网G
{
	cout << "请输入图的总顶点数，总边数：" << endl;
	cin >> G.vexnum >> G.arcnum;//输入总顶点，总边数
	int i, j, w;//w为weight
	char v1, v2;
	cout << "请输入顶点的信息：" << endl;
	for (int i = 0; i < G.vexnum; i++) {//依次输入点的信息
		cin >> G.vexs[i];
	}
	for (i = 0; i < G.vexnum; i++) {//初始化邻接矩阵，边的权值均置为极大值MaxInt
		for(int j = 0; j < G.vexnum; j++) {
			G.arcs[i][j] = MaxInt;
		}
	}
	cout << "请输入两点间的权值：" << endl;
	for (int k = 0; k < G.arcnum; k++) {
		cin >> v1 >> v2 >> w;
		i = LocateVex(G, v1);
		j = LocateVex(G, v2);
		G.arcs[i][j] = w;
		G.arcs[j][i] = G.arcs[i][j];
	}
	return 1;
}


void DFS_AM(AMGraph G, int v)//深度优先搜索遍历连通图
{
	cout << v;
	visited[v] = true;//访问第v个顶点，并置访问标志为true
	for (int w = 0; w < G.vexnum; w++) {//依次检查邻接矩阵v所在的行
		if ((G.arcs[v][w] != 0) && (!visited[w]))
			DFS_AM(G, w);
		//G.arcs[v][w]!=0表示w是v的邻接点，如果w未访问，则递归调用DFS
	}
}

void BFS_AM(AMGraph G, int v)//按广度优先非递归遍历连通图G
{
	cout<<v;
	visited[v] = true;//访问第v个顶点，并置访问标志数组相应分量值为true
	queue<int> q;//辅助队列q初始化， 置空
	q.push(v);//v进队
	while (!q.empty()) {//队列非空
		int u = q.front();//队头元素出队并置为u
		q.pop();
		for (int w = 0; w <= G.vexnum;w++) {
			//依次检查u的所有邻接点w, FirstAdjVex(G,u)表示u的第一个邻接点
			//NextAdjVex(G,u,w)表示u相对于w的下一个邻接点，w>=0表示存在邻接点
			if (!visited[w]) {//w为u的尚未访问的邻接顶点
				cout << w;
				visited[w] = true;//访问 w, 并置访问标志数组相应分扯值为true
				q.push(w);//w进队
			}
		}
	}
}

int LocateVex(AMGraph G, char a) //定位i在图G中的序号
{
	for (int i = 0; i < G.vexnum; i++) {
		if (G.vexs[i] == a)
			return i;
		continue;
	}
	return 0;
}

void ShortestPath_DIJ(AMGraph G, int v0)//用Dijkstra算法求有向网G的vO顶点到其余顶点的最短路径
{
	int n = G.vexnum;        //n为G中顶点的个数
	int v,w;
	S = new bool[n];
	D = new int[n];
	Path = new int[n];
	for (v = 0; v < n; v++) {//n个顶点依次初始化
		S[v] = false;        //S初始为空集
		D[v] = G.arcs[v0][v];//将vO到各个终点的最短路径长度初始化为弧上的权值
		if (D[v] < MaxInt)
			Path[v] = v0;    //如果vO 和v之间有弧， 则将v的前驱置为vO
		else
			Path[v] = -1;    //如果vO 和v之间无弧， 则将v的前驱置为一1
	}
	S[v0] = true;            //将vO加人S
	D[v0] = 0;               //源点到源点的距离为 0
	/*-----初始化结束，开始主循环，每次求得vO到某个顶点v的最短路径，将v加到s集----*/
	for (int i = 1; i < n; i++) {//对其余 n-1个顶点，依次进行计算
		int min = MaxInt;
		for (w = 0; w < n; w++) {
			if (!S[w] && D[w] < min) {
				v = w; min = D[w];//选择一条当前的最短路径，终点为v
			}
			S[v] = true;          //将v加入S
			for (w = 0; w < n; w++) {//更新从v0出发到集合v-s上所有顶点的最短路径长度
				if (!S[w] && D[v] + G.arcs[v][w] < D[w]) {
					D[w] = D[v] + G.arcs[v][w];//更新 D[w]
					Path[w] = v;               //更改w的前驱为v
				}
			}
		}
	}
}

void ShortestPath_Floyd(AMGraph G)//用Floyd算法求有向网G中各对顶点1和）之间的最短路径
{
	int i, j, k;
	rD = new int* [G.vexnum];
	rPath = new int* [G.vexnum];
	for (i = 0; i < G.vexnum; i++) {//各对结点之间初始已知路径及距离
		for (j = 0; j < G.vexnum; j++) {
			rD[i][j] = G.arcs[i][j];
			if (rD[i][j] < MaxInt)
				rPath[i][j] = i;//如果i和j之间有弧，则将j的前驱置为i
			else
				rPath[i][j] = -1;//如果i和j之间无弧，则将j的前驱置为-1
		}
		for (k = 0; k < G.vexnum; k++)
			for (i = 0; i < G.vexnum; i++)
				for (j = 0; j < G.vexnum; j++)
					if (rD[i][k] + rD[k][j] < rD[i][j]) {//从i经k到j的一条路径更短
						rD[i][j] = rD[i][k] + rD[k][j];//更新D[i][j]
						rPath[i][j] = rPath[k][j]; //更改j的前驱为k
					}
	}
}