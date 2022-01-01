#include <cstdlib>
#include <iostream>
#include <string>

static const int WIDTH = 5;
static const int DEPTH = 4;

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

class Tree {
public:
    Tree(Tree *p = nullptr) : parent(p), value(-1.0)
                  {for (int i = 0; i < WIDTH; ++i) children[i] = nullptr;}
    ~Tree() {for (int i = 0; i < WIDTH; ++i) delete children[i];}
    Tree* children[WIDTH], *parent;
    double value;
};

Tree* makeTree(Tree *parent = nullptr, int level = DEPTH) {
    Tree* tree = new Tree(parent);
    if (level <= 0) {
        tree->value = fRand(0.0, 1.0);
    }
    else {
        for (int i = 0; i < WIDTH; ++i)
            tree->children[i] = makeTree(tree, level-1);
    }
    return tree;
}

void describe (int level, Tree *node) {
    std::cout << "level " << level << ", value " << node->value << '\n';
    std::cout << "children: ";
    for (int i = 0; i < WIDTH; ++ i) {
        if (node->children[i]) {
            if (node->children[i]->value < 0.0)
                std::cout << "empty " ;
            else
                std::cout << node->children[i]->value << ' ';
        }
        else
            std::cout << "none ";
    }
    std::cout << '\n';
}

void inspect(Tree *tree) {
    int level = DEPTH;
    while (true) {
        std::string instruction;
        std::cout << "where? ";
        std::cin >> instruction;
        if (instruction == "up") {
            if (tree->parent) {
                tree = tree->parent;
                describe(++level, tree);
            }
            else
                std::cout << "stand still\n";
        }
        else if (instruction == "down") {
            int i;
            std::cin >> i;
            if (i >= 0 && i < WIDTH && tree->children[i]) {
                tree = tree->children[i];
                describe(--level, tree);
            }
            else
                std::cout << "stand still\n";
        }
        else if (instruction == "quit")
            break;
        else
            describe(level, tree);
    }
}

bool better(bool white, double value, double storage) {
    if (storage < 0.0)
        return true;
    return (white && value > storage) || (!white && value < storage);
}

bool out_of_limits(bool white, double value, double alpha, double beta) {
    return (white && value > beta) || (!white && value < alpha);
}

double minimax(Tree *node, double alpha = 0.0, double beta = 1.0, bool white = true) {
    double result = -1.0;
    for (int i = 0; i < WIDTH; ++i) {
        if (node->children[i]) {
            double value = minimax(node->children[i], alpha, beta, !white);
            if (out_of_limits(white, value, alpha, beta)) {
                node->value = -1.0;
                return value;
            }
            if (better(white, value, result)) {
                result = value;
                if (white)
                    alpha = value;
                else
                    beta = value;
            }
        }
    }
    if (result >= 0.0) {    // промежуточный узел
        node->value = result;
        return result;
    }
    else                    // лист
        return node->value;
}

int main() {
    Tree *tree = makeTree();
    minimax(tree);
    inspect(tree);
    return 0;
}
