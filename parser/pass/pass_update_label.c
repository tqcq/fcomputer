/**
 * Created by Tqcq Ann on 11/19/22.
**/
#include "pass_update_label.h"

static int IsRelativeAddress(const char* opname) {
        static char opname_table[][100] = {
                "store_pc",
                "jmp",
                "jl",
                "je",
                "jg",
                "jle",
                "jge",
                "jne",
        };

        for (int i = 0; i < sizeof(opname_table); ++i)
                if (!strcmp(opname, opname_table[i]))
                        return 1;
        return 0;
}

void PassUpdateLabel(Program *program) {
    const int boot_address = 0x40000000;

    if (!program)
        return;
    char label_name[MAX_PROGRAM_SIZE][100];
    int  label_address[MAX_PROGRAM_SIZE];
    int  label_len = 0;

    int offset = 0;
    for (int i = 0; i < program->len; ++i) {
        InstructionPtr in = program->instruction_list[i];
        if (!in)
            continue;
        if (in->type == I_LABEL) {
            strcpy(label_name[label_len], in->args[0].data);
            label_address[label_len] = i + offset;
            // printf("%s %d\n", label_name[label_len], label_address[label_len]);
            label_len++;
            --offset;
        }
    }

    offset = 0;
    for (int i = 0; i < program->len; ++i) {
        InstructionPtr in = program->instruction_list[i];
        if (!in)
            continue;

        if (in->type == I_LABEL) {
            --offset;
        }
        int cur_address = i + offset;

        if (in->type == I_RI) {
            if (in->args[2].type == OPERAND_LABEL) {
                int idx = 0;
                while (idx < label_len && strcmp(label_name[idx], in->args[2].data)) {
                    ++idx;
                }
                // printf("[%3d] %s %s %d\n", idx, in->args[2].data, label_name[idx], label_address[idx]);

                if (IsRelativeAddress(in->opname)) {
                    sprintf(in->args[2].data, "%d", label_address[idx] - cur_address);
                } else {
                    sprintf(in->args[2].data, "%d", label_address[idx] + boot_address);
                }
                in->args[2].type = OPERAND_IMM;
            }
        }
    }
}
