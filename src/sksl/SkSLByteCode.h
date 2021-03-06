/*
 * Copyright 2019 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKSL_BYTECODE
#define SKSL_BYTECODE

#include "include/private/SkOnce.h"
#include "include/private/SkVx.h"
#include "src/sksl/SkSLString.h"
#include "src/sksl/ir/SkSLFunctionDeclaration.h"

#include <memory>
#include <vector>

namespace SkSL {

class ByteCode;
class ExternalValue;

class ByteCodeFunction {
public:
    // all counts are of 32-bit values, so a float4 counts as 4 parameter or return slots
    struct Parameter {
        int fSlotCount;
        bool fIsOutParameter;
    };

    /**
     * Note that this is the actual number of parameters, not the number of parameter slots.
     */
    int getParameterCount() const { return fParameters.size(); }

    Parameter getParameter(int idx) const { return fParameters[idx]; }

    int getParameterSlotCount() const { return fParameterSlotCount; }

    int getReturnSlotCount() const { return fReturnSlotCount; }

    void disassemble() const { }

private:
    ByteCodeFunction(const FunctionDeclaration* declaration)
        : fName(declaration->fName) {}

    String fName;

    std::vector<Parameter> fParameters;

    int fParameterSlotCount;

    int fReturnSlotCount;

    int fStackSlotCount;

    std::vector<uint8_t> fCode;

    friend class ByteCode;
    friend class ByteCodeGenerator;
    template<int width>
    friend class Interpreter;
};

enum class TypeCategory {
    kBool,
    kSigned,
    kUnsigned,
    kFloat,
};

class SK_API ByteCode {
public:
    ByteCode() = default;
    ByteCode(ByteCode&&) = default;
    ByteCode& operator =(ByteCode&&) = default;

    template<int width>
    union Vector {
        skvx::Vec<width, float> fFloat;
        skvx::Vec<width, int32_t> fInt;
        skvx::Vec<width, uint32_t> fUInt;

        Vector() = default;

        Vector(skvx::Vec<width, float> f)
            : fFloat(f) {}

        Vector(skvx::Vec<width, int32_t> i)
            : fInt(i) {}

        Vector(skvx::Vec<width, uint32_t> u)
            : fUInt(u) {}
    };

