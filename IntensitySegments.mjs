export class IntensitySegments {
    /**
     * Initializes a new IntensitySegments instance with an empty segment map.
     */
    constructor() {
        this.segmentIntensity = {};
    }

    /**
     * Merges adjacent segments with the same intensity to simplify the segment map.
     * This ensures no two consecutive segments have the same intensity value.
     */
    mergeSegments() {
        let preAmount = 0; // Track the intensity of the previous segment
        for (let key in this.segmentIntensity) {
            if (this.segmentIntensity[key] === preAmount) {
                // If current segment has the same intensity as the previous, remove it
                delete this.segmentIntensity[key];
            } else {
                // Update the previous intensity and move to the next segment
                preAmount = this.segmentIntensity[key];
            }
        }
    }

    /**
     * Adds a specified intensity amount to all segments between `from` and `to`.
     * If the intensity amount is zero, no changes are made.
     * @param {number} from - The start of the range (inclusive).
     * @param {number} to - The end of the range (exclusive).
     * @param {number} amount - The intensity value to add.
     */
    add(from, to, amount) {
        if (amount === 0) return; // No change if amount is zero

        // If the map is empty, initialize with the given range
        if (Object.keys(this.segmentIntensity).length === 0) {
            this.segmentIntensity[from] = amount;
            this.segmentIntensity[to] = 0;
            return;
        }

        // Find the first segment that is greater than or equal to `from`
        let keys = Object.keys(this.segmentIntensity).map(Number).sort((a, b) => a - b);
        let it = keys.find(key => key >= from);

        if (it === from) {
            // If 'from' is exactly a key, update its intensity
            this.segmentIntensity[from] += amount;
        } else {
            if (it === undefined) {
                // If 'from' is beyond the last segment, add a new segment
                this.segmentIntensity[from] = amount;
            } else {
                // Otherwise, add a new segment with intensity adjusted by the previous segment
                let prevKey = keys[keys.indexOf(it) - 1];
                this.segmentIntensity[from] = amount + (prevKey !== undefined ? this.segmentIntensity[prevKey] : 0);
            }
        }

        // Update all segments between `from` and `to`
        keys = Object.keys(this.segmentIntensity).map(Number).sort((a, b) => a - b);
        let startIndex = keys.indexOf(from) + 1;
        for (let i = startIndex; i < keys.length && keys[i] < to; i++) {
            this.segmentIntensity[keys[i]] += amount;
        }

        // Handle the segment at `to`
        if (keys[keys.length - 1] < to) {
            // If 'to' is beyond the last segment, add a new segment with intensity 0
            this.segmentIntensity[to] = 0;
        } else {
            let lastKey = keys.find(key => key >= to);
            if (lastKey > to) {
                // If 'to' is not a key, add a new segment with the intensity of the previous segment
                this.segmentIntensity[to] = this.segmentIntensity[keys[keys.indexOf(lastKey) - 1]];
            }
        }

        // Merge adjacent segments with the same intensity
        this.mergeSegments();
    }

    /**
     * Sets the intensity of all segments between `from` and `to` to a specified amount.
     * If the intensity amount is zero and the map is empty, no changes are made.
     * @param {number} from - The start of the range (inclusive).
     * @param {number} to - The end of the range (exclusive).
     * @param {number} amount - The intensity value to set.
     */
    set(from, to, amount) {
        // If the map is empty and amount is zero, do nothing
        if (Object.keys(this.segmentIntensity).length === 0) {
            if (amount === 0) return;
            // Initialize with the given range
            this.segmentIntensity[from] = amount;
            this.segmentIntensity[to] = 0;
            return;
        }

        // Set the intensity at `from`
        this.segmentIntensity[from] = amount;

        // Update all segments between `from` and `to`
        let keys = Object.keys(this.segmentIntensity).map(Number).sort((a, b) => a - b);
        let startIndex = keys.indexOf(from) + 1;
        let preAmount = amount; // Track the intensity of the previous segment
        for (let i = startIndex; i < keys.length && keys[i] < to; i++) {
            preAmount = this.segmentIntensity[keys[i]];
            delete this.segmentIntensity[keys[i]]; // Remove segments within the range
        }

        // Handle the segment at `to`
        if (keys[keys.length - 1] < to) {
            // If 'to' is beyond the last segment, add a new segment with intensity 0
            this.segmentIntensity[to] = 0;
        } else {
            let lastKey = keys.find(key => key >= to);
            if (lastKey > to) {
                // If 'to' is not a key, add a new segment with the intensity of the previous segment
                this.segmentIntensity[to] = preAmount;
            }
        }

        // Merge adjacent segments with the same intensity
        this.mergeSegments();
    }

    /**
     * Converts the intensity segments to a string representation.
     * The format is a list of segments, where each segment is represented as [position, intensity].
     * @return {string} - The string representation of the intensity segments.
     */
    toString() {
        let result = "[";
        let keys = Object.keys(this.segmentIntensity).map(Number).sort((a, b) => a - b);
        for (let i = 0; i < keys.length; i++) {
            if (i > 0) result += ",";
            result += `[${keys[i]},${this.segmentIntensity[keys[i]]}]`;
        }
        result += "]";
        return result;
    }
}