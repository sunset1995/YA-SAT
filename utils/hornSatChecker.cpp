#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
using namespace std;

/*********************************************************************
               ================
               CNF Parsing code
               ================
**********************************************************************/
#include <iostream>

#define CHUNK_LIMIT 1048576

class StreamBuffer {
    FILE   *in;
    char    buf[CHUNK_LIMIT];
    int     pos;
    int     size;

    void assureLookahead() {
        if (pos >= size) {
            pos  = 0;
            size = fread(buf, 1, sizeof(buf), in);
        } 
    }

public:
    StreamBuffer(FILE *i) : in(i), pos(0), size(0) {
        assureLookahead();
    }

    int  operator *  () { return (pos >= size) ? EOF : buf[pos]; }
    void operator ++ () { pos++; assureLookahead(); }
};

void skipWhitespace(StreamBuffer &in) {
    while ((*in >= 9 && *in <= 13) || *in == 32)
        ++in;
}


void skipLine(StreamBuffer &in) {
    while (true) {
        if (*in == EOF) return;
        if (*in == '\n') { ++in; return; }
        ++in;
    }
}


int parseInt(StreamBuffer &in) {
    int     val = 0;
    bool    neg = false;
    skipWhitespace(in);
    if      (*in == '-') neg = true, ++in;
    else if (*in == '+') ++in;
    if (*in < '0' || *in > '9')
    fprintf(stderr, "PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
    while (*in >= '0' && *in <= '9') {
        val = val*10 + (*in - '0');
        ++in;
    }
    return neg ? -val : val;
}


void readClause(StreamBuffer &in, vector<vector<int> > &clauses) {
    int parsed_lit;
    vector<int> newClause;
    while (true) {
        parsed_lit = parseInt(in);
        if (parsed_lit == 0) break;
        newClause.push_back(parsed_lit);
    }
    clauses.push_back(newClause);
}


void parse_DIMACS_main(StreamBuffer &in, vector<vector<int> > &clauses) {
    while (true) {
        skipWhitespace(in);
        if (*in == EOF) break;
        else if (*in == 'c' || *in == 'p') skipLine(in);
        else readClause(in, clauses);
    }
}


void parse_DIMACS(FILE *input_stream, vector<vector<int> > &clauses) {
    StreamBuffer in(input_stream);
    parse_DIMACS_main(in, clauses);
}


void parse_DIMACS_CNF(vector<vector<int> > &clauses, int &maxVarIndex, const char *DIMACS_cnf_file) {
    unsigned int i, j;
    int candidate;
    FILE *in = fopen(DIMACS_cnf_file, "r");
    if (in == NULL) {
        fprintf(stderr, "ERROR! Could not open file: %s\n",
        DIMACS_cnf_file);
        exit(1);
    }
    parse_DIMACS(in, clauses);
    fclose(in);

    maxVarIndex = 0;
    for (i = 0; i < clauses.size(); ++i)
        for (j = 0; j < clauses[i].size(); ++j) {
            candidate = abs(clauses[i][j]);
            if (candidate > maxVarIndex) maxVarIndex = candidate;
        }
}
/*********************************************************************/


/*********************************************************************
               ==============
               Two SAT solver
               ==============
**********************************************************************/
class TwoSAT {
private:
    static const int maxN = 100004;
    static const int size = 2*maxN + 4;
    bool        pick[size];
    vector<int> G   [size];

    int id(int i, int T) { return (i<<1) + T; }
    int alter(int i) { return i^1; }
    bool dfsTry(int nowAt, vector<int> &stk) {
        if( pick[alter(nowAt)] )
            return false;
        stk.emplace_back(nowAt);
        pick[nowAt] = true;
        for(auto v : G[nowAt]) {
            if( !pick[v] && !dfsTry(v, stk) )
                return false;
        }
        return true;
    }
public:
    void init() {
        memset(pick, 0, sizeof(pick));
        for(int i=0; i<size; ++i)
            G[i].clear();
    }
    void addClause(bool TA, int A, bool TB, int B) {
        // Add clause (TA + TB)
        // When TA not true, TB must true. vise versa.
        G[id(A, !TA)].emplace_back(id(B, TB));
        G[id(B, !TB)].emplace_back(id(A, TA));
    }
    void imply(bool TA, int A, bool TB, int B) {
        addClause(!TA, A, TB, B);
    }
    bool solve() {
        // O(n) solve
        memset(pick, 0, sizeof(pick));
        for(int i=0; i<maxN; ++i) {
            if( pick[id(i, 0)] || pick[id(i, 1)] )
                continue;
            vector<int> stk;
            if( dfsTry(id(i, 0), stk) )
                continue;
            for(auto v : stk)
                pick[v] = false;
            if( !dfsTry(id(i, 1), stk) )
                return false;
        }
        return true;
    }
    bool T(int i) {
        // should solve() first
        return pick[id(i, 1)];
    }
} solver;
/*********************************************************************/


void helpMessage() {
    puts("Unexpected parameter");
    puts("./hornSatChecker testcase.cnf");
}


int main(int argc, const char *argv[]) {
    if( argc != 2 ) {
        helpMessage();
        exit(1);
    }

    int maxVarIndex;
    vector< vector<int> > cnf;
    parse_DIMACS_CNF(cnf, maxVarIndex, argv[1]);

    solver.init();
    for(auto &cls : cnf)
        for(int i=0; i<cls.size(); ++i) {
            int id_1 = abs(cls[i]);
            int sign_1 = cls[i] > 0;
            for(int j=0; j<cls.size(); ++j) {
                if( i==j ) continue;
                int id_2 = abs(cls[j]);
                int sign_2 = cls[j] > 0;
                solver.imply(!sign_1, id_1, sign_2, id_2);
            }
        }

    puts( solver.solve() ? "Horn SAT" : "None" );
    /*
    for(auto &cls : cnf) {
        for(int i=0; i<cls.size(); ++i)
            printf("%d ", cls[i]);
        puts("");
        for(int i=0; i<cls.size(); ++i)
            printf("%d ", solver.T(abs(cls[i])) ? -cls[i] : cls[i] );
        puts("\n============");
    }
    */

    return 0;
}
