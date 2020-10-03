#pragma once
#include"vector.h"
#include"stack.h"
#include "queue.h"


namespace mystl {

	typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;		//����״̬
	typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EStatus; //��״̬

	template<typename Tv, typename Te> // node, edge
	class Graph {
	private:
		void reset() {
			for (int i = 0; i < n;i++) {
				status(i) = UNDISCOVERED; dTime(i) = fTime(i) = -1; // ״̬��ʱ���ǩ
				parent(i) = -1; priority(i) = INT_MAX; // �ڱ������ֵĸ��ڵ㣬���ȼ���
				for (int j = 0;j < n;j++) { // ���бߵ�
					if (exists(i, j)) status(i, j) = UNDETERMINED; //״̬
				}
			}
		}

		void BFS(int, int&); // (��ͨ��)BFS
		void DFS(int, int&);
		void BCC(int, int&, stack<int>&); //��ͨ�����DFS��˫��ͨ������ֽ��㷨
		bool TSort(int, int&, stack<Tv>*); //��ͨ�����DFS�����������㷨
		template<typename PU>
		void PFS(int, PU);//��ͨ�� ���ȼ��������
	public:
		//node
		int n;//��������
		virtual int insert(Tv const&) = 0; //���붥�㣬���ر��
		virtual Tv remove(int) = 0;//ɾ�������Լ�������ߣ����ظö�����Ϣ
		virtual Tv& vertex(int) = 0; //����v������
		virtual int inDegree(int) = 0; //���
		virtual int outDegree(int) = 0; //����
		virtual int firstNbr(int) = 0; //����v���׸��ڽӶ���
		virtual int nextNbr(int, int) = 0;//����v������ڶ���j����һ�ڽӶ���
		virtual VStatus& status(int) = 0; // ����״̬
		virtual int& dTime(int) = 0; //ʱ���ǩdTime   ����ʱ��discovery time
		virtual int& fTime(int) = 0; // ʱ���ǩfTime  ����ʱ��finished  time
		virtual int& parent(int) = 0; //�ڱ������еĸ���
		virtual int& priority(int) = 0; //���ȼ���
		//edge, ���������ת��Ϊ�������һ�������
		int e; // ������
		virtual bool exists(int, int) = 0; //e(u,v)�Ƿ����
		virtual void insert(Te const&, int, int, int) = 0; //��v u֮�����Ȩ��Ϊw�ı�
		virtual Te remove(int, int) = 0; //ɾ��uv��ıߣ����ظ���Ϣ
		virtual EStatus& status(int, int) = 0; // �ߵ�״̬
		virtual Te& edge(int, int) = 0; //��uv������
		virtual int& weight(int, int) = 0; 
		//�㷨
		void bfs(int);
		void dfs(int);
		
		void bcc(int);
		stack<Tv>* tSort(int);  //����dfs�����������㷨
		void prim(int); //��С֧����prim
		void dijkstra(int); 
		template<typename PU> void pfs(int, PU); //���ȼ��������
	};


	// �������
	template<typename Tv>
	struct Vertex {
		Tv data; 
		int inDegree, outDegree;
		VStatus status;
		int dTime, fTime;
		int parent, priority;
		Vertex(Tv const& d= (Tv)0):
			data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
			dTime(-1), fTime(-1), parent(-1), priority(INT_MAX){}
	};

	// �߶���
	template<typename Te>
	struct Edge {
		Te data; int weight; EStatus status;
		Edge(Te const& d, int w) :data(d), weight(w), Status(UNDETERMINED) {};
	};



	template<typename Tv, typename Te>
	class GraphMatrix :public Graph<Tv, Te> {
	private:
		vector<Vertex<Tv>> V;
		vector<vector<Edge<Te>*>> E; // �ߣ� �ڽӾ���
	public:
		GraphMatrix() { n = e = 0; }
		~GraphMatrix() {
			for (int j = 0;j < n;j++)
				for (int k = 0;k < n;k++)
					delete E[j][k];
		}
		// �����������
		virtual Tv& vertex(int i)		{ return V[i].data; }
		virtual int inDegree(int i)		{ return V[i].inDegree;}
		virtual int outDegree(int i)	{ return V[i].outDegree;}
		virtual int firstNbr(int i) { return nestNbr(i, n); }
		virtual int nextNbr(int i, int j) // ����ڶ���j����һ�ڽӶ��� ?
		{
			while ((-1 < j) && (!exists(i, --j))); return j;
		}
		virtual int& dTime(int i) { return V[i].dTime; }
		virtual int& fTime(int i) { return V[i].fTime; }
		virtual int& parent(int i) { return V[i].parent; }
		virtual int& priority(int i) { return V[i].priority; }
		
