#include <iostream>
#include <bitset>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>

const unsigned MAX_KEY_LENGTH = 257;
const unsigned TREE_DEGREE = 500;
const unsigned MAX_AMOUNT = 2 * TREE_DEGREE - 1;
const char SEPARATOR = '$';
const int BITS = 8;
const int MAX_SHIFT = 7;
const int DEFAULT = -100;

std::string CodeLetter(char &a)
{
    return std::bitset<BITS>(a - 'a' + 1).to_string();
}

void CodeWord(std::string &key)
{
    std::transform(key.begin(), key.end(),
                   key.begin(), tolower);
    std::string temp;
    for (auto i : key)
    {
        temp += CodeLetter(i);
    }
    key = temp;
}

int GetAmountOfLeadingZeroes(std::string &string)
{
    int amount = 0;
    while (string[amount] == '0')
    {
        ++amount;
    }
    return amount;
}

bool GetIndex(std::string &word, int index)
{
    if (index / BITS >= word.size())
    {
        return false;
    }
    return (word[index / BITS] & ((1u << MAX_SHIFT) >> (index % BITS)));
}

int FirstDifferentBit(std::string &word1, std::string &word2)
{
    for (int i = 0; i < std::max(word1.size(), word2.size()) * BITS; ++i)
    {
        if (GetIndex(word1, i) != GetIndex(word2, i))
        {
            return i;
        }
    }
    return -1;
}

class Node
{
public:
    std::string key;
    unsigned long long value;
    int index;
    Node *left;
    Node *right;

    Node()
    {
        key = " ";
        value = 0;
        index = DEFAULT;
        left = right = nullptr;
    }

    ~Node()
    {
        right = left = nullptr;
    }
};

class Patricia
{
public:
    Node *root;

    Patricia()
    {
        root = nullptr;
    }

    bool IsEmpty()
    {
        if (root == nullptr)
        {
            return true;
        }
        return false;
    }

    void Search(std::string &key)
    {
        if (IsEmpty())
        {
            std::cout << "NoSuchWord\n";
            return;
        }
        if (root->key == key)
        {
            std::cout << "OK: " << root->value << "\n";
            return;
        }
        Node *previous_node = root;
        Node *current_node = root->left;
        while (current_node->index >
               previous_node->index)
        {
            previous_node = current_node;
            if (GetIndex(key, current_node->index))
            {
                current_node = current_node->right;
            }
            else
            {
                current_node = current_node->left;
            }
        }
        if (current_node->key == key)
        {
            std::cout << "OK: " << current_node->value << "\n";
            return;
        }
        else
        {
            std::cout << "NoSuchWord\n";
            return;
        }
    }

    bool Add(std::string &key, const unsigned long long &value)
    {
        // Tree is empty
        if (IsEmpty())
        {
            Node *node = new Node;
            node->key = key;
            node->value = value;
            node->left = node;
            node->index = 0;
            node->right = nullptr;
            root = node;
            return true;
        }
        else
        {
            Node *previous_node = root;
            Node *current_node = root->left;
            while (current_node->index > previous_node->index)
            {
                previous_node = current_node;
                if (GetIndex(key, current_node->index))
                {
                    current_node = current_node->right;
                }
                else
                {
                    current_node = current_node->left;
                }
            }
            // Node already exists
            if (current_node->key == key)
            {
                return false;
            }
            Node *node = new Node;
            node->key = key;
            node->value = value;
            node->index = FirstDifferentBit(current_node->key, key);
            previous_node = root;
            current_node = root->left;
            while (current_node->index > previous_node->index && current_node->index <
                                                                     node->index)
            {
                previous_node = current_node;
                if (GetIndex(key, current_node->index))
                {
                    current_node = current_node->right;
                }
                else
                {
                    current_node = current_node->left;
                }
            }
            if (GetIndex(key, node->index))
            {
                node->left = current_node;
                node->right = node;
            }
            else
            {
                node->left = node;
                node->right = current_node;
            }
            if (previous_node->right ==
                current_node)
            {
                previous_node->right = node;
            }
            else
            {
                previous_node->left = node;
            }
        }
        return true;
    }

