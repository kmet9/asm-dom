#include "VNode.hpp"
#include <emscripten/bind.h>

std::vector<VNode> VNode::get_children() const {
  std::vector<VNode> vnodes;
  for(std::vector<VNode*>::size_type i = 0; i < children.size(); i++) {
      vnodes.push_back(*children[i]);
  }
  return vnodes;
}

// readonly
void VNode::set_children(std::vector<VNode> nodeChildren) {}

EMSCRIPTEN_BINDINGS(vnode) {
  emscripten::value_object<VNode>("VNode")
    .field("sel", &VNode::sel)
    .field("key", &VNode::key)
    .field("text", &VNode::text)
    .field("data", &VNode::data)
    .field("elm", &VNode::elm)
    .field("children", &VNode::get_children, &VNode::set_children);
  
  emscripten::register_vector<VNode>("VNodeVector");
  emscripten::register_vector<std::size_t>("VNodePtrVector");
}