    enum class Instruction : uint8_t {
        // no parameters
        kNop,
        // no parameters
        kAbort,
        // Register target, Register src1, Register src2
        kAddF,
        // uint8_t count, Register target, Register src1, Register src2
        kAddFN,
        // Register target, Register src1, Register src2
        kAddI,
        // uint8_t count, Register target, Register src1, Register src2
        kAddIN,
        // Register target, Register src1, Register src2
        kAnd,
        // Register index, int arrayLength
        kBoundsCheck,
        // Pointer target
        kBranch,
        // Pointer target
        kBranchIfAllFalse,
        // no parameters
        kBreak,
        // Register target, uint8_t functionIndex, Register parameters
        kCall,
        // Register target, uint8_t externalValueIndex, uint8_t targetSize, Register arguments,
        // uint8_t argumentSize
        kCallExternal,
        // Register target, Register src1, Register src2
        kCompareEQF,
        // Register target, Register src1, Register src2
        kCompareEQI,
        // Register target, Register src1, Register src2
        kCompareNEQF,
        // Register target, Register src1, Register src2
        kCompareNEQI,
        // Register target, Register src1, Register src2
        kCompareGTF,
        // Register target, Register src1, Register src2
        kCompareGTS,
        // Register target, Register src1, Register src2
        kCompareGTU,
        // Register target, Register src1, Register src2
        kCompareGTEQF,
        // Register target, Register src1, Register src2
        kCompareGTEQS,
        // Register target, Register src1, Register src2
        kCompareGTEQU,
        // Register target, Register src1, Register src2
        kCompareLTF,
        // Register target, Register src1, Register src2
        kCompareLTS,
        // Register target, Register src1, Register src2
        kCompareLTU,
        // Register target, Register src1, Register src2
        kCompareLTEQF,
        // Register target, Register src1, Register src2
        kCompareLTEQS,
        // Register target, Register src1, Register src2
        kCompareLTEQU,
        // no parameters
        kContinue,
        // Register target, Register src
        kCopy,
        // Register target, Register src,
        kCos,
        // Register target, Register src1, Register src2
        kDivideF,
        // uint8_t count, Register target, Register src1, Register src2
        kDivideFN,
        // Register target, Register src1, Register src2
        kDivideS,
        // uint8_t count, Register target, Register src1, Register src2
        kDivideSN,
        // Register target, Register src1, Register src2
        kDivideU,
        // uint8_t count, Register target, Register src1, Register src2
        kDivideUN,
        // Register target, Register src
        kFloatToSigned,
        // Register target, Register src
        kFloatToUnsigned,
        // Load a constant into a register
        // Register target, Immediate value
        kImmediate,
        // Register target, Register src
        kInverse2x2,
        // Register target, Register src
        kInverse3x3,
        // Register target, Register src
        kInverse4x4,
        // Load the memory cell pointed to by srcPtr into a register
        // Register target, Register srcPtr
        kLoad,
        // uint8_t count, Register target, Register srcPtr
        kLoadN,
        // Load the memory cell pointed to by src into a register
        // Register target, Pointer src
        kLoadDirect,
        // uint8_t count, Register target, Pointer src
        kLoadDirectN,
        // Load the parameter slot pointed to by srcPtr into a register
        // Register target, Register srcPtr
        kLoadParameter,
        // uint8_t count, Register target, Register srcPtr
        kLoadParameterN,
        // Load the parameter slot pointed to by src into a register
        // Register target, Pointer src
        kLoadParameterDirect,
        // uint8_t count, Register target, Pointer src
        kLoadParameterDirectN,
        // Load the stack cell pointed to by srcPtr + sp into a register
        // Register target, Register srcPtr
        kLoadStack,
        // uint8_t count, Register target, Register srcPtr
        kLoadStackN,
        // Load the stack cell pointed to by src + sp into a register
        // Register target, Pointer src
        kLoadStackDirect,
        // uint8_t count, Register target, Pointer src
        kLoadStackDirectN,
        // Pushes a new loop onto the loop and continue stacks
        // no parameters
        kLoopBegin,
        // Pops the loop and continue stacks
        // no parameters
        kLoopEnd,
        // Register mask
        kLoopMask,
        // no parameters
        kLoopNext,
        // no parameters
        kMaskNegate,
        // no parameters
        kMaskPop,
        // Register mask
        kMaskPush,
        // Register target, Register left, Register right, uint8_t leftColsAndRightRows,
        // uint8_t leftRows, uint8_t rightCols
        kMatrixMultiply,
        // Register target, Register src, uint8_t srcColumns, uint8_t srcRows, uint8_t dstColumns,
        // uint8_t dstRows
        kMatrixToMatrix,
        // Register target, Register src1, Register src2
        kMultiplyF,
        // uint8_t count, Register target, Register src1, Register src2
        kMultiplyFN,
        // Register target, Register src1, Register src2
        kMultiplyI,
        // uint8_t count, Register target, Register src1, Register src2
        kMultiplyIN,
        // Register target, Register src
        kNegateF,
        // Register target, Register src
        kNegateS,
        // Register target, Register src
        kNot,
        // Register target, Register src1, Register src2
        kOr,
        // Register src
        kPrint,
        // Register target, uint8_t count, uint8_t index
        kReadExternal,
        // Register target, Register src1, Register src2
        kRemainderF,
        // uint8_t count, Register target, Register src1, Register src2
        kRemainderFN,
        // Register target, Register src1, Register src2
        kRemainderS,
        // uint8_t count, Register target, Register src1, Register src2
        kRemainderSN,
        // Register target, Register src1, Register src2
        kRemainderU,
        // uint8_t count, Register target, Register src1, Register src2
        kRemainderUN,
        // no parameters
        kReturn,
        // Register value
        kReturnValue,
        // Register target, Register src, uint8_t columns, uint8_t rows
        kScalarToMatrix,
        // Register target, Register test, Register ifTrue, Register ifFalse
        kSelect,
        // Register target, Register src, uint8_t count
        kShiftLeft,
        // Register target, Register src, uint8_t count
        kShiftRightS,
        // Register target, Register src, uint8_t count
        kShiftRightU,
        // Register target, Register src
        kSignedToFloat,
        // Register target, Register src,
        kSin,
        // Duplicates the src to <count> targets
        // uint8_t count, Register target, Register src
        kSplat,
        // Register target, Register src,
        kSqrt,
        // Store to the memory cell pointed to by dstPtr
        // Register dstPtr, Register src
        kStore,
        // uint8_t count, Register dstPtr, Register src
        kStoreN,
        // Store to the memory cell pointed to by dst
        // Pointer dst, Register src
        kStoreDirect,
        // uint8_t count, Pointer dst, Register src
        kStoreDirectN,
        // Store to the parameter slot pointed to by dstPtr
        // Register dstPtr, Register src
        kStoreParameter,
        // uint8_t count, Register dstPtr, Register src
        kStoreParameterN,
        // Store to the parameter slot pointed to by dst
        // Pointer dst, Register src
        kStoreParameterDirect,
        // uint8_t count, Pointer dst, Register src
        kStoreParameterDirectN,
        // Stores a register into the stack cell pointed to by dst + sp
        // Register dst, Register src
        kStoreStack,
        // uint8_t count, Register dst, Register src
        kStoreStackN,
        // Stores a register into the stack cell pointed to by dstPtr + sp
        // Pointer dst, Register src
        kStoreStackDirect,
        // uint8_t count, Pointer dst, Register src
        kStoreStackDirectN,
        // Register target, Register src1, Register src2
        kSubtractF,
        // uint8_t count, Register target, Register src1, Register src2
        kSubtractFN,
        // Register target, Register src1, Register src2
        kSubtractI,
        // uint8_t count, Register target, Register src1, Register src2
        kSubtractIN,
        // Register target, Register src,
        kTan,
        // Register target, Register src,
        kUnsignedToFloat,
        // uint8_t index, uint8_t count, Register src
        kWriteExternal,
        // Register target, Register src1, Register src2
        kXor,
    };


