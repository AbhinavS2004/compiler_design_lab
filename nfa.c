#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_INPUTS 10
#define MAX_TRANS 20

// NFA transitions
int nfa[MAX_STATES][MAX_INPUTS][MAX_TRANS];
int nfaTransCount[MAX_STATES][MAX_INPUTS];

// DFA
int dfa[1 << MAX_STATES][MAX_INPUTS];
int dfaUsed[1 << MAX_STATES];
int dfaFinal[1 << MAX_STATES];

// Parameters
int numStates, numInputs, startState, numFinals, finalStates[MAX_STATES];
char inputs[MAX_INPUTS];

// check if state is final
int isFinal(int s) {
    for (int i = 0; i < numFinals; i++)
        if (finalStates[i] == s) return 1;
    return 0;
}

// print subset
void printSubset(int mask) {
    printf("{");
    int first = 1;
    for (int i = 0; i < numStates; i++) {
        if (mask & (1 << i)) {
            if (!first) printf(",");
            printf("q%d", i);
            first = 0;
        }
    }
    printf("}");
}

// get input index
int getInputIndex(char c) {
    for (int i = 0; i < numInputs; i++)
        if (inputs[i] == c) return i;
    return -1;
}

// subset construction
void nfaToDfa() {
    int queue[1 << MAX_STATES], front = 0, rear = 0;
    int startMask = 1 << startState;
    queue[rear++] = startMask;
    dfaUsed[startMask] = 1;

    while (front < rear) {
        int curr = queue[front++];
        for (int a = 0; a < numInputs; a++) {
            int next = 0;
            for (int s = 0; s < numStates; s++) {
                if (curr & (1 << s)) {
                    for (int k = 0; k < nfaTransCount[s][a]; k++)
                        next |= (1 << nfa[s][a][k]);
                }
            }
            dfa[curr][a] = next;
            if (next && !dfaUsed[next]) {
                dfaUsed[next] = 1;
                queue[rear++] = next;
            }
        }
    }

    // mark final DFA states
    for (int mask = 0; mask < (1 << numStates); mask++) {
        if (dfaUsed[mask]) {
            for (int s = 0; s < numStates; s++) {
                if ((mask & (1 << s)) && isFinal(s)) {
                    dfaFinal[mask] = 1;
                    break;
                }
            }
        }
    }
}

// main
int main() {
    printf("Enter number of input symbols: ");
    scanf("%d", &numInputs);

    printf("Enter input symbols: ");
    for (int i = 0; i < numInputs; i++)
        scanf(" %c", &inputs[i]);

    printf("Enter number of states: ");
    scanf("%d", &numStates);

    printf("Enter start state (0..%d): ", numStates - 1);
    scanf("%d", &startState);

    printf("Enter number of final states: ");
    scanf("%d", &numFinals);
    printf("Enter final states: ");
    for (int i = 0; i < numFinals; i++) scanf("%d", &finalStates[i]);

    int numTransitions;
    printf("Enter number of transitions: ");
    scanf("%d", &numTransitions);

    memset(nfaTransCount, 0, sizeof(nfaTransCount));

    printf("Enter transitions (fromState inputSymbol toState):\n");
    for (int i = 0; i < numTransitions; i++) {
        int from, to;
        char sym;
        scanf("%d %c %d", &from, &sym, &to);
        int idx = getInputIndex(sym);
        if (idx == -1) {
            printf("Invalid symbol: %c\n", sym);
            return 1;
        }
        nfa[from][idx][nfaTransCount[from][idx]++] = to;
    }

    nfaToDfa();

    printf("\n--- DFA ---\n");
    printf("States: ");
    for (int mask = 0; mask < (1 << numStates); mask++)
        if (dfaUsed[mask]) { printSubset(mask); printf(" "); }
    printf("\n");

    printf("Start state: ");
    printSubset(1 << startState);
    printf("\n");

    printf("Final states: ");
    for (int mask = 0; mask < (1 << numStates); mask++)
        if (dfaFinal[mask]) { printSubset(mask); printf(" "); }
    printf("\n");

    printf("Transitions:\n");
    for (int mask = 0; mask < (1 << numStates); mask++) {
        if (dfaUsed[mask]) {
            for (int a = 0; a < numInputs; a++) {
                if (dfa[mask][a]) {
                    printSubset(mask);
                    printf(" --%c--> ", inputs[a]);
                    printSubset(dfa[mask][a]);
                    printf("\n");
                }
            }
        }
    }

    return 0;
}

