# AXIS-ALIGNED ENCLOSURE OPTIMIZATION PROJECT- CODING CLUB(IITG)

Optimal heuristic-based solver for enclosing at least **K zones/buildings** with an **axis-aligned rectangle** of **minimum total cost** (penalty + perimeter). Designed for large-scale zoning optimization in grid-based environments.

---

## 🔧 Problem Overview

You're given `N` zones, each with `(x, y)` coordinates and a `penalty` value. Your task is to **select at least `K` zones** and enclose them in a **minimal perimeter rectangle**, such that the total cost (sum of all selected penalties + perimeter) is minimized.

---

## 🚀 Solution Strategy

Implemented several fast heuristics:
- **Negative penalty greedy**: Select all zones with negative penalties and pad with the cheapest positive ones.
- **Cost-based greedy**: Start with the `K` lowest penalty zones and explore incremental improvements.
- **Pure cost**: Try strictly the cheapest `K` zones.

These strategies run in **O(N log N)** time and are well-suited for large datasets.

---

## 🛠️ How to Use

### 1. File Structure


