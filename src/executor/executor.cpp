#include <functional>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Transforms/Utils/Cloning.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/JITMemoryManager.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#pragma GCC diagnostic pop

#include <dlambda/compiler/compiler.hpp>
#include <dlambda/executor/executor.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace executor {
    module::module(
      const compiler::module &source, const std::string &triple
    ) : context( source.get_llvm_context() ) {
      std::string error;
      const llvm::Target * const target = llvm::TargetRegistry::lookupTarget( triple, error );
      if( !error.empty() ) {
        std::cerr << error << std::endl;
        throw exceptions::unsupported_target_machine();
      }
      llvm::TargetOptions target_opts;
      target_opts.UseInitArray = 0;
      target_opts.UseSoftFloat = 0;
      target_opts.FloatABIType = llvm::FloatABI::Hard;
      const std::shared_ptr< llvm::TargetMachine > target_machine( target->createTargetMachine( triple, llvm::sys::getHostCPUName(), "", target_opts ) );
      if( !target_machine )
        throw exceptions::unsupported_target_machine();
      const llvm::DataLayout *layout = target_machine->getDataLayout();
      const std::shared_ptr< llvm::LLVMContext > &context_ = context;
      std::shared_ptr< std::string > error_message( new std::string );
      llvm_module.reset(
        llvm::CloneModule( source.get_llvm_module().get() ),
        [context_]( llvm::Module * ) {}
      );
      llvm_module->setTargetTriple( triple );
      llvm_module->setDataLayout( layout->getStringRepresentation() );
      if( !llvm_module )
        throw exceptions::invalid_module();
      std::shared_ptr< llvm::EngineBuilder > builder(
        new llvm::EngineBuilder( llvm_module.get() ),
        std::bind( &module::deleteBuilder, std::placeholders::_1, engine, error_message, llvm_module, context )
      );
      if( !builder )
        throw exceptions::internal_compiler_error();
      builder->setJITMemoryManager(
        llvm::JITMemoryManager::CreateDefaultMemManager()
      );
      builder->setUseMCJIT( true );
      builder->setErrorStr( error_message.get() );
      builder->setEngineKind( llvm::EngineKind::JIT );
      builder->setOptLevel( llvm::CodeGenOpt::Aggressive );
      builder->setRelocationModel( llvm::Reloc::Default );
      builder->setCodeModel( llvm::CodeModel::JITDefault );
      std::shared_ptr< llvm::RTDyldMemoryManager > mm(
        new llvm::SectionMemoryManager(),
        [builder]( llvm::SectionMemoryManager* ){}
      );
      builder->setMCJITMemoryManager( mm.get() );
      engine.reset(
        builder->create(),
        []( llvm::ExecutionEngine* ) {}
      );
      if( !engine )
        throw exceptions::internal_compiler_error();
      engine->RegisterJITEventListener( llvm::JITEventListener::createOProfileJITEventListener() );
//      engine->DisableLazyCompilation( true );
      if( !error_message->empty() ) {
        std::cerr << *error_message << std::endl;
        throw exceptions::llvm_compilation_failed();
      }
      engine->finalizeObject();
      engine->runStaticConstructorsDestructors( false );
    }
    void module::deleteBuilder(
      llvm::EngineBuilder *builder,
      std::shared_ptr< llvm::ExecutionEngine > engine,
      std::shared_ptr< std::string >,
      std::shared_ptr< llvm::Module >,
      std::shared_ptr< llvm::LLVMContext >
    ) {
      if( engine )
        engine->runStaticConstructorsDestructors(true);
      delete builder;
    }
  }
}

