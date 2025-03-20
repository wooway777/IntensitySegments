#include <iostream>
#include <sstream>

using namespace std;

class IntensitySegments
{
private:
    struct Node
    {
        int position = 0;
        int intensity = 0;
        Node *next = nullptr;
        Node(int position, int intensity) : position(position), intensity(intensity) {}
    };
    
    Node *head = nullptr;

    void DeleteNode(Node *pre, Node *target)
    {
        if (pre) pre->next = target->next;
        else head = target->next;
        delete target;
    }

public:
    /**
     * Adds a specified intensity amount to all segments between `from` and `to`.
     * If the intensity amount is zero, no changes are made.
     * @param {int} from - The start of the range (inclusive).
     * @param {int} to - The end of the range (exclusive).
     * @param {int} amount - The intensity value to add.
     */
    void add(int from, int to, int amount)
    {
        if (amount == 0)
            return;
        if (from >= to)
            return;
        // find the first segment that is greater than or equal to from
        Node *cur = head;
        Node *pre = nullptr;
        while (cur)
        {
            if (cur->position < from)
            {
                pre = cur;
                cur = cur->next;
            }
            else break;
        }
        if (cur) // cur >= from
        {
            if (cur->position == from)
            {
                int preIntensity = pre ? pre->intensity : 0;
                Node *next = cur->next;
                if (cur->intensity + amount == preIntensity)
                {
                    DeleteNode(pre, cur);
                }
                else
                {
                    cur->intensity += amount;
                    pre = cur;
                }
                cur = next;
            }
            else // cur > from
            {
                if (pre)
                {
                    Node *node = new Node(from, amount + pre->intensity);
                    pre->next = node;
                    node->next = cur;
                    pre = node;
                }
                else
                {
                    head = new Node(from, amount); // new head
                    head->next = cur;
                    pre = head;
                }
            }
            // update all segments that are between from and to
            while (cur)
            {
                if (cur->position < to)
                {
                    cur->intensity += amount;
                    pre = cur;
                    cur = cur->next;
                }
                else break;
            }
            // cur >= to; deal with the last segment
            if (cur)
            {
                int preIntensity = pre ? pre->intensity : 0;
                if (cur->position == to)
                {
                    if (preIntensity == cur->intensity)
                        DeleteNode(pre, cur);
                    // no need to update cur otherwise
                }
                else // cur > to
                {
                    int next_intensity = cur->next ? cur->next->intensity : 0;
                    if (preIntensity + amount != next_intensity)
                    {
                        Node *node = new Node(to, pre ? pre->intensity : 0);
                        node->next = cur;
                        if (pre)
                            pre->next = node;
                        else
                            head = node;
                    }
                    // no need to insert new node otherwise
                }
            }
            else // new_end
            {
                Node *node = new Node(to, 0);
                pre->next = node;
            }
        }
        else // from is the largest
        {
            if (amount != 0)
            {
                Node *node = new Node(from, amount);
                Node *last = new Node(to, 0);
                if (pre) pre->next = node;
                else head = node;
                node->next = last;
            }
        }
    }

    /**
     * Sets the intensity of all segments between `from` and `to` to a specified amount.
     * If the intensity amount is zero, segments may be removed if they match the surrounding intensity.
     * @param {int} from - The start of the range (inclusive).
     * @param {int} to - The end of the range (exclusive).
     * @param {int} amount - The intensity value to set.
     */
    void set(int from, int to, int amount)
    {
        if (from >= to)
            return;
        // find the first segment that is greater than or equal to from
        Node *cur = head;
        Node *pre = nullptr;
        while (cur)
        {
            if (cur->position < from)
            {
                pre = cur;
                cur = cur->next;
            }
            else
                break;
        }
        int preIntensity = pre ? pre->intensity : 0;
        if (cur) // cur >= from
        {
            if (cur->position == from)
            {
                Node *next = cur->next;
                if (amount == preIntensity)
                {
                    DeleteNode(pre, cur);
                }
                else
                {
                    preIntensity = cur->intensity;
                    cur->intensity = amount;
                    pre = cur;
                }
                cur = next;
            }
            else // cur > from
            {
                if (preIntensity != amount)
                {
                    if (pre)
                    {
                        Node *node = new Node(from, amount);
                        pre->next = node;
                        node->next = cur;
                        pre = node;
                    }
                    else
                    {
                        head = new Node(from, amount); // new head
                        head->next = cur;
                        pre = head;
                    }
                }
                // no need to insert new node otherwise
            }
            // update all segments that are between from and to
            while (cur)
            {
                if (cur->position < to)
                {
                    Node *next = cur->next;
                    DeleteNode(pre, cur);
                    cur = next;
                    preIntensity = cur ? cur->intensity : 0;
                }
                else
                    break;
            }
            // cur >= to; deal with the last segment
            if (cur)
            {
                if (cur->position == to)
                {
                    if (amount == cur->intensity)
                        DeleteNode(pre, cur);
                    // no need to update cur otherwise
                }
                else // cur > to
                {
                    if (preIntensity != amount)
                    {
                        Node *node = new Node(to, preIntensity);
                        node->next = cur;
                        if (pre)
                            pre->next = node;
                        else
                            head = node;
                    }
                    // no need to insert new node otherwise
                }
            }
            else // new_end
            {
                if (amount != 0)
                {
                    Node *node = new Node(to, 0);
                    pre->next = node;
                }
            }
        }
        else // from is the largest
        {
            if (amount != 0)
            {
                Node *node = new Node(from, amount);
                Node *last = new Node(to, 0);
                node->next = last;
                if (pre)
                    pre->next = node;
                else
                    head = node;
            }
        }
    }