    void Delete(std::string &key)
    {
        // Checks if tree is empty - nothing to delete
        if (IsEmpty())
        {
            std::cout << "NoSuchWord\n";
            return;
        }
        // Root is the only node - delete root or nothing
        if (root->left == root)
        {
            if (root->key == key)
            {
                delete root;
                root = nullptr;
                std::cout << "OK\n";
                return;
            }
            else
            {
                std::cout << "NoSuchWord\n";
                return;
            }
        }
        // Node is somewhere below
        Node *parent_node;
        Node *owner_node = root;
        Node *node_to_delete = root->left;
        while (node_to_delete->index > owner_node->index)
        {
            parent_node = owner_node;
            owner_node = node_to_delete;
            if (GetIndex(key, node_to_delete->index))
            {
                node_to_delete = node_to_delete->right;
            }
            else
            {
                node_to_delete = node_to_delete->left;
            }
        }
        if (!(node_to_delete->key == key))
        {
            std::cout << "NoSuchWord\n";
            return;
        }
        // Node is a leaf
        if (owner_node == node_to_delete)
        {
            if (parent_node->right == node_to_delete)
            {
                if (node_to_delete->right == node_to_delete)
                {
                    parent_node->right = node_to_delete->left;
                }
                else
                {
                    parent_node->right = node_to_delete->right;
                }
            }
            else
            {
                if (node_to_delete->right == node_to_delete)
                {
                    parent_node->left = node_to_delete->left;
                }
                else
                {
                    parent_node->left = node_to_delete->right;
                }
            }
            delete node_to_delete;
            node_to_delete = nullptr;
            std::cout << "OK\n";
            return;
        }
        // Node is not a leaf
        Node *p = node_to_delete;
        Node *q = owner_node;
        Node *parent_of_q = parent_node;
        Node *r;
        node_to_delete = root->left;
        r = root;
        while (node_to_delete->index > r->index)
        {
            r = node_to_delete;
            if (GetIndex(q->key, node_to_delete->index))
            {
                node_to_delete = node_to_delete->right;
            }
            else
            {
                node_to_delete = node_to_delete->left;
            }
        }
        bool is_from_right = GetIndex(r->key,
                                      q->index);
        if (r->right == q)
        {
            r->right = p;
        }
        else
        {
            r->left = p;
        }
        if (parent_of_q->right == q)
        {
            if (is_from_right)
            {
                parent_of_q->right = q->right;
            }
            else
            {
                parent_of_q->right = q->left;
            }
        }
        else
        {
            if (is_from_right)
            {
                parent_of_q->left = q->right;
            }
            else
            {
                parent_of_q->left = q->left;
            }
        }
        // Don't need to move anything - just exchanging values
        p->key = q->key;
        p->value = q->value;
        delete q;
        q = nullptr;
        std::cout << "OK\n";
    }

    void ClearSubTree(Node *node)
    {
        if (node->left->index > node->index)
        {
            ClearSubTree(node->left);
        }
        if (node->right->index > node->index)
        {
            ClearSubTree(node->right);
        }
        delete node;
        node = nullptr;
    }

    void ClearTree()
    {
        if (!root)
        {
            return;
        }
        if (root != root->left)
        {
            ClearSubTree(root->left);
        }
        delete root;
        root = nullptr;
    }

    void Save(Node *node, std::ofstream &file)
    {
        if (!node)
        {
            std::cout << "OK\n";
            return;
        }
        int index = node->index;
        int size_of_key = node->key.size();
        file.write((char *)(&size_of_key), sizeof(int));
        file.write(node->key.c_str(), sizeof(char) * node->key.size());
        file.write((char *)(&index), sizeof(int));
        file.write((char *)(&node->value), sizeof(unsigned long long));
        if (node->left != nullptr && node->left->index > index)
        {
            Save(node->left, file);
        }
        if (node->right != nullptr && node->right->index > index)
        {
            Save(node->right, file);
        }
        if (node->index == 0)
        {
            std::cout << "OK\n";
        }
    }

    void Load(std::ifstream &file)
    {
        int size_of_key;
        Patricia trie;
        while (file.read((char *)&size_of_key, sizeof(size_of_key)))
        {
            unsigned long long value;
            int index;
            std::string key;
            key.resize(size_of_key);
            file.read((char *)key.data(), size_of_key);
            file.read((char *)&index, sizeof(index));
            file.read((char *)&value, sizeof(value));
            trie.Add(key, value);
        }
        std::swap(*this, trie);
    }

    void PrintTree(Node *node)
    {
        if (node != nullptr)
        {
            std::cout << node->key << " " << node->index << " " << node->value << "\n";
            if (node->left != nullptr && node->index < node->left->index)
            {
                PrintTree(node->left);
            }
            if (node->right != nullptr && node->index < node->right->index)
            {
                PrintTree(node->right);
            }
        }
    }

    ~Patricia()
    {
        ClearTree();
    }
};

int main(int args, char **argv)
{
    unsigned amount;
    std::cin >> amount;
    auto *tree = new Patricia;
    unsigned curSearchPosition;
    char command;
    std::string buf;
    unsigned long long curValue;

    // TEST 1 - ADDING ELEMENTS TO PATRICIA
    std::chrono::steady_clock::time_point begin_of_adding = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf >> curValue;
        std::transform(buf.begin(), buf.end(),
                       buf.begin(), tolower);
        if (tree->Add(buf, curValue))
        {
            std::cout << "OK\n";
        }
        else
        {
            std::cout << "Exist\n";
        }
    }
    std::chrono::steady_clock::time_point end_of_adding = std::chrono::steady_clock::now();
    unsigned addition_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_adding - begin_of_adding).count();

    // TEST 2 - FINDING ELEMENTS IN PATRICIA
    std::chrono::steady_clock::time_point begin_of_finding = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf;
        std::transform(buf.begin(), buf.end(),
                       buf.begin(), tolower);
        tree->Search(buf);
    }
    std::chrono::steady_clock::time_point end_of_finding = std::chrono::steady_clock::now();
    unsigned finding_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_finding - begin_of_finding).count();

    std::chrono::steady_clock::time_point begin_of_deleting = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i)
    {
        std::cin >> buf;
        std::transform(buf.begin(), buf.end(),
                       buf.begin(), tolower);
        tree->Delete(buf);
    }
    std::chrono::steady_clock::time_point end_of_deleting = std::chrono::steady_clock::now();
    unsigned deleting_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_of_deleting - begin_of_deleting).count();

    std::cout << "Pushed " << amount << " elements in " << addition_time << "ms\n";
    std::cout << "Found " << amount << " elements in " << finding_time << "ms\n";
    std::cout << "Deleted " << amount << " elements in " << deleting_time << "ms\n";
    return 0;
}