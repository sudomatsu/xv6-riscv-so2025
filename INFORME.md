
# Instalación de xv6-riscv en macOS  
**Grupo-F de Sistemas Operativos (Segundo Semestre 2025)**  
*Por Matías Brickle*  

---

## Pasos de instalación

### 1. Instalar dependencias necesarias
Utilizar Homebrew (el gestor de paquetes de macOS, similar a `apt-get` en Linux):

```bash
brew install qemu
brew tap riscv-software-src/riscv
brew install riscv-tools
```
### 2. Clonar el repositorio de xv6-riscv
Con git clonamos el proyecto oficial:

```bash
git clone https://github.com/mit-pdos/xv6-riscv.git
```
### 3. Acceder a la carpeta del proyecto
Entramos a la carpeta recién clonada:
```bash
cd xv6-riscv
```
### 4. Compilar xv6
Ejecutar make para construir el sistema operativo:
```bash
make
```

```
Nota: si make no está instalado, se puede instalar con:
brew install make
```

### 5. Ejecutar xv6 en QEMU
Finalmente, iniciamos QEMU con:
```bash
make qemu
```
Esto arrancará xv6-riscv en un entorno emulado y ya tendrás el sistema corriendo en tu sistema.

## Problemas y Soluciones
Para poder instalar xv6-riscv no tuve ningún problema dentro de mi plataforma ya que tenía todas las dependencias necesarias como `make` y todo pudo construirse correctamente.

## Pruebas de Ejecución
Comando: `echo "Hola xv6"`
![Comando Echo](https://img001.prntscr.com/file/img001/OYEyH6d_QQCc9IJFfSYDSQ.png)

Comando `ls`
![comando ls](https://img001.prntscr.com/file/img001/MvzyH_S0TO6YO3UMS3vJFg.png)

Comando `cat README`

![Comando README](https://img001.prntscr.com/file/img001/9-VXn31NQSWGCyMqFHqSdA.png)

## Conclusión

Con estos pasos logramos instalar y ejecutar xv6-riscv en un entorno emulado con QEMU, lo que nos brinda una plataforma ideal para experimentar con conceptos de sistemas operativos. Este entorno nos permite comprender mejor cómo funciona un kernel, practicar con llamadas al sistema, administración de procesos, memoria y otros elementos fundamentales, en un ambiente seguro y controlado sin dañar nuestro computador.