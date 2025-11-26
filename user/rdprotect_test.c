#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    printf("=== Test mrdprotect ===\n");
    
    char *addr = sbrk(4096);
    if(addr == (char*)-1){
        printf("sbrk falló\n");
        exit(1);
    }
    
    addr[0] = 'Z';
    if(mrdprotect(addr, 1) < 0) {
        printf("mrdprotect falló\n");
        exit(1);
    }
    printf("2. Permiso de lectura removido (mrdprotect OK).\n");

    int pid = fork();
    if(pid < 0){
        printf("fork falló\n");
        exit(1);
    }

    if(pid == 0) {
        // --- PROCESO HIJO ---
        printf("   [Hijo] Intentando leer memoria protegida...\n");
        
        // Esta lectura debería causar un 'Load Page Fault' (scause 0xd)
        // y el kernel matará a este proceso.
        char c = addr[0]; 
        
        printf("   [Hijo] ERROR FATAL: Pude leer '%c' sin permiso!\n", c);
        exit(1); 
    }

    // --- PROCESO PADRE ---
    wait(0); // Esperar a que el hijo muera por el trap
    printf("3. El proceso hijo terminó.\n");

    // 4. Restaurar permisos
    if(munrdprotect(addr, 1) < 0) {
        printf("munrdprotect falló\n");
        exit(1);
    }
    printf("4. Permisos restaurados (munrdprotect OK).\n");

    // 5. Verificar que ahora sí podemos leer
    printf("5. Verificando lectura tras restaurar...\n");
    char c = addr[0];
    
    if(c == 'Z'){
        printf("   Lectura OK: Valor leído '%c'\n", c);
        printf("=== TEST PASS ===\n");
    } else {
        printf("   Lectura FALLÓ: Valor esperado 'Z', obtenido '%c'\n", c);
        printf("=== TEST FAIL ===\n");
    }

    exit(0);
}
