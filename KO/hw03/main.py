#!/usr/bin/env python3
import itertools
import math
import sys
from collections import deque

from typing import Dict, List, Optional, Tuple


class Edge:
    def __init__(
            self,
            u: int,
            v: int,
            cost: float = 0,
            lower_bound: float = 0,
            upper_bound: float = 1,
            flow: float = 0,
            residual_flow: float = 0,
            pair: 'Edge' = None
    ):
        self.u = u
        self.v = v
        self.cost = cost
        self.lower_bound = lower_bound
        self.upper_bound = upper_bound
        self.flow = flow
        self.residual_flow = residual_flow
        self.pair: Edge = pair

    def augment(self, val: float):
        self.residual_flow -= val
        self.flow += val


class Graph:
    def __init__(self, num_vertices: int):
        self.num_vertices = num_vertices
        self.adjacency: List[List[Edge]] = [[] for _ in range(num_vertices)]

    def add_edge(self, *edges: Edge):
        for edge in edges:
            self.adjacency[edge.u].append(edge)

    def __copy__(self):
        g = Graph(self.num_vertices)
        g.adjacency = [node_edges.copy() for node_edges in self.adjacency]
        return g

    def min_flow(self, balances: Dict[int, int]):
        if self.max_flow_demands(balances):
            return self.cycle_canceling()
        else:
            raise ValueError('wtf')

    @staticmethod
    def paired_edges(u, v, ub) -> Tuple[Edge, Edge]:
        e1 = Edge(u, v, lower_bound=0, upper_bound=ub, residual_flow=ub)
        e2 = Edge(v, u, lower_bound=0, upper_bound=ub, residual_flow=0)
        e1.pair, e2.pair = e2, e1
        return e1, e2

    def max_flow_demands(self, balances: Dict[int, int]):
        g = self.__copy__()
        g.adjacency.extend([[], []])
        new_s = self.num_vertices
        new_t = self.num_vertices + 1

        for v, balance in balances.items():
            if balance > 0:
                e1, e2 = self.paired_edges(new_s, v, balance)
                g.add_edge(e1, e2)
            elif balance < 0:
                e1, e2 = self.paired_edges(v, new_t, -balance)
                g.add_edge(e1, e2)

        g.edmonds_karp(new_s, new_t)
        return all(e.flow >= e.upper_bound for e in g.adjacency[new_s])

    def bfs(self, s: int, t: int) -> Dict[int, Edge]:
        parents = {}
        queue = deque([s])
        while queue:
            current_vertex = queue.popleft()

            if current_vertex == t:
                break

            for edge in self.adjacency[current_vertex]:
                if edge.v not in parents and edge.residual_flow > 0:
                    parents[edge.v] = edge
                    queue.append(edge.v)

        return parents

    def edmonds_karp(self, s: int, t: int):
        flow_value = 0

        while True:
            parents = self.bfs(s, t)
            if t not in parents:
                break
            flow_value += self.calculate_augmentation(parents, s, t)

        return flow_value

    @staticmethod
    def calculate_augmentation(parents: Dict[int, Edge], s: int, t: int):
        augmentation_value = math.inf

        current_vertex = t
        while current_vertex != s:
            edge = parents[current_vertex]
            augmentation_value = min(augmentation_value, edge.residual_flow)
            current_vertex = edge.u

        current_vertex = t
        while current_vertex != s:
            edge = parents[current_vertex]
            pair_edge = edge.pair

            edge.augment(augmentation_value)
            pair_edge.augment(-augmentation_value)

            current_vertex = edge.u

        return augmentation_value

    def cycle_canceling(self):
        while True:
            pred, cycle_vertex = self.find_negative_cycle()
            if cycle_vertex is None:
                break

            self.cancel_cycle(pred, cycle_vertex)

    def find_negative_cycle(self) -> Tuple[Dict[int, Edge], Optional[int]]:
        best_cost = {u: 0 for u in range(self.num_vertices)}
        parents: Dict[int, Edge] = {v: None for v in range(self.num_vertices)}
        last_relaxed = None

        for i in range(self.num_vertices):
            last_relaxed = None

            for edge in filter(lambda e: e.residual_flow > 0, itertools.chain.from_iterable(self.adjacency)):
                if best_cost[edge.v] > best_cost[edge.u] + edge.cost:
                    best_cost[edge.v] = best_cost[edge.u] + edge.cost
                    parents[edge.v] = edge
                    last_relaxed = edge.v

        if last_relaxed is None:
            return parents, None
        else:
            for i in range(self.num_vertices):
                last_relaxed = parents[last_relaxed].u
            return parents, last_relaxed

    @staticmethod
    def cancel_cycle(pred: Dict[int, Edge], cycle_vertex: int):
        augmentation = math.inf

        current_vertex = cycle_vertex
        while True:
            e = pred[current_vertex]
            augmentation = min(augmentation, e.residual_flow)
            current_vertex = e.u

            if current_vertex == cycle_vertex:
                break

        current_vertex = cycle_vertex
        while True:
            edge = pred[current_vertex]
            pair_edge = edge.pair

            edge.augment(augmentation)
            pair_edge.augment(-augmentation)

            current_vertex = edge.u

            if current_vertex == cycle_vertex:
                break


def main(input_path, output_path):
    with open(input_path, mode='r+', encoding='utf8') as f_in:
        n, p = map(int, f_in.readline().split())
        graphs = []

        prev_positions = list(map(int, f_in.readline().split()))
        for i, line in enumerate(f_in):
            curr_positions = list(map(int, line.split()))
            graph = Graph(2 * n)
            for u, (ux, uy) in enumerate(zip(prev_positions[::2], prev_positions[1::2])):
                for v, (vx, vy) in enumerate(zip(curr_positions[::2], curr_positions[1::2])):
                    dist = math.sqrt(pow(ux - vx, 2) + pow(uy - vy, 2))
                    e1 = Edge(u, v + n, cost=dist, residual_flow=1)
                    e2 = Edge(v + n, u, cost=-dist, residual_flow=0)
                    e1.pair, e2.pair = e2, e1
                    graph.add_edge(e1, e2)
            graphs.append(graph)
            prev_positions = curr_positions

    with open(output_path, mode='w+', encoding='utf8') as f_out:
        b = {u: 1 if u < n else -1 for u in range(2 * n)}
        for graph in graphs:
            graph.min_flow(b)
            positions = []
            for u in range(n):
                edge = next(edge for edge in graph.adjacency[u] if edge.flow > 0 and edge.v >= n)
                positions.append(edge.v - n + 1)
            f_out.write(f"{' '.join(map(str, positions))}\n")


if __name__ == "__main__":
    assert len(sys.argv) >= 3
    main(sys.argv[1], sys.argv[2])
