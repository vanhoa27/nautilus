#pragma once
#include "nautilus/compiler/DumpHandler.hpp"
#include "nautilus/options.hpp"
namespace NES {
class DumpHelper;
namespace Nautilus {
class CompilationOptions;
}
} // namespace NES

#include <llvm/IR/Module.h>
#include <mlir/IR/BuiltinOps.h>
#include <mlir/Pass/Pass.h>
#include <vector>

namespace nautilus::compiler::mlir {

/**
 * @brief The LLVMIROptimizer takes a generated MLIR module,
 * and applies configured lowering & optimization passes to it.
 */
class LLVMIROptimizer {
public:
	LLVMIROptimizer();  // Disable default constructor
	~LLVMIROptimizer(); // Disable default destructor

	static std::function<llvm::Error(llvm::Module*)> getLLVMOptimizerPipeline(const engine::Options& options,
	                                                                          const DumpHandler& handler);
};
} // namespace nautilus::compiler::mlir
