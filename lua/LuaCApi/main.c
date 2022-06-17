
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
    
    // ��ָ��Ԫ�صĸ���ѹ��ջ
    lua_pushvalue(L, -4);
    stackDump(L); // true 10 nil hello true
    
    // ����ջ��Ԫ�أ����滻ָ��λ�õ�Ԫ��
    // lua_copy(L, -1, 3) true 10 true hello true
    // lua_pop(L, 1)      true 10 true hello
    lua_replace(L, 3);
    stackDump(L);
    
    // ��ջ����֪������6��λ�ã�����������nil
    lua_settop(L, 6);
    stackDump(L);  // true 10 true hello nil nil
    
    // ��ת����3λ�õ�ջ��Ԫ�أ���ջ�������ƶ�1λ
    lua_rotate(L, 3, 1);
    stackDump(L);  // true 10 nil true hello nil
    
    // ɾ��ָ��λ��Ԫ��
    // lua_rotate(L, -3, -1) true 10 nil hello nil true
    // lua_pop(L, 1)         true 10 nil hello nil
    lua_remove(L, -3);
    stackDump(L);
    
    // -5λ����Ϊջ��
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
    // ��ջ�׵�ջ������
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
