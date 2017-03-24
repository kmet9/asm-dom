#include "h.hpp"
#include "../VNodeData/VNodeData.hpp"
#include "../VNode/VNode.hpp"
#include <emscripten/bind.h>
#include <string>

void addNS(VNode* vnode) {
	vnode->data.ns = std::string("http://www.w3.org/2000/svg");
	if (vnode->sel.compare("foreignObject") != 0 && !vnode->children.empty()) {
        for(std::vector<VNode>::size_type i = vnode->children.size(); i--;) {
            addNS(vnode->children[i]);
        }
	}
}

void adjustVNode(VNode* vnode) {
    vnode->key = vnode->data.key;
    if (
    vnode->sel.length() >= 3 && vnode->sel[0] == 's' && vnode->sel[1] == 'v' && vnode->sel[2] == 'g' &&
    (vnode->sel.length() == 3 || vnode->sel[3] == '.' || vnode->sel[3] == '#')
  ) {
        addNS(vnode);
    }
}

VNode* _h_s(std::string sel) {
    VNode* vnode = new VNode();
    vnode->sel = sel;
    adjustVNode(vnode);
    return vnode;
};

std::size_t h_s(std::string sel) {
    return reinterpret_cast<std::size_t>(_h_s(sel));
};

std::size_t h_ti(std::string text, bool isText) {
    VNode* vnode = new VNode();
    if (isText) {
        vnode->text = text;
    } else {
        vnode->sel = text;
    }
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_sn(std::string sel, std::size_t node) {
    VNode* vnode = _h_s(sel);
    vnode->children.push_back(reinterpret_cast<VNode*>(node));
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_st(std::string sel, std::string text) {
    VNode* vnode = _h_s(sel);
    vnode->text = text;
    return reinterpret_cast<std::size_t>(vnode);
};

VNode* _h_sd(std::string sel, VNodeData data) {
    VNode* vnode = _h_s(sel);
    vnode->data = data;
    return vnode;
};

std::size_t h_sd(std::string sel, VNodeData data) {
    return reinterpret_cast<std::size_t>(_h_sd(sel, data));
};

std::size_t h_sc(std::string sel, std::vector<std::size_t> children) {
    VNode* vnode = _h_s(sel);
    for(std::vector<std::size_t>::size_type i = 0; i < children.size(); i++) {
        vnode->children.push_back(reinterpret_cast<VNode*>(children[i]));
    }
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_sdn(std::string sel, VNodeData data, std::size_t node) {
    VNode* vnode = _h_sd(sel, data);
    vnode->children.push_back(reinterpret_cast<VNode*>(node));
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_sdt(std::string sel, VNodeData data, std::string text) {
    VNode* vnode = _h_sd(sel, data);
    vnode->text = text;
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_sdc(std::string sel, VNodeData data, std::vector<std::size_t> children) {
    VNode* vnode = _h_sd(sel, data);
    for(std::vector<std::size_t>::size_type i = 0; i < children.size(); i++) {
        vnode->children.push_back(reinterpret_cast<VNode*>(children[i]));
    }
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

std::size_t h_stdc(std::string sel, std::string text, VNodeData data, std::vector<std::size_t> children) {
    std::vector<VNode*> vnodes;
    for(std::vector<std::size_t>::size_type i = 0; i < children.size(); i++) {
        vnodes.push_back(reinterpret_cast<VNode*>(children[i]));
    }
    VNode* vnode = new VNode(sel, text, data, vnodes);
    adjustVNode(vnode);
    return reinterpret_cast<std::size_t>(vnode);
};

VNode getVNode(std::size_t vnode) {
	return *(reinterpret_cast<VNode*>(vnode));
}

EMSCRIPTEN_BINDINGS(h_function) {
    emscripten::function("_h_s", &h_s, emscripten::allow_raw_pointers());
	emscripten::function("_h_ti", &h_ti, emscripten::allow_raw_pointers());
	emscripten::function("_h_sn", &h_sn, emscripten::allow_raw_pointers());
	emscripten::function("_h_st", &h_st, emscripten::allow_raw_pointers());
	emscripten::function("_h_sd", &h_sd, emscripten::allow_raw_pointers());
	emscripten::function("_h_sc", &h_sc, emscripten::allow_raw_pointers());
	emscripten::function("_h_sdn", &h_sdn, emscripten::allow_raw_pointers());
	emscripten::function("_h_sdt", &h_sdt, emscripten::allow_raw_pointers());
	emscripten::function("_h_sdc", &h_sdc, emscripten::allow_raw_pointers());
	emscripten::function("_h_stdc", &h_stdc, emscripten::allow_raw_pointers());
    emscripten::function("_getVNode", &getVNode, emscripten::allow_raw_pointer<emscripten::arg<0>>());
}
