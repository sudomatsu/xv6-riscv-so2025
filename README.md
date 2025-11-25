# Tarea 3: Protección de Memoria en xv6-riscv

## Introducción y Objetivos

En esta tarea trabajamos sobre el kernel de xv6 para agregar una nueva funcionalidad de seguridad: la capacidad de que un proceso proteja sus propias páginas de memoria contra lecturas. El objetivo principal fue implementar dos nuevas llamadas al sistema (syscalls): `mrdprotect` para quitar el permiso de lectura y `munrdprotect` para restaurarlo.

Esto nos permitió explorar cómo el sistema operativo interactúa directamente con el hardware (la MMU de RISC-V) para gestionar los permisos de acceso a memoria.

---

## ¿Cómo funciona nuestra implementación?

La lógica central reside en manipular los bits de la Tabla de Páginas (Page Table) del proceso en ejecución. Cada página de memoria tiene asociados ciertos permisos (Lectura, Escritura, Ejecución) definidos en sus "Page Table Entries" (PTE).

### Las System Calls

Implementamos dos funciones clave en el kernel:

1.  **`mrdprotect(addr, len)`**:
    *   Recibe una dirección de memoria y un tamaño.
    *   Recorre la tabla de páginas buscando las entradas correspondientes a ese rango.
    *   Para cada página encontrada, **apagamos el bit de lectura (`PTE_R`)**.
    *   Al final, le avisamos al procesador que actualice su caché de traducciones (TLB) usando `sfence_vma()`, para que el cambio sea inmediato.

2.  **`munrdprotect(addr, len)`**:
    *   Hace el proceso inverso. Recorre las mismas páginas y **vuelve a encender el bit de lectura**, restaurando el acceso normal.

---

## Cambios Realizados en el Código

Para lograr esto, tuvimos que tocar varios puntos del sistema operativo:

*   **`kernel/vm.c`**: Aquí escribimos la lógica pesada. Creamos las funciones que caminan por la tabla de páginas (`walk`), verifican que la memoria sea válida y modifican los bits de permiso.
*   **`kernel/sysproc.c`**: Añadimos la interfaz para que el usuario pueda llamar a nuestras funciones desde su código C.
*   **`kernel/syscall.h`, `syscall.c`**: Registramos las nuevas syscalls con los números 22 y 23 para que el kernel sepa qué hacer cuando las invocamos.
*   **`user/user.h`, `usys.pl`**: Preparamos todo para que sea fácil compilar programas de usuario que usen estas nuevas funciones.

---

## Compilación y Ejecución

Para probar nuestra implementación, es necesario compilar el sistema operativo completo y ejecutarlo bajo el emulador QEMU.

### Pasos para ejecutar:

1.  **Limpiar y Compilar**:
    Primero, nos aseguramos de eliminar cualquier archivo antiguo.
    ```bash
    make clean
    ```

2.  **Iniciar QEMU**:
    Lanzamos el sistema operativo xv6.
    ```bash
    make qemu
    ```

Una vez que el sistema arranca y vemos el prompt (`$`), estamos listos para ejecutar nuestras pruebas.

---

## Pruebas y Resultados

Utilizamos el programa `rdprotect_test` para verificar que todo funcionara. La prueba consiste en:
1.  Reservar una página de memoria.
2.  Escribir algo en ella.
3.  **Protegerla** usando `mrdprotect` (quitar lectura).
4.  Intentar acceder a ella nuevamente.

### Resultado

Al ejecutar la prueba, obtenemos esto:

```
$ rdprotect_test
usertrap(): unexpected scause 0xf pid=3
            sepc=0x30 stval=0x4000
```

El mensaje `usertrap` significa que el kernel atrapó al proceso intentando hacer algo ilegal. Como le quitamos el permiso de lectura a la memoria, cuando el programa intentó acceder a ella, el hardware RISC-V bloqueó el acceso y el sistema operativo detuvo el proceso.

Esto confirma que **nuestra protección funciona**: la memoria se volvió inaccesible.

---

## Conclusión

Logramos implementar con éxito la protección de memoria solicitada. Aprendimos a navegar por las estructuras de memoria del kernel, a modificar las tablas de paginación y a entender cómo el hardware impone restricciones de seguridad que el sistema operativo debe gestionar. El "crash" del programa de prueba es la prueba definitiva de que nuestro código está protegiendo la memoria correctamente.