    /**
     * Converts the current state of the intensity segments to a string representation.
     * @return {std::string} - A string representation of the intensity segments.
     */
    std::string toString()
    {
        std::stringstream ss;
        ss << "[";
        Node *node = head;
        while (node)
        {
            ss << "[" << node->position << "," << node->intensity << "]";
            if (node->next) ss << ",";
            node = node->next;
        }
        ss << "]";
        return ss.str();
    }
};

int main() {
    // Test cases to demonstrate functionality
    IntensitySegments segments;
    cout << "==================" << endl;
    cout << "Actual re: " << segments.toString() << endl; // Should be "[]"
    cout << "Should be: " << "[]" << endl;
    cout << "==================" << endl;
    segments.add(10, 30, 1);
    cout << "Actual re: " << segments.toString() << endl; // Should be: "[[10,1],[30,0]]"
    cout << "Should be: " << "[[10,1],[30,0]]" << endl;
    cout << "==================" << endl;
    segments.add(20, 40, 1);
    cout << "Actual re: " << segments.toString() << endl; // Should be: "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "Should be: " << "[[10,1],[20,2],[30,1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments.add(10, 40, -1);
    cout << "Actual re: " << segments.toString() << endl; // Should be "[[20,1],[30,0]]"
    cout << "Should be: " << "[[20,1],[30,0]]" << endl;
    cout << "==================" << endl;
    segments.add(10, 40, -1);
    cout << "Actual re: " << segments.toString() << endl; // Should be "[[10,-1],[20,0],[30,-1],[40,0]]"
    cout << "Should be: " << "[[10,-1],[20,0],[30,-1],[40,0]]" << endl;

    IntensitySegments segments1;
    cout << "==================" << endl;
    cout << "Actual re: " << segments1.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;
    cout << "==================" << endl;
    segments1.add(10, 30, 1);
    cout << "Actual re: " << segments1.toString() << endl; // "[[10,1],[30,0]]"
    cout << "Should be: " << "[[10,1],[30,0]]" << endl;
    cout << "==================" << endl;
    segments1.add(20, 40, 1);
    cout << "Actual re: " << segments1.toString() << endl; // "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "Should be: " << "[[10,1],[20,2],[30,1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments1.add(10, 40, -2);
    cout << "Actual re: " << segments1.toString() << endl; // "[[10,-1],[20,0],[30,-1],[40,0]]"
    cout << "Should be: " << "[[10,-1],[20,0],[30,-1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments1.set(10, 40, 1);
    cout << "Actual re: " << segments1.toString() << endl; // "[[10,1],[40,0]]"
    cout << "Should be: " << "[[10,1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments1.set(10, 40, 1);
    cout << "Actual re: " << segments1.toString() << endl; // "[[10,1],[40,0]]"
    cout << "Should be: " << "[[10,1],[40,0]]" << endl;

    IntensitySegments segments2;
    cout << "==================" << endl;
    cout << "Actual re: " << segments2.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;
    cout << "==================" << endl;
    segments2.add(10, 30, 1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[10,1],[30,0]]"
    cout << "Should be: " << "[[10,1],[30,0]]" << endl;
    cout << "==================" << endl;
    segments2.add(20, 40, 1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "Should be: " << "[[10,1],[20,2],[30,1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments2.add(10, 40, -1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[20,1],[30,0]]"
    cout << "Should be: " << "[[20,1],[30,0]]" << endl;
    cout << "==================" << endl;
    segments2.add(10, 40, -1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[10,-1],[20,0],[30,-1],[40,0]]"
    cout << "Should be: " << "[[10,-1],[20,0],[30,-1],[40,0]]" << endl;

    cout << "==================" << endl;
    segments2.set(20, 30, 1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[10,-1],[20,1],[30,-1],[40,0]]"
    cout << "Should be: " << "[[10,-1],[20,1],[30,-1],[40,0]]" << endl;
    cout << "==================" << endl;
    segments2.set(20, 30, 1);
    cout << "Actual re: " << segments2.toString() << endl; // "[[10,-1],[20,1],[30,-1],[40,0]]"
    cout << "Should be: " << "[[10,-1],[20,1],[30,-1],[40,0]]" << endl;

    cout << "==================" << endl;
    segments2.set(0, 100, 0);
    cout << "Actual re: " << segments2.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;

    cout << "1111111111" << endl;
    IntensitySegments segments3;
    cout << "Actual re: " << segments3.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;
    segments3.set(0, 100, 0);
    cout << "Actual re: " << segments3.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;
    segments3.add(0, 100, 0);
    cout << "Actual re: " << segments3.toString() << endl; // "[]"
    cout << "Should be: " << "[]" << endl;

    cin.get();
    return 0;
}