		//����Ķ�̬����
		virtual int insert(Tv const& vertex) { //���붥�㣬���ر��
			for (int j = 0; j < n; j++) E[j].insert(nullptr); n++; //������Ԥ��һ��Ǳ�ڵĹ�����
			E.insert(vector<Edge<Te>*>(n, n, (Edge<Te>*)nullptr)); //�����¶������ı�����
			return  V.insert(Vertex<Tv>(vertex)); //������������һ������
		}

		virtual Tv remove(int i) { // ɾ����i�����㼰������
			// ɾ��
			for(int j=0; j<n;j++)  //�������
				if (exists(i, j)) { delete E[i][j]; V[j].inDegree--; }
			E.remove(i); n--; //ɾ����i��
			for(int j=0; j<n;j++) //���г���
				if (exists(j, i)) { delete E[j].remove(i); V[j].outDegree--; }
			
			Tv vBak = vertex(i); V.remvoe(i); //ɾ������i
			return vBak;
		}

		// �ߵ�ȷ�ϲ���
		virtual bool exists(int i, intj) {
			return (0 <= i) && (i < n) && (0 <= j) && (j < n) && (E[i][j] != nullptr);
		}

		//�ߵĻ�������
		virtual EStatus& status(int i, int j) { return E[i][j]->status; }
		virtual Te& edge(int i, int j) { return E[i][j]->data; }
		virtual int& weight(int i, int j) { return E[i][j]->weight; }

		//�ߵĶ�̬����
		// ����Ȩ��Ϊw��e =(i,j)
		virtual void insert(Te const& edge, int w, int i, int j) {
			if (exist(i, j)) return; //ȷ���߲�����
			E[i][j] = new Edge<T>(edge, w);
			e++;	V[i].outDegree++;	V[j].inDegree++;
		}

		// ɾ��, �����ر��ڵ�����
		virtual Te remove(int i, int j) {
			Te eBak = edge(i, j); delete E[i][j]; 
			E[i][j] = nullptr;
			e--;	V[i].outDegree--; V[j].inDegree--;
			return eBak;
		}
	};

	//ȫͼ�����ܶ����ͨ��
	template<typename Tv, typename Te>
	void Graph<Tv, Te>::bfs(int s) {
		reset(); 
		int clock = 0;
		int v = s;
		do // ��һ������ж���
			if (UNDISCOVERED == status(v)) // һ��������δ���ֵĶ���
				BFS(v, clock); //�Ӹö����������һ��BFS
		while (s != (v = (++v % n))); //����ż�飬��©����
	}

	template <typename Tv, typename Te> //������ͨ��
	void Graph<Tv, Te>::BFS(int v, int& clock) {
		queue<int> Q;
		status(v) = DISCOVERED; 
		Q.enqueue(v);
		while (!Q.empty()) {
			int v = Q.dequeue(); 
			dTime(v) = ++clock;
			for(int u=fristNbr(v); -1<u; u=nextNbr(v,u))//ö��v���е��ھ�u
				if (UNDISCOVERED == status(u)) {
					status(u) = DISCOVERED; 
					Q.enqueue(u);
					status(v, u) = TREE;
					parent(u) = v; //����������չ֧����
				}
				else { //��u�Ѿ������֣����������ѷ������
					status(v, u) = CROSS; //��(v,u)�����ڿ��
				}
			status(v) = VISITED; //���ˣ� ��ǰ�ڵ�������
		}
	}

	//�����������DFS�㷨��ȫͼ��
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::dfs(int s) { //assert: 0 <= s < n
		reset(); int clock = 0; int v = s; //��ʼ��
		do //��һ������ж���
			if (UNDISCOVERED == status(v)) //һ��������δ���ֵĶ���
				DFS(v, clock); //���Ӹö����������һ��DFS
		while (s != (v = (++v % n))); //����ż�飬�ʲ�©����
	}

	//�����������DFS�㷨��������ͨ��
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::DFS(int v, int& clock) { //assert: 0 <= v < n
		dTime(v) = ++clock; status(v) = DISCOVERED; //���ֵ�ǰ����v
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //ö��v�������ھ�u
			switch (status(u)) { //������״̬�ֱ���
			case UNDISCOVERED: //u��δ���֣���ζ��֧�������ڴ���չ
				type(v, u) = TREE;   status(u) = DISCOVERED; parent(u) = v; DFS(u, clock); break;
			case DISCOVERED: //u�ѱ����ֵ���δ������ϣ�Ӧ�������ָ�������
				type(v, u) = BACKWARD; break;
			default: //u�ѷ�����ϣ�VISITED������ͼ�������ӳ�Ϯ��ϵ��Ϊǰ��߻���
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
			}
		status(v) = VISITED; fTime(v) = ++clock; //���ˣ���ǰ����v����������
	}





