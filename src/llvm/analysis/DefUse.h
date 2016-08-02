#ifndef _LLVM_DEF_USE_ANALYSIS_H_
#define _LLVM_DEF_USE_ANALYSIS_H_

#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/DataLayout.h>

#include "analysis/DataFlowAnalysis.h"
#include "ReachingDefinitions.h"

using dg::analysis::rd::LLVMReachingDefinitions;

namespace llvm {
    class DataLayout;
    class ConstantExpr;
};

namespace dg {

class LLVMDependenceGraph;
class LLVMNode;

class LLVMDefUseAnalysis : public analysis::DataFlowAnalysis<LLVMNode>
{
    LLVMDependenceGraph *dg;
    LLVMReachingDefinitions *RD;
    LLVMPointerAnalysis *PTA;
    const llvm::DataLayout *DL;
public:
    LLVMDefUseAnalysis(LLVMDependenceGraph *dg,
                       LLVMReachingDefinitions *rd,
                       LLVMPointerAnalysis *pta);
    ~LLVMDefUseAnalysis() { delete DL; }

    /* virtual */
    bool runOnNode(LLVMNode *node, LLVMNode *prev);
private:
    void addDataDependence(LLVMNode *node,
                           analysis::pta::PSNode *pts,
                           analysis::rd::RDNode *mem,
                           uint64_t size);

    void addDataDependence(LLVMNode *node,
                           const llvm::Value *where, /* in CFG */
                           const llvm::Value *ptrOp,
                           uint64_t size);

    void handleLoadInst(llvm::LoadInst *, LLVMNode *);
    void handleCallInst(LLVMNode *);
    void handleInlineAsm(LLVMNode *callNode);
    void handleIntrinsicCall(LLVMNode *callNode, llvm::CallInst *CI);
};

} // namespace dg

#endif //  _LLVM_DEF_USE_ANALYSIS_H_
