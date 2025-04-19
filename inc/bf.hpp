#pragma once

enum INS : char
{
    MOV_IP_RIGHT   = '>',
    MOV_IP_LEFT    = '<',
    INC_BYTE       = '+',
    DEC_BYTE       = '-',
    OUT_BYTE       = '.',
    ACCEPT_INPUT   = ',',
    COND_JMP_START = '[', // if byte at DP is 0, goto end
    COND_JMP_END   = ']'  // if byte at DP is !0, goto begin
};
