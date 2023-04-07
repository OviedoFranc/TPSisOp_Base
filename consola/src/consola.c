#include "consola.h"
int main(int argc, char ** argv){

    printf("Iniciando la consola");

    if(argc > 1 && strcmp(argv[1],"-test")==0)
        return run_tests();
    else{  
        t_log* logger = log_create("./cfg/consola.log", "consola", true, LOG_LEVEL_INFO);
        log_info(logger, "Soy la consola! %s", mi_funcion_compartida());
        log_destroy(logger);
    } 
}