#include <iostream>
#include <map>
#include <sstream>
#include <string>

using namespace std;

class IntensitySegments
{
private:
    // Map to store intensity segments: key = position, value = intensity
    map<int, int> segment_intensity_;

    /**
     * Merges adjacent segments with the same intensity to simplify the segment map.
     * This ensures no two consecutive segments have the same intensity value.
     */
    void mergeSegments()
    {
        auto it = segment_intensity_.begin();
        int pre_amount = 0; // Track the intensity of the previous segment
        while (it != segment_intensity_.end())
        {
            if (it->second == pre_amount)
            {
                // If current segment has the same intensity as the previous, remove it
                it = segment_intensity_.erase(it);
            }
            else
            {
                // Update the previous intensity and move to the next segment
                pre_amount = it->second;
                ++it;
            }
        }
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
        if (amount == 0) // No change if amount is zero
            return;

        // If the map is empty, initialize with the given range
        if (segment_intensity_.empty())
        {
            segment_intensity_[from] = amount;
            segment_intensity_[to] = 0;
            return;
        }

        // Find the first segment that is greater than or equal to 'from'
        auto it = segment_intensity_.lower_bound(from);
        if (it->first == from)
        {
            // If 'from' is exactly a key, update its intensity
            it->second += amount;
        }
        else
        {
            if (it == segment_intensity_.begin())
            {
                // If 'from' is before the first segment, add a new segment
                segment_intensity_[from] = amount;
            }
            else
            {
                // Otherwise, add a new segment with intensity adjusted by the previous segment
                segment_intensity_[from] = amount + (--it)->second;
            }
        }

        // Update all segments between 'from' and 'to'
        it = segment_intensity_.find(from);
        ++it;
        while (it != segment_intensity_.end() && it->first < to)
        {
            it->second += amount;
            ++it;
        }

        // Handle the segment at 'to'
        if (it == segment_intensity_.end())
        {
            // If 'to' is beyond the last segment, add a new segment with intensity 0
            segment_intensity_[to] = 0;
        }
        else
        {
            if (it->first > to)
                // If 'to' is not a key, add a new segment with the intensity of the previous segment
                segment_intensity_[to] = prev(it)->second;
        }

        // Merge adjacent segments with the same intensity
        mergeSegments();
    }

    /**
     * Sets the intensity of all segments between `from` and `to` to a specified amount.
     * If the intensity amount is zero and the map is empty, no changes are made.
     * @param {int} from - The start of the range (inclusive).
     * @param {int} to - The end of the range (exclusive).
     * @param {int} amount - The intensity value to set.
     */
    void set(int from, int to, int amount)
    {
        // If the map is empty and amount is zero, do nothing
        if (segment_intensity_.empty())
        {
            if (amount == 0)
                return;
            // Initialize with the given range
            segment_intensity_[from] = amount;
            segment_intensity_[to] = 0;
            return;
        }

        // Set the intensity at 'from'
        segment_intensity_[from] = amount;

        // Update all segments between 'from' and 'to'
        auto it = segment_intensity_.find(from);
        ++it;
        int pre_amount = amount; // Track the intensity of the previous segment
        while (it != segment_intensity_.end() && it->first < to)
        {
            pre_amount = it->second;
            it = segment_intensity_.erase(it); // Remove segments within the range
        }

        // Handle the segment at 'to'
        if (it == segment_intensity_.end())
        {
            // If 'to' is beyond the last segment, add a new segment with intensity 0
            segment_intensity_[to] = 0;
        }
        else
        {
            if (it->first > to)
                // If 'to' is not a key, add a new segment with the intensity of the previous segment
                segment_intensity_[to] = pre_amount;
        }

        // Merge adjacent segments with the same intensity
        mergeSegments();
    }

    /**
     * Converts the intensity segments to a string representation.
     * The format is a list of segments, where each segment is represented as [position, intensity].
     * @return {string} - The string representation of the intensity segments.
     */
    string toString()
    {
        stringstream ss;
        ss << "[";
        for (auto it = segment_intensity_.begin(); it != segment_intensity_.end(); ++it)
        {
            if (it != segment_intensity_.begin()) {
                ss << ",";
            }
            ss << "[" << it->first << "," << it->second << "]";
        }
        ss << "]";

        string res = ss.str();
        cout << res << endl; // Print the result for debugging
        return res;
    }
};

int main()
{
    // Test cases to demonstrate functionality
    IntensitySegments segments;
    cout << "==================" << endl;
    segments.toString(); // Should be "[]"
    cout << "==================" << endl;
    segments.add(10, 30, 1);
    segments.toString(); // Should be: "[[10,1],[30,0]]"
    cout << "==================" << endl;
    segments.add(20, 40, 1);
    segments.toString(); // Should be: "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "==================" << endl;
    segments.add(10, 40, -1);
    segments.toString(); // Should be "[[20,1],[30,0]]"
    cout << "==================" << endl;
    segments.add(10, 40, -1);
    segments.toString(); // Should be "[[10,-1],[20,0],[30,-1],[40,0]]"

    IntensitySegments segments1;
    cout << "==================" << endl;
    segments1.toString(); // "[]"
    cout << "==================" << endl;
    segments1.add(10, 30, 1);
    segments1.toString(); // "[[10,1],[30,0]]"
    cout << "==================" << endl;
    segments1.add(20, 40, 1);
    segments1.toString(); // "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "==================" << endl;
    segments1.add(10, 40, -2);
    segments1.toString(); // "[[10,-1],[20,0],[30,-1],[40,0]]"
    cout << "==================" << endl;
    segments1.set(10, 40, 1);
    segments1.toString(); // "[[10,1],[40,0]]"
    cout << "==================" << endl;
    segments1.set(10, 40, 1);
    segments1.toString(); // "[[10,1],[40,0]]"

    IntensitySegments segments2;
    cout << "==================" << endl;
    segments2.toString(); // "[]"
    cout << "==================" << endl;
    segments2.add(10, 30, 1);
    segments2.toString(); // "[[10,1],[30,0]]"
    cout << "==================" << endl;
    segments2.add(20, 40, 1);
    segments2.toString(); // "[[10,1],[20,2],[30,1],[40,0]]"
    cout << "==================" << endl;
    segments2.add(10, 40, -1);
    segments2.toString(); // "[[20,1],[30,0]]"
    cout << "==================" << endl;
    segments2.add(10, 40, -1);
    segments2.toString(); // "[[10,-1],[20,0],[30,-1],[40,0]]"

    cout << "==================" << endl;
    segments2.set(20, 30, 1);
    segments2.toString(); // "[[10,-1],[20,1],[30,-1],[40,0]]"
    cout << "==================" << endl;
    segments2.set(20, 30, 1);
    segments2.toString(); // "[[10,-1],[20,1],[30,-1],[40,0]]"

    cout << "==================" << endl;
    segments2.set(0, 100, 0);
    segments2.toString(); // "[]"

    cout << "1111111111" << endl;
    IntensitySegments segments3;
    segments3.toString(); // "[]"
    segments3.set(0, 100, 0);
    segments3.toString(); // "[]"
    segments3.add(0, 100, 0);
    segments3.toString(); // "[]"

    cin.get();
    return 0;
}