
#include <stdio.h>
#include "lua.h"
#include "lauxlib.h"

static void stackDump(lua_State *L);

int main(void) {
    lua_State *L = luaL_newstate();
    
    printf("Study Lua C Api, Lua version: %g\n", lua_version(L));
    
    // c -> stack
    lua_pushboolean(L, 1);
    lua_pushnumber(L, 10);
    lua_pushnil(L);
    lua_pushstring(L, "hello");
    
    stackDump(L);  // true    10    nil    hello
    
    // 将指定元素的副本压入栈
    lua_pushvalue(L, -4);
    stackDump(L); // true 10 nil hello true
    
    // 弹出栈顶元素，并替换指定位置的元素
    // lua_copy(L, -1, 3) true 10 true hello true
    // lua_pop(L, 1)      true 10 true hello
    lua_replace(L, 3);
    stackDump(L);
    
    // 将栈顶设知道所有6的位置，个数不够补nil
    lua_settop(L, 6);
    stackDump(L);  // true 10 true hello nil nil
    
    // 旋转，从3位置到栈顶元素，向栈顶方向移动1位
    lua_rotate(L, 3, 1);
    stackDump(L);  // true 10 nil true hello nil
    
    // 删除指定位置元素
    // lua_rotate(L, -3, -1) true 10 nil hello nil true
    // lua_pop(L, 1)         true 10 nil hello nil
    lua_remove(L, -3);
    stackDump(L);
    
    // -5位置做为栈顶
    lua_settop(L, -5);
    stackDump(L);  // true
    
    lua_pushinteger(L, 6);
    stackDump(L); // true 6
    lua_insert(L, 1);
    stackDump(L); // 6 true
    
    lua_close(L);
    return 0;
}


static void stackDump(lua_State *L) {
    int top = lua_gettop(L);
    // 从栈底到栈顶遍历
    for (int idx = 1; idx <= top; ++idx) {
        int type = lua_type(L, idx);
        switch (type) {
            case LUA_TSTRING:
                printf("%s", lua_tostring(L, idx));
                break;
            case LUA_TBOOLEAN: 
                printf(lua_toboolean(L, idx) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("%g", lua_tonumber(L, idx));
                break;
            default:
                printf("%s", lua_typename(L, type));
                break;
        }
        printf("    ");
    }
    printf("\n");
}
