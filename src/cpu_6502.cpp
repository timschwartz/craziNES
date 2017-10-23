#include <cpu_6502.h>
#include <iostream>

namespace nes
{
    uint16_t cpu_6502::get_PC()
    {
        return PC;
    }

    uint8_t cpu_6502::get_status()
    {
        return status;
    }

    std::string cpu_6502::get_state()
    {
        char message[1024];
        sprintf(message, " A:%.2X X:%.2X Y:%.2X P:%.2X SP:%.2X CYC: %d", 
                A, X, Y, status, SP, (cycles * 3) % 341);
        return std::string(message);
    }

    void cpu_6502::push8(uint8_t value)
    {
        mmu->write_byte(SP + 0x100, value);
        SP--;
    }

    uint8_t cpu_6502::pop8()
    {
        uint8_t value = mmu->read_byte(SP + 0x100);
        SP++;
        return value;
    }

    void cpu_6502::push16(uint16_t value)
    {
        mmu->write_byte(SP + 0x100 - 1, value & 0xFF);
        mmu->write_byte(SP + 0x100, (value >> 8) & 0xFF);
        SP -= 2;
    }

    uint16_t cpu_6502::pop16()
    {
        uint16_t value = (mmu->read_byte(SP + 0x100 + 1) << 8) + mmu->read_byte(SP + 0x100);
        SP += 2;
        return value;
    }

    void cpu_6502::setN(uint8_t check)
    {
        if((check >> 7)) status |= 0x80;
        else status &= ~0x80;
    }

    bool cpu_6502::checkN()
    {
        return (status >> 7) & 0x1;
    }

    void cpu_6502::setV(uint8_t check)
    {
        if((check >> 6) & 1) status |= 0x40;
        else status &= ~0x40;
    }

    void cpu_6502::clearV()
    {
        status &= ~0x40;
    }

    bool cpu_6502::checkV()
    {
        return (status >> 6) & 0x1;
    }

    void cpu_6502::setZ(uint8_t check)
    {
        if(check == 0) status |= 0x2;
        else status &= ~0x2; 
    }

    bool cpu_6502::checkZ()
    {
        return status & 0x2;
    }

    void cpu_6502::setC()
    {
        status |= 0x1;
    }

    void cpu_6502::clearC()
    {
        status &= ~0x1;
    }

    bool cpu_6502::checkC()
    {
        return status & 0x1;
    }

    void cpu_6502::setI()
    {
        status |= 0x4;
    }

    void cpu_6502::setD()
    {
        status |= 0x8;
    }

    void cpu_6502::clearD()
    {
        status &= ~0x8;
    }

    void cpu_6502::not_implemented(cpu_6502 *cpu, opcode_t *op)
    {
        char message[1024];
        sprintf(message, "0x%.4X: NOT IMPLEMENTED %s\n", cpu->get_PC(), op->to_string().c_str());
        throw std::string(message);
    }

    void cpu_6502::brk(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x00
        cpu->cycles += 7;
        cpu->PC += op->sz;
        uint8_t status = cpu->status;
        status |= 0x10; 
        cpu->push16(cpu->PC);
        cpu->push8(status);
        cpu->setI();
        cpu->PC = (cpu->mmu->read_byte(0xFFFF) << 8) | cpu->mmu->read_byte(0xFFFE);
        
/*
        char message[1024];
        sprintf(message, "Jumping to 0x%.4X %s", cpu->PC, cpu->get_state().c_str());
        throw std::string(message);
*/
    }

