// IntensitySegments.test.mjs
import { IntensitySegments } from './IntensitySegments.mjs';

function runTests() {
    let segments = new IntensitySegments();
    console.assert(segments.toString() === "[]", "Test 1 Failed: Initial state should be empty");

    segments.add(10, 30, 1);
    console.assert(segments.toString() === "[[10,1],[30,0]]", "Test 2 Failed: Add [10,30) with amount 1");

    segments.add(20, 40, 1);
    console.assert(segments.toString() === "[[10,1],[20,2],[30,1],[40,0]]", "Test 3 Failed: Add [20,40) with amount 1");

    segments.add(10, 40, -1);
    console.assert(segments.toString() === "[[20,1],[30,0]]", "Test 4 Failed: Add [10,40) with amount -1");

    segments.add(10, 40, -1);
    console.assert(segments.toString() === "[[10,-1],[20,0],[30,-1],[40,0]]", "Test 5 Failed: Add [10,40) with amount -1");

    let segments1 = new IntensitySegments();
    segments1.add(10, 30, 1);
    segments1.add(20, 40, 1);
    segments1.add(10, 40, -2);
    console.assert(segments1.toString() === "[[10,-1],[20,0],[30,-1],[40,0]]", "Test 6 Failed: Complex add operations");

    segments1.set(10, 40, 1);
    console.assert(segments1.toString() === "[[10,1],[40,0]]", "Test 7 Failed: Set [10,40) with amount 1");

    let segments2 = new IntensitySegments();
    segments2.add(10, 30, 1);
    segments2.add(20, 40, 1);
    segments2.add(10, 40, -1);
    segments2.add(10, 40, -1);
    segments2.set(20, 30, 1);
    console.assert(segments2.toString() === "[[10,-1],[20,1],[30,-1],[40,0]]", "Test 8 Failed: Set [20,30) with amount 1");

    segments2.set(0, 100, 0);
    console.assert(segments2.toString() === "[]", "Test 9 Failed: Set [0,100) with amount 0");

    let segments3 = new IntensitySegments();
    segments3.set(0, 100, 0);
    segments3.add(0, 100, 0);
    console.assert(segments3.toString() === "[]", "Test 10 Failed: Edge case with zero intensity");

    console.log("All tests passed!");
}

// Run the unit tests
runTests();