	// ����DFS�����������㷨
	template <typename Tv, typename Te> //����DFS�����������㷨
	stack<Tv>* Graph<Tv, Te>::tSort(int s) { //assert: 0 <= s < n
		reset(); int clock = 0; int v = s;
		stack<Tv>* S = new stack<Tv>; //��ջ��¼���򶥵�
		do {
			if (UNDISCOVERED == status(v))
				if (!TSort(v, clock, S)) { //clock���Ǳ���
					/*DSA*/print(S);
					while (!S->empty()) //��һ��ͨ���༴��ͼ����DAG
						S->pop(); break; //�򲻱ؼ������㣬��ֱ�ӷ���
				}
		} while (s != (v = (++v % n)));
		return S; //������ΪDAG����S�ڸ������Զ�������򣻷��򣨲������������򣩣�S��
	}

	template <typename Tv, typename Te> //����DFS�����������㷨�����ˣ�
	bool Graph<Tv, Te>::TSort(int v, int& clock, stack<Tv>* S) { //assert: 0 <= v < n
		dTime(v) = ++clock; status(v) = DISCOVERED; //���ֶ���v
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //ö��v�������ھ�u
			switch (status(u)) { //����u��״̬�ֱ���
			case UNDISCOVERED:
				parent(u) = v; type(v, u) = TREE;
				if (!TSort(u, clock, S)) //�Ӷ���u��������������
					return false; //��u��������������������ȫͼ�����ˣ����ʷ��ز�����
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD; //һ�����ֺ���ߣ���DAG������
				return false; //�������룬�ʷ��ز�����
			default: //VISITED (digraphs only)
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		status(v) = VISITED; S->push(vertex(v)); //���㱻���ΪVISITEDʱ���漴��ջ
		return true; //v������������������
	}


	// ����DFS������ܵ�˫��ͨ��ֽ��㷨
	template <typename Tv, typename Te> void Graph<Tv, Te>::bcc(int s) { //����DFS��BCC�ֽ��㷨
		reset(); int clock = 0; int v = s; Stack<int> S; //ջS���Լ�¼�ѷ��ʵĶ���
		do
			if (UNDISCOVERED == status(v)) { //һ������δ���ֵĶ��㣨����ͨ������
				BCC(v, clock, S); //���Ӹö����������һ��BCC
				S.pop(); //�������غ󣬵���ջ�����һ�����㡪����ǰ��ͨ������
			}
		while (s != (v = (++v % n)));
	}

#define hca(x) (fTime(x)) //���ô˴����õ�fTime[]�䵱hca[]
	template <typename Tv, typename Te> //�������͡�������
	void Graph<Tv, Te>::BCC(int v, int& clock, stack<int>& S) { //assert: 0 <= v < n
		hca(v) = dTime(v) = ++clock; status(v) = DISCOVERED; S.push(v); //v�����ֲ���ջ
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) //ö��v�������ھ�u
			switch (status(u)) { //����u��״̬�ֱ���
			case UNDISCOVERED:
				parent(u) = v; type(v, u) = TREE; BCC(u, clock, S); //�Ӷ���u������
				if (hca(u) < dTime(v)) //�������غ�������u��ͨ������ߣ���ָ��v��������
					hca(v) = __min(hca(v), hca(u)); //��v������
				else //������vΪ�ؽڵ㣨u���¼���һ��BCC�������ж����ʱ��������ջS�Ķ�����
					/*DSA*/ {
					/*DSA*/printf("BCC rooted at %c:", vertex(v));
					/*DSA*/Stack<int> temp; do { temp.push(S.pop()); print(vertex(temp.top())); } while (u != temp.top()); print(vertex(parent(u))); while (!temp.empty()) S.push(temp.pop());
					while (u != S.pop()); //������ǰBCC�У���v�⣩�����нڵ㣬������Ҫ����һ������
					/*DSA*/printf("\n");
					/*DSA*/
				}
				break;
			case DISCOVERED:
				type(v, u) = BACKWARD; //���(v, u)�������ա�ԽСԽ�ߡ���׼��
				if (u != parent(v)) hca(v) = __min(hca(v), dTime(u)); //����hca[v]
				break;
			default: //VISITED (digraphs only)
				type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
				break;
			}
		status(v) = VISITED; //��v�ķ��ʽ���
	}
#undef hca




	// ���ȼ�����
	template <typename Tv, typename Te> 
	template <typename PU>
	void Graph<Tv, Te>::pfs(int s, PU prioUpdater) { //assert: 0 <= s < n
		reset(); int v = s; //��ʼ��
		do //��һ������ж���
			if (UNDISCOVERED == status(v)) //һ��������δ���ֵĶ���
				PFS(v, prioUpdater); //���Ӹö����������һ��PFS
		while (s != (v = (++v % n))); //����ż�飬�ʲ�©����
	}

