export class IntensitySegments {
    constructor() {
        this.head = null;
    }

    // Node class
    Node = class {
        constructor(position, intensity) {
            this.position = position;
            this.intensity = intensity;
            this.next = null;
        }
    };

    // Helper function to delete a node
    deleteNode(pre, target) {
        if (pre) pre.next = target.next;
        else this.head = target.next;
    }

    /**
     * Adds a specified intensity amount to all segments between `from` and `to`.
     * If the intensity amount is zero, no changes are made.
     * @param {number} from - The start of the range (inclusive).
     * @param {number} to - The end of the range (exclusive).
     * @param {number} amount - The intensity value to add.
     */
    add(from, to, amount) {
        if (amount === 0 || from >= to) return;

        let cur = this.head;
        let pre = null;

        // Find the first segment that is greater than or equal to `from`
        while (cur && cur.position < from) {
            pre = cur;
            cur = cur.next;
        }

        if (cur) { // cur >= from
            if (cur.position === from) {
                const preIntensity = pre ? pre.intensity : 0;
                const next = cur.next;
                if (cur.intensity + amount === preIntensity) {
                    this.deleteNode(pre, cur);
                } else {
                    cur.intensity += amount;
                    pre = cur;
                }
                cur = next;
            } else { // cur > from
                if (pre) {
                    const node = new this.Node(from, amount + pre.intensity);
                    pre.next = node;
                    node.next = cur;
                    pre = node;
                } else {
                    this.head = new this.Node(from, amount); // new head
                    this.head.next = cur;
                    pre = this.head;
                }
            }

            // Update all segments between `from` and `to`
            while (cur && cur.position < to) {
                cur.intensity += amount;
                pre = cur;
                cur = cur.next;
            }

            // cur >= to; deal with the last segment
            if (cur) {
                const preIntensity = pre ? pre.intensity : 0;
                if (cur.position === to) {
                    if (preIntensity === cur.intensity) {
                        this.deleteNode(pre, cur);
                    }
                } else { // cur > to
                    const nextIntensity = cur.next ? cur.next.intensity : 0;
                    if (preIntensity + amount !== nextIntensity) {
                        const node = new this.Node(to, pre ? pre.intensity : 0);
                        node.next = cur;
                        if (pre) pre.next = node;
                        else this.head = node;
                    }
                }
            } else { // new end
                const node = new this.Node(to, 0);
                pre.next = node;
            }
        } else { // `from` is the largest
            if (amount !== 0) {
                const node = new this.Node(from, amount);
                const last = new this.Node(to, 0);
                if (pre) pre.next = node;
                else this.head = node;
                node.next = last;
            }
        }
    }

    /**
     * Sets the intensity of all segments between `from` and `to` to a specified amount.
     * If the intensity amount is zero, segments may be removed if they match the surrounding intensity.
     * @param {number} from - The start of the range (inclusive).
     * @param {number} to - The end of the range (exclusive).
     * @param {number} amount - The intensity value to set.
     */
    set(from, to, amount) {
        if (from >= to) return;

        let cur = this.head;
        let pre = null;

        // Find the first segment that is greater than or equal to `from`
        while (cur && cur.position < from) {
            pre = cur;
            cur = cur.next;
        }

        const preIntensity = pre ? pre.intensity : 0;

        if (cur) { // cur >= from
            if (cur.position === from) {
                const next = cur.next;
                if (amount === preIntensity) {
                    this.deleteNode(pre, cur);
                } else {
                    cur.intensity = amount;
                    pre = cur;
                }
                cur = next;
            } else { // cur > from
                if (preIntensity !== amount) {
                    if (pre) {
                        const node = new this.Node(from, amount);
                        pre.next = node;
                        node.next = cur;
                        pre = node;
                    } else {
                        this.head = new this.Node(from, amount); // new head
                        this.head.next = cur;
                        pre = this.head;
                    }
                }
            }

            // Update all segments between `from` and `to`
            while (cur && cur.position < to) {
                const next = cur.next;
                this.deleteNode(pre, cur);
                cur = next;
            }

            // cur >= to; deal with the last segment
            if (cur) {
                if (cur.position === to) {
                    if (amount === cur.intensity) {
                        this.deleteNode(pre, cur);
                    }
                } else { // cur > to
                    if (preIntensity !== amount) {
                        const node = new this.Node(to, preIntensity);
                        node.next = cur;
                        if (pre) pre.next = node;
                        else this.head = node;
                    }
                }
            } else { // new end
                if (amount !== 0) {
                    const node = new this.Node(to, 0);
                    pre.next = node;
                }
            }
        } else { // `from` is the largest
            if (amount !== 0) {
                const node = new this.Node(from, amount);
                const last = new this.Node(to, 0);
                node.next = last;
                if (pre) pre.next = node;
                else this.head = node;
            }
        }
    }

    /**
     * Converts the current state of the intensity segments to a string representation.
     * @return {string} - A string representation of the intensity segments.
     */
    toString() {
        let result = "[";
        let node = this.head;
        while (node) {
            result += `[${node.position},${node.intensity}]`;
            if (node.next) result += ",";
            node = node.next;
        }
        result += "]";
        return result;
    }
}