    // Compound values like vectors span multiple Registers or Pointer addresses. We always refer to
    // them by the address of their first slot, so for instance if you add two float4's together,
    // the resulting Register contains the first channel of the result, with the other three
    // channels following in the next three Registers.

    struct Register {
        uint16_t fIndex;

        Register operator+(uint16_t offset) const {
            return Register{(uint16_t) (fIndex + offset)};
        }
    };

    struct Pointer {
        uint16_t fAddress;

        Pointer operator+(uint16_t offset) const {
            return Pointer{(uint16_t) (fAddress + offset)};
        }
    };

    union Immediate {
        float fFloat;
        int32_t fInt;
        uint32_t fUInt;

        Immediate() {}

        Immediate(float f)
            : fFloat(f) {}

        Immediate(int32_t i)
            : fInt(i) {}

        Immediate(uint32_t u)
            : fUInt(u) {}
    };

    static constexpr int kPointerMax = 65535;
    static constexpr int kRegisterMax = 65535;

    const ByteCodeFunction* getFunction(const char* name) const {
        for (const auto& f : fFunctions) {
            if (f->fName == name) {
                return f.get();
            }
        }
        return nullptr;
    }

    int getGlobalSlotCount() const {
        return fGlobalSlotCount;
    }

    struct Uniform {
        SkSL::String fName;
        TypeCategory fType;
        int fColumns;
        int fRows;
        int fSlot;
    };

    int getUniformSlotCount() const { return fUniformSlotCount; }
    int getUniformCount() const { return fUniforms.size(); }
    int getUniformLocation(const char* name) const {
        for (int i = 0; i < (int)fUniforms.size(); ++i) {
            if (fUniforms[i].fName == name) {
                return fUniforms[i].fSlot;
            }
        }
        return -1;
    }
    const Uniform& getUniform(int i) const { return fUniforms[i]; }

private:
    ByteCode(const ByteCode&) = delete;
    ByteCode& operator=(const ByteCode&) = delete;

    std::vector<std::unique_ptr<ByteCodeFunction>> fFunctions;
    std::vector<ExternalValue*> fExternalValues;

    int fGlobalSlotCount;

    int fUniformSlotCount = 0;
    std::vector<Uniform> fUniforms;

    friend class ByteCodeGenerator;
    template<int width>
    friend class Interpreter;
};

} // namespace

#endif