	template <typename Tv, typename Te> template <typename PU> //�������͡������͡����ȼ�������
	void Graph<Tv, Te>::PFS(int s, PU prioUpdater) { //���ȼ�������������ͨ��
		priority(s) = 0; status(s) = VISITED; parent(s) = -1; //��ʼ�������s����PFS����
		while (1) { //����һ����ͱ߼���PFS����
			for (int w = firstNbr(s); -1 < w; w = nextNbr(s, w)) //ö��s�������ھ�w
				prioUpdater(this, s, w); //���¶���w�����ȼ����丸����
			for (int shortest = INT_MAX, w = 0; w < n; w++)
				if (UNDISCOVERED == status(w)) //����δ����������Ķ�����
					if (shortest > priority(w)) //ѡ����һ��
					{
						shortest = priority(w); s = w;
					} //���ȼ���ߵĶ���s
			if (VISITED == status(s)) break; //ֱ�����ж�����Ѽ���
			status(s) = VISITED; type(parent(s), s) = TREE; //��s�����丸�����߼��������
		}
	} //ͨ�������������ȼ����²���prioUpdater������ʵ�ֲ�ͬ���㷨����



	template <typename Tv, typename Te> struct PrimPU { //���Prim�㷨�Ķ������ȼ�������
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if (UNDISCOVERED == g->status(v)) //����ukÿһ��δ�����ֵ��ڽӶ���v
				if (g->priority(v) > g->weight(uk, v)) { //��Prim�������ɳ�
					g->priority(v) = g->weight(uk, v); //�������ȼ�������
					g->parent(v) = uk; //���¸��ڵ�
				}
		}
	};



	template <typename Tv, typename Te> //Prim�㷨��������ͨͼ�����߱�ʾΪ�����桢Ȩ����ȵ�һ�Ա�
	void Graph<Tv, Te>::prim(int s) { //assert: 0 <= s < n
		reset(); priority(s) = 0;
		for (int i = 0; i < n; i++) { //��������n�������n-1����
			status(s) = VISITED;
			if (-1 != parent(s)) type(parent(s), s) = TREE; //���뵱ǰ��s
			for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) //ö��s�������ھ�j
				if ((status(j) == UNDISCOVERED) && (priority(j) > weight(s, j))) //���ڽӶ���j���ɳ�
				{
					priority(j) = weight(s, j); parent(j) = s;
				} //��Dijkstra�㷨Ψһ�Ĳ�֮ͬ��
			for (int shortest = INT_MAX, j = 0; j < n; j++) //ѡ����һ���̿��
				if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
				{
					shortest = priority(j); s = j;
				}
		}
	}





	//���Dijkstra�㷨�Ķ������ȼ�������
	template <typename Tv, typename Te> struct DijkPU { 
		virtual void operator() (Graph<Tv, Te>* g, int uk, int v) {
			if (UNDISCOVERED == g->status(v)) //����ukÿһ��δ�����ֵ��ڽӶ���v����Dijkstra����
				if (g->priority(v) > g->priority(uk) + g->weight(uk, v)) { //���ɳ�
					g->priority(v) = g->priority(uk) + g->weight(uk, v); //�������ȼ�������
					g->parent(v) = uk; //��ͬʱ���¸��ڵ�
				}
		}
	};


	//���·��Dijkstra�㷨��������һ�������ͼ
	template <typename Tv, typename Te> 
	void Graph<Tv, Te>::dijkstra(int s) { //assert: 0 <= s < n
		reset(); priority(s) = 0;
		for (int i = 0; i < n; i++) { //��������n�������n-1����
			status(s) = VISITED;
			if (-1 != parent(s)) type(parent(s), s) = TREE; //���뵱ǰ��s
			for (int j = firstNbr(s); -1 < j; j = nextNbr(s, j)) //ö��s�������ھ�j
				if ((status(j) == UNDISCOVERED) && (priority(j) > priority(s) + weight(s, j))) //���ڽӶ���j���ɳ�
				{
					priority(j) = priority(s) + weight(s, j); parent(j) = s;
				} //��Prim�㷨Ψһ�Ĳ�֮ͬ��
			for (int shortest = INT_MAX, j = 0; j < n; j++) //ѡ����һ�������
				if ((status(j) == UNDISCOVERED) && (shortest > priority(j)))
				{
					shortest = priority(j); s = j;
				}
		}
	} //����������ͨͼ������ÿһ���߱�ʾΪ�����桢Ȩ����ȵ�һ�Ա�



};