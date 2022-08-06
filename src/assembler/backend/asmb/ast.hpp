#pragma once
#include "expr.hpp"

namespace asmb {
    
    struct node {

    };

    std::vector<node> get_tree_from_expressions(std::vector<expr> exprs);
};