# Game of Life (Serial Optimisations)

This project uses the Game of Life as a test case for high-performance computing. Large grids require billions of neighbour checks, placing heavy pressure on CPU execution and memory access. The focus of this milestone is serial optimisation, exploring how much performance can be gained without using parallelism.

## Baseline Implementation
The baseline implementation follows a simple approach:
- The grid is initialised to match the window size
- Cells are randomly populated using a fixed seed
- Each generation is processed left-to-right, top-to-bottom
- For each cell, up to 8 neighbours are checked
- Game of Life rules are applied
- Results are written to a separate "next board"
- Boards are swapped after each generation
- The simulation runs for 1000 iterations
- Memory is cleaned up at the end

## Benchmarking Setup
All benchmarks use a fixed configuration to ensure fairness and repeatability.
- Fixed random seed
- Cell initialisation to 50% dead or alive
- 1000 iterations
- Fixed grid size per experiment

## Profiling results
With approximately 368 million neighour-count calls per run, neighbour computation was the dominant hotspot. All major optimisations therefore targeted this section. 

## Optimisation Techniques
The following techniques were applied incrementally:
- Loop unrolling: reduced loop overhead with explicit neighbour checks
- Board padding: added dead-cell borders to remove boundary checks, significantly reducing branching
- Inline neighbour counting: eliminated function call overhead
- Pre-computed indices: avoided repeated index arithmetic inside hot loops
- Cache-friendly traversal: reordered memory access for improved cache locality

Early-exit neighbour counting was also tested but showed no benefit due to branching overhead.

The most impactful improvements came from board padding and cache-friendly traversal.

## Rendering 
SDL is used to render live cells as filled rectangles. Due to high simulation speed, some oscillating patterns appear visually stable. 

## Limitations
- Serial-only implementation
- Reduced code readability from low-level optimisations

## Future work
- Bit-packing cell states
- Pattern detection
- Active cell tracking
- Parallel and GPU-based implementations

## Conclusion
This project demonstrates how systematic profiling and memory-aware optimisations can produce large performance gains in compute-intensive simulations. Through targeted improvements, execution time was reduced from 17.04 to 4.03s without parallelism. 
