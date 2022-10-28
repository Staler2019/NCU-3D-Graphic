#include "./BasicGraph.h"

#include "./RGBCode.h"

BasicGraph::BasicGraph() { return; }

BasicGraph::BasicGraph(RGBCode edge_rgbc, float edge_size)
    : edge_rgbc(edge_rgbc), edge_size(edge_size) {
    return;
}
