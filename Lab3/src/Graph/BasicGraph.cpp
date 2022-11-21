#include "BasicGraph.h"

#include "RGBCode.h"

BasicGraph::BasicGraph(enum GraphType graph_type) : graph_type(graph_type) { return; }

BasicGraph::BasicGraph(enum GraphType graph_type, RGBCode rgbc, float edge_size)
    : rgbc(rgbc), edge_size(edge_size), graph_type(graph_type)
{
    return;
}

void BasicGraph::setEdgeRGBC(RGBCode rgbc)
{
    this->rgbc = rgbc;
    return;
}

// void BasicGraph::setFillRGBC(RGBCode rgbc) {
//     this->fill_rgbc = rgbc;
//     return;
// }

void BasicGraph::setEdgeSize(float size)
{
    this->edge_size = size;
    return;
}

// bool BasicGraph::isNULLObj() { return this->nullObj; }

// bool BasicGraph::setNULLObj(bool val/* = 1*/) {
//     this->nullObj = val;
//     return;
// }

enum GraphType BasicGraph::getGraphType() { return this->graph_type; }

RGBCode BasicGraph::getRGBCode() { return this->rgbc; }

float BasicGraph::getEdgeSize() { return this->edge_size; }