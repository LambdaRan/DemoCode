#include <stdio.h>
#include <limits.h>

#include "lua.h"
#include "lauxlib.h"

typedef struct BitArray {
    int size;
    unsigned int values[1];
} BitArray;

#define BITS_PER_WORD (CHAR_BIT * sizeof(unsigned int))
#define I_WORD(i) ((unsigned int)(i) / BITS_PER_WORD)
#define I_BIT(i) (1 << (unsigned int)(i) % BITS_PER_WORD)

#define CheckBitArray(L) (BitArray *)luaL_checkudata(L, 1, "LambdaRan.BitArray")

// 1. size
static int newarray(lua_State *L) {
    int n = (int)luaL_checkinteger(L, 1);
    luaL_argcheck(L, n >= 1, 1, "Invalid size");
    int nbytes = sizeof(BitArray) + I_WORD(n - 1) * sizeof(unsigned int);
    BitArray* a = (BitArray*)lua_newuserdata(L, nbytes);
    a->size = n;
    for (int i = 0; i <= I_WORD(n - 1); i++)
        a->values[i] = 0;
    
    luaL_getmetatable(L, "LambdaRan.BitArray");
    lua_setmetatable(L, -2);
    return 1;
}

static unsigned int *getparams(lua_State *L, unsigned int *mask) {
    BitArray* a = CheckBitArray(L);
    
    int index = (int)luaL_checkinteger(L, 2) - 1;
    luaL_argcheck(L, index >= 0 && index <= a->size, 2, "Index out of range");
    
    *mask = I_BIT(index);
    return &(a->values[I_WORD(index)]);
}

// arr/index/value
static int setarray(lua_State *L) {
    unsigned int mask;
    unsigned int *entry = getparams(L, &mask);
    luaL_checkany(L, 3);
    
    if (lua_toboolean(L, 3)) {
        *entry |= mask;
    } else {
        *entry &= ~mask;
    }
    return 0;
}

// array/index
static int getarray(lua_State *L) {
    unsigned int mask;
    unsigned int *entry = getparams(L, &mask);
    lua_pushboolean(L, *entry & mask);
    return 1;
}

static int getsize(lua_State *L) {
    BitArray* a = CheckBitArray(L);
    luaL_argcheck(L, a != NULL, 1, "'array' expected");
    lua_pushinteger(L, a->size);
    return 1;
}

static int array2string(lua_State *L) {
    BitArray* a = CheckBitArray(L);
    lua_pushfstring(L, "BitArray(%d)", a->size);
    return 1;
}
// static const struct luaL_Reg bitarraylib[] = {
//     {"new", newarray},
//     {"set", setarray},
//     {"get", getarray},
//     {"size", getsize},
//     {NULL, NULL}
// };
// https://www.xknote.com/ask/60c892235bf3a.html
// http://lua-users.org/wiki/BuildingModules
// __declspec(dllexport)
// int luaopen_bitarray(lua_State *L) {
//     // 区别不同类型的用户数据，一种常用的方法是为每种类型创建唯一的元表。
//     luaL_newmetatable(L, "LambdaRan.BitArray");
//     luaL_newlib(L, bitarraylib);
//     return 1;
// }

// 面向对象访问
// a:size() 等价于 a.size(a)
static const struct luaL_Reg bitarraylib_f[] = {
    {"new", newarray},
    {NULL, NULL}
};

static const struct luaL_Reg bitarraylib_m[] = {
    {"set", setarray},
    {"get", getarray},
    {"size", getsize},
    {"__tostring", array2string},
    {NULL, NULL}
};
__declspec(dllexport)
int luaopen_bitarray(lua_State *L) {
    luaL_newmetatable(L, "LambdaRan.BitArray");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");  // mt.__index = mt
    luaL_setfuncs(L, bitarraylib_m, 0);  // 注册元方法
    luaL_newlib(L, bitarraylib_f);   // 创建库
    return 1;
}

// 数组访问
// static const struct luaL_Reg bitarraylib_f[] = {
//     {"new", newarray},
//     {NULL, NULL}
// };

// static const struct luaL_Reg bitarraylib_m[] = {
//     {"__newindex", setarray},
//     {"__index", getarray},
//     {"__len", getsize},
//     {"__tostring", array2string},
//     {NULL, NULL}
// };
// __declspec(dllexport)
// int luaopen_bitarray(lua_State *L) {
//     luaL_newmetatable(L, "LambdaRan.BitArray");
//     luaL_setfuncs(L, bitarraylib_m, 0);  // 注册元方法
//     luaL_newlib(L, bitarraylib_f);   // 创建库
//     return 1;
// }