    void cpu_6502::bpl(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x10
        if(!cpu->checkN())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::clc(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x18
        cpu->clearC();
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::jsr(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x20
        uint16_t addr = cpu->PC +  op->sz - 1;
        cpu->push16(addr);
        cpu->PC = op->imm;
        cpu->cycles += 6;
    }

    void cpu_6502::bit_zp(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x24
        uint16_t addr = (0 << 8) | op->imm;
        uint8_t acc = cpu->A;
        uint8_t test = cpu->mmu->read_byte(addr);

        cpu->setZ(acc & test);
        cpu->setN(test);
        cpu->setV(test);
        cpu->PC += op->sz;
        cpu->cycles += 3;
    }

    void cpu_6502::and_imm(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x29
        cpu->A &= op->imm;
        cpu->cycles += 2;
        cpu->PC += op->sz;
        cpu->setZ(cpu->A); 
    }

    void cpu_6502::sec(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x38
        cpu->setC();
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::rti(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x40
        cpu->cycles += 6;
        cpu->status = cpu->pop8();
        cpu->PC = cpu->pop16();

        char message[1024];
        sprintf(message, "PC: 0x%.4X   P: 0x%.2X", cpu->PC, cpu->status);
//        throw std::string(message);
    }

    void cpu_6502::jmp_abs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x4C
        cpu->PC = op->imm;
        cpu->cycles += 3;
    }

    void cpu_6502::bvc(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x50
        if(!cpu->checkV())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::rts(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x60
/*
        char message[1024];
        sprintf(message, "Popped 0x%.4X off stack, new pointer: 0x%.2X", value, cpu->SP);
        throw std::string(message);
*/
        cpu->PC = cpu->pop16() + 1;
        cpu->cycles += 6;
    }

    void cpu_6502::bvs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x70
        if(cpu->checkV())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::sei(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x78
        cpu->cycles += 2;
        cpu->PC += op->sz;
        cpu->setI();
    }

    void cpu_6502::sta_zp(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x85
        uint16_t addr = (0 << 8) | op->imm;
        cpu->mmu->write_byte(addr, cpu->A);
        cpu->cycles += 3;
        cpu->PC += op->sz;
    }

    void cpu_6502::stx_zp(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x86
        uint16_t addr = (0 << 8) | op->imm;
        cpu->mmu->write_byte(addr, cpu->X);
        cpu->cycles += 3;
        cpu->PC += op->sz;
    }

    void cpu_6502::sta_abs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x8D
        cpu->mmu->write_byte(op->imm, cpu->A);
        cpu->cycles += 4;
        cpu->PC += op->sz;
    }

    void cpu_6502::bcc(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x90
        if(!cpu->checkC())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::txs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0x9A
        cpu->SP = cpu->X;
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::ldx_imm(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xA2
        cpu->X = op->imm;
        cpu->cycles += 2;
        cpu->PC += op->sz;
        cpu->setZ(cpu->X);
    }

    void cpu_6502::lda_imm(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xA9
        cpu->A = op->imm;
        cpu->cycles += 2;
        cpu->PC += op->sz;
        cpu->setZ(cpu->A);
        cpu->setN(cpu->A);
    }

    void cpu_6502::lda_abs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xAA
        cpu->A = cpu->mmu->read_byte(op->imm);
        cpu->cycles += 2;
        if((cpu->PC >> 8) != (op->imm >>8)) cpu->cycles++;
        cpu->PC += op->sz;
        cpu->setZ(cpu->A);
    }

    void cpu_6502::bcs(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xB0
        if(cpu->checkC())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::clv(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xB8
        cpu->clearV();
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::bne(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xD0
        if(!cpu->checkZ())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    void cpu_6502::cld(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xD8
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::nop_ea(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xEA
        cpu->cycles += 2;
        cpu->PC += op->sz;
    }

    void cpu_6502::beq(cpu_6502 *cpu, opcode_t *op)
    {
        // 0xF0
        if(cpu->checkZ())
        {
            uint16_t next_addr = cpu->PC + op->sz;
            cpu->PC += op->sz + op->imm;
            cpu->cycles += 3;
            if((next_addr >> 8) != (cpu->PC >> 8)) cpu->cycles += 2;
        }
        else
        {
            cpu->PC += op->sz;
            cpu->cycles += 2;
        }
    }

    cpu_6502::cpu_6502()
    {
        mmu = new MMU();

        uint16_t counter = 0;
        while(counter < 256)
        {
            opcode[counter] = not_implemented;
            counter++;
        }

        opcode[BRK] = brk;
        opcode[BPL] = bpl;
        opcode[CLC] = clc;
        opcode[JSR] = jsr;
        opcode[BIT_zp] = bit_zp;
        opcode[AND_imm] = and_imm;
        opcode[SEC] = sec;
        opcode[RTI] = rti;
        opcode[JMP_abs] = jmp_abs;
        opcode[BVC] = bvc; 
        opcode[RTS] = rts;
        opcode[BVS] = bvs;
        opcode[SEI] = sei;
        opcode[STA_zp] = sta_zp;
        opcode[STX_zp] = stx_zp;
        opcode[STA_abs] = sta_abs;
        opcode[BCC] = bcc;
        opcode[TXS] = txs;
        opcode[LDX_imm] = ldx_imm;
        opcode[LDA_imm] = lda_imm;
        opcode[LDA_abs] = lda_abs;
        opcode[BCS] = bcs;
        opcode[CLV] = clv;
        opcode[BNE] = bne;
        opcode[CLD] = cld;
        opcode[NOP_EA] = nop_ea;
        opcode[BEQ] = beq;
    }

    void cpu_6502::load_rom(std::string filename)
    {
        try
        {
            mmu->load_rom(filename); 
        }
        catch(std::string e)
        {
            throw e;
        }

        PC = (mmu->read_byte(0xFFFD) << 8) | mmu->read_byte(0xFFFC);
    }

    std::string cpu_6502::step()
    {
        opcode_t *op = new opcode_t(mmu, PC);
        char message[1024];
        sprintf(message, "%.4X %s", PC, op->to_string().c_str());

        std::string state = message + get_state();

        try
        {
            opcode[op->opcode](this, op);
        }
        catch(std::string e)
        {
            throw e;
        }

        return state;
    }

    opcode_t::opcode_t(nes::MMU *mmu, uint16_t addr)
    {
        opcode = mmu->read_byte(addr);
        instruction = opcode;

        switch(opcode)
        {
            case BRK:
                sz = 1;
                break;
            case NOP_zp_04:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case ORA_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_abs_0C:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case PHP:
                sz = 1;
                break;
            case NOP_zp_14:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case CLC:
                sz = 1;
                break;
            case NOP_1A:
                sz = 1;
                break;
            case NOP_abs_1C:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case BPL:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case JSR:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case BIT_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PLP:
                sz = 1;
                break;
            case AND_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case BIT_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case NOP_zp_34:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case SEC:
                sz = 1;
                break;
            case RTI:
                sz = 1;
                break;
            case NOP_3A:
                sz = 1;
                break;
            case NOP_zp_44:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PHA:
                sz = 1;
                break;
            case LSR_acc:
                sz = 1;
                break;
            case JMP_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case BVC:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_54:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_5A:
                sz = 1;
                break;
            case NOP_abs_5C: 
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case RTS:
                sz = 1;
                break;
            case NOP_zp_64:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case PLA:
                sz = 1;
                break;
            case BVS:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_74:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case SEI:
                sz = 1;
                break;
            case NOP_7A:
                sz = 1;
                break;
            case NOP_abs_7C:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case NOP_imm_80:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STY_zp_x:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STA_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case STX_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case DEY:
                sz = 1;
                break;
            case STY_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case STA_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case STX_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case TXS:
                sz = 1;
                break;
            case BCC:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDY_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDX_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDX_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDA_abs:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case BCS:
                sz  = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case LDY_zp_x:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case CLV:
                sz = 1;
                break;
            case LDA_abs_x:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case DEC_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INY:
                sz = 1;
                break;
            case CMP_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case DEX:
                sz = 1;
                break;
            case BNE:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_D4:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case CLD:
                sz = 1;
                break;
            case NOP_DA:
                sz = 1;
                break;
            case NOP_abs_DC:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case CPX_imm:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INC_zp:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case INX:
                sz = 1;
                break;
            case NOP_EA:
                sz = 1;
                break;
            case BEQ:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_zp_F4:
                sz = 2;
                imm = mmu->read_byte(addr + 1);
                instruction = (opcode << 8) | imm;
                break;
            case NOP_FA:
                sz = 1;
                break;
            case NOP_abs_FC:
                sz = 3;
                imm = (mmu->read_byte(addr + 2) << 8) | mmu->read_byte(addr + 1);
                instruction = (opcode << 16) | imm;
                break;
            case 0xFF:
                sz = 1;
                break;
            default:
                char message[1024];
                sprintf(message, "Unknown opcode 0x%.2X", opcode);
                throw std::string(message);
                break;
        }
    }

    std::string opcode_t::to_string()
    {
        char temp[1024];

        switch(opcode)
        {
            case BRK:
                sprintf(temp, "BRK");
                break;
            case NOP_zp_04:
                sprintf(temp, "NOP(0x04) 0x%.2X, 0", imm);
                break;
            case ORA_zp:
                sprintf(temp, "ORA 0x%.2X, 0", imm);
                break;
            case NOP_abs_0C:
                sprintf(temp, "NOP(0x0C) 0x%.4X", imm);
                break;
            case PHP:
                sprintf(temp, "PHP");
                break;
            case NOP_zp_14:
                sprintf(temp, "NOP(0x14) 0x%.2X, 0", imm);
                break;
            case CLC:
                sprintf(temp, "CLC");
                break;
            case NOP_1A:
                sprintf(temp, "NOP(0x1A)");
                break;
            case NOP_abs_1C:
                sprintf(temp, "NOP(0x1C) 0x%.2X", imm);
                break;
            case BPL:
                sprintf(temp, "BPL #0x%.2X", imm);
                break;
            case JSR:
                sprintf(temp, "JSR 0x%.4X", imm);
                break;
            case BIT_zp:
                sprintf(temp, "BIT 0x%.2X", imm);
                break;
            case PLP:
                sprintf(temp, "PLP");
                break;
            case AND_imm:
                sprintf(temp, "AND #0x%.2X", imm);
                break;
            case BIT_abs:
                sprintf(temp, "BIT 0x%.4X", imm);
                break;
            case NOP_zp_34:
                sprintf(temp, "NOP(0x34) 0x%.2X, 0", imm);
                break;
            case SEC:
                sprintf(temp, "SEC");
                break;
            case RTI:
                sprintf(temp, "RTI");
                break;
            case NOP_3A:
                sprintf(temp, "NOP(0x3A)");
                break;
            case NOP_zp_44:
                sprintf(temp, "NOP(0x44) 0x%.2X, 0", imm);
                break;
            case PHA:
                sprintf(temp, "PHA");
                break;
            case LSR_acc:
                sprintf(temp, "LSR accumulator");
                break;
            case JMP_abs:
                sprintf(temp, "JMP 0x%.4X", imm);
                break;
            case BVC:
                sprintf(temp, "BVC 0x%.2X", imm);
                break;
            case NOP_zp_54:
                sprintf(temp, "NOP(0x54) 0x%.2X, 0", imm);
                break;
            case NOP_5A:
                sprintf(temp, "NOP(0x5A)");
                break;
            case NOP_abs_5C:
                sprintf(temp, "NOP(0x5C) 0x%.4X, X", imm);
                break;
            case RTS:
                sprintf(temp, "RTS");
                break;
            case NOP_zp_64:
                sprintf(temp, "NOP(0x64) 0x%.2X, 0", imm);
                break;
            case PLA:
                sprintf(temp, "PLA");
                break;
            case BVS:
                sprintf(temp, "BVS 0x%.2X, 0", imm);
                break;
            case NOP_zp_74:
                sprintf(temp, "NOP(0x74) 0x%.2X, 0", imm);
                break;
            case NOP_abs_7C:
                sprintf(temp, "NOP(0x7C) 0x%.4X, X", imm);
                break;
            case SEI:
                sprintf(temp, "SEI");
                break;
            case NOP_7A:
                sprintf(temp, "NOP(0x7A)");
                break;
            case NOP_imm_80:
                sprintf(temp, "NOP(0x80) 0x%.2X, 0", imm);
                break;
            case STY_zp_x:
                sprintf(temp, "STY 0x%.2X, X", imm);
                break;
            case STA_zp:
                sprintf(temp, "STA 0x%.2X, 0", imm);
                break;
            case STX_zp:
                sprintf(temp, "STX 0x%.2X, 0", imm);
                break;
            case DEY:
                sprintf(temp, "DEY");
                break;
            case STY_abs:
                sprintf(temp, "STY 0x%.4X", imm);
                break;
            case STA_abs:
                sprintf(temp, "STA 0x%.4X", imm);
                break;
            case STX_abs:
                sprintf(temp, "STX 0x%.4X", imm);
                break;
            case TXS:
                sprintf(temp, "TXS");
                break;
            case BCC:
                sprintf(temp, "BCC 0x%.2X", imm);
                break;
            case LDY_imm:
                sprintf(temp, "LDY #0x%.2X", imm);
                break;
            case LDX_imm:
                sprintf(temp, "LDX #0x%.2X", imm);
                break;
            case LDA_zp:
                sprintf(temp, "LDA 0x%.2X, 0", imm);
                break;
            case LDX_zp:
                sprintf(temp, "LDX 0x%.2X, 0", imm);
                break;
            case LDA_imm:
                sprintf(temp, "LDA #0x%.2X", imm);
                break;
            case LDA_abs:
                sprintf(temp, "LDA 0x%.4X", imm);
                break;
            case BCS:
                sprintf(temp, "BCS 0x%.2X", imm);
                break;
            case LDY_zp_x:
                sprintf(temp, "LDY 0x%.2X, X", imm);
                break;
            case CLV:
                sprintf(temp, "CLV");
                break;
            case LDA_abs_x:
                sprintf(temp, "LDA 0x%.4X, X", imm);
                break;
            case DEC_zp:
                sprintf(temp, "DEC 0x%.2X, 0", imm);
                break;
            case INY:
                sprintf(temp, "INY");
                break;
            case CMP_imm:
                sprintf(temp, "CMP #0x%.2X, 0", imm);
                break;
            case DEX:
                sprintf(temp, "DEX");
                break;
            case BNE:
                sprintf(temp, "BNE #0x%.2X", imm);
                break;
            case NOP_zp_D4:
                sprintf(temp, "NOP(0xD4) 0x%.2X, 0", imm);
                break;
            case CLD:
                sprintf(temp, "CLD");
                break;
            case NOP_DA:
                sprintf(temp, "NOP(0xDA)");
                break;
            case NOP_abs_DC:
                sprintf(temp, "NOP(0xDC) 0x%.4X, X", imm);
                break;
            case CPX_imm:
                sprintf(temp, "CPX #0x%.2X", imm);
                break;
            case INC_zp:
                sprintf(temp, "INC 0x%.2X, 0", imm);
                break;
            case INX:
                sprintf(temp, "INX");
                break;
            case NOP_EA:
                sprintf(temp, "NOP(0xEA)");
                break;
            case BEQ:
                sprintf(temp, "BEQ 0x%.2X, 0", imm);
                break;
            case NOP_zp_F4:
                sprintf(temp, "NOP(0xF4) 0x%.2X, 0", imm);
                break;
            case NOP_FA:
                sprintf(temp, "NOP(0xFA)");
                break;
            case NOP_abs_FC:
                sprintf(temp, "NOP(0xFC) 0x%.4X, X", imm);
                break;
            case 0xFF:
                break;
            default:
                sprintf(temp, "Unknown opcode 0x%.2X", opcode);
                break;
        }

        char message[1024];
        sprintf(message, "   %s", temp);
        return std::string(message);
    }
}
