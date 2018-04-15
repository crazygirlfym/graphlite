/*2, 201628015029030, yanmei fu   */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <set>
#include <iostream>
#include "GraphLite.h"

#define VERTEX_CLASS_NAME(name) GraphColor##name

#define EPS 1e-6
int v0_id ;
int color_number;

class VERTEX_CLASS_NAME(InputFormatter): public InputFormatter {
public:
    int64_t getVertexNum() {
        unsigned long long n;
        sscanf(m_ptotal_vertex_line, "%lld", &n);
        m_total_vertex= n;
        return m_total_vertex;
    }
    int64_t getEdgeNum() {
        unsigned long long n;
        sscanf(m_ptotal_edge_line, "%lld", &n);
        m_total_edge= n;
        return m_total_edge;
    }
    int getVertexValueSize() {
        m_n_value_size = sizeof(int);
        return m_n_value_size;
    }
    int getEdgeValueSize() {
        m_e_value_size = sizeof(int);
        return m_e_value_size;
    }
    int getMessageValueSize() {
        m_m_value_size = sizeof(int);
        return m_m_value_size;
    }
    void loadGraph() {
        unsigned long long last_vertex;
        unsigned long long from;
        unsigned long long to;
        int weight = 1;
        
        int value = -1;
        int outdegree = 0;
        
        const char *line= getEdgeLine();

        // Note: modify this if an edge weight is to be read
        //       modify the 'weight' variable

        sscanf(line, "%lld %lld", &from, &to);
        addEdge(from, to, &weight);

        last_vertex = from;
        if(last_vertex==v0_id){
            value = 0;
        }
        ++outdegree;
        for (int64_t i = 1; i < m_total_edge; ++i) {
            line= getEdgeLine();

            // Note: modify this if an edge weight is to be read
            //       modify the 'weight' variable

            sscanf(line, "%lld %lld", &from, &to);
            if (last_vertex != from) {
                addVertex(last_vertex, &value, outdegree);
                last_vertex = from;
                outdegree = 1;
            } else {
                ++outdegree;
            }
            addEdge(from, to, &weight);
        }
        addVertex(last_vertex, &value, outdegree);
    }
};

class VERTEX_CLASS_NAME(OutputFormatter): public OutputFormatter {
public:
    void writeResult() {
        int64_t vid;
        int value;
        char s[1024];

        for (ResultIterator r_iter; ! r_iter.done(); r_iter.next() ) {
            r_iter.getIdValue(vid, &value);
            int n = sprintf(s, "%lld: %d\n", (unsigned long long)vid, value);
            writeNextResLine(s, n);
        }
    }
};

// An aggregator that records a int value tom compute sum
class VERTEX_CLASS_NAME(Aggregator): public Aggregator<int> {
public:
    void init() {
        m_global = 0;
        m_local = 0;
    }
    void* getGlobal() {
        return &m_global;
    }
    void setGlobal(const void* p) {
        m_global = * (int *)p;
    }
    void* getLocal() {
        return &m_local;
    }
    void merge(const void* p) {
        m_global += * (int *)p;
    }
    void accumulate(const void* p) {
        m_local += * (int *)p;
    }
};

class VERTEX_CLASS_NAME(): public Vertex <int, int, int> {
public:
    void compute(MessageIterator* pmsgs) {
        int val;
        if (getSuperstep() == 0) {
               //val= 0;
           if (getVertexId() != v0_id){
                val =(rand() % (color_number-1))+ 1;
           }
        } else {
            if (getSuperstep() >= 2) {
                int global_val = * (int *)getAggrGlobal(0);
                if (global_val < EPS) {
                    voteToHalt(); return;
                }
            }

            //double sum = 0;
            set<int> nearbyColors;
            for(; !pmsgs->done(); pmsgs->next()){
                if (pmsgs->getValue()!=-1){
                    nearbyColors.insert(pmsgs->getValue());
                }

            }

                // no conflict, no change
            if (getValue()!= -1 && nearbyColors.find(getValue())==nearbyColors.end()){
                    voteToHalt(); return;
            }else{
		
                while(true){
			val = (rand() % (color_number-0))+ 0;
                	if(nearbyColors.size() == color_number)
                	{
                		val = -1;
				break;
                	}else{
                    		if (nearbyColors.find(val) == nearbyColors.end())
                    		{
                            //if (val!=getValue())
                            break;
                    		}
			}
                        
                    }
                }

            



            //for ( ; ! pmsgs->done(); pmsgs->next() ) {
                //sum += pmsgs->getValue();
            //}
            //val = 0.15 + 0.85 * sum;

            //double acc = fabs(getValue() - val);
		int acc;
		if (val  != -1){
            acc = abs(getValue()-val);
}else{
acc = 1;
}
		if (acc !=0){
		cout  << getVertexId()<< " "<<getValue() << " " << val <<endl;
		}
            accumulateAggr(0, &acc);
        }
	
        * mutableValue() = val;
        //const int64_t n = getOutEdgeIterator().size();
        //sendMessageToAllNeighbors(val / n);
        sendMessageToAllNeighbors(val);
    }
};

class VERTEX_CLASS_NAME(Graph): public Graph {
public:
    VERTEX_CLASS_NAME(Aggregator)* aggregator;

public:
    // argv[0]: GraphColor.so
    // argv[1]: <input path>
    // argv[2]: <output path>
    // argv[3]: <v0 id>
    // argv[4]: <color number>
    void init(int argc, char* argv[]) {

        setNumHosts(5);
        setHost(0, "localhost", 1411);
        setHost(1, "localhost", 1421);
        setHost(2, "localhost", 1431);
        setHost(3, "localhost", 1441);
        setHost(4, "localhost", 1451);

        if (argc < 5) {
           printf ("Usage: %s <input path> <output path> <v0 id> <color number>\n", argv[0]);
           exit(1);
        }

        m_pin_path = argv[1];
        m_pout_path = argv[2];
        v0_id = atoi(argv[3]);
        color_number = atoi(argv[4]);

        aggregator = new VERTEX_CLASS_NAME(Aggregator)[1];
        regNumAggr(1);
        regAggr(0, &aggregator[0]);
    }

    void term() {
        delete[] aggregator;
    }
};

/* STOP: do not change the code below. */
extern "C" Graph* create_graph() {
    Graph* pgraph = new VERTEX_CLASS_NAME(Graph);

    pgraph->m_pin_formatter = new VERTEX_CLASS_NAME(InputFormatter);
    pgraph->m_pout_formatter = new VERTEX_CLASS_NAME(OutputFormatter);
    pgraph->m_pver_base = new VERTEX_CLASS_NAME();

    return pgraph;
}

extern "C" void destroy_graph(Graph* pobject) {
    delete ( VERTEX_CLASS_NAME()* )(pobject->m_pver_base);
    delete ( VERTEX_CLASS_NAME(OutputFormatter)* )(pobject->m_pout_formatter);
    delete ( VERTEX_CLASS_NAME(InputFormatter)* )(pobject->m_pin_formatter);
    delete ( VERTEX_CLASS_NAME(Graph)* )pobject;
}
