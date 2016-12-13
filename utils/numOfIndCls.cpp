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

void helpMessage() {
    puts("Unexpected parameter");
    puts("./nomOfIndCls testcase.cnf");
}

vector<int> root;
int findRoot(int id) {
    return root[id] == id ? id : (root[id] = findRoot(root[id]));
}
void unionRoot(int a, int b) {
    a = findRoot(a);
    b = findRoot(b);
    root[b] = a;
}

int main(int argc, const char *argv[]) {
    if( argc != 2 ) {
        helpMessage();
        exit(1);
    }

    int maxVarIndex;
    vector< vector<int> > cnf;
    parse_DIMACS_CNF(cnf, maxVarIndex, argv[1]);

    root = vector<int>(maxVarIndex+1);
    for(int i=0; i<=maxVarIndex; ++i)
        root[i] = i;
    for(auto &cls : cnf)
        for(int i=1, rid=abs(cls[0]); i<cls.size(); ++i)
            unionRoot(rid, abs(cls[i]));

    int cnt = 0;
    for(int i=1; i<=maxVarIndex; ++i)
        cnt += (i == findRoot(i));
    printf("%d\n", cnt);

    return 0;
}
