import time, os
import statistics

hilos = [2, 4, 8, 16, 32]

for h in hilos:
    cmd = "./multiMatHilos " + str(h)
    myList = []
    for r in range(5):
        # Tomar tiempo de reloj de pared antes de la corrida r
        inicio = time.time()
        # Correr programa de matrices con h hilos
        print(cmd)
        os.system(cmd)
        # Tomar tiempo de reloj de pared al final de la corrida
        final = time.time()
        # Calcular tiempo de la corrida r (diferencia final-inicio)
        diferencia = (final-inicio)
        # Imprimirlo o agregarlo a una lista para calcular el promedio al final
        myList.append(diferencia)
        print('\nTiempo de la corrida',r+1,':',diferencia)
        print('\n')
    # Fin de las 5 corridas con h hilos
    # Si agregaron los promedio a una lista, calcular promedio de las 5 corridas con h hilos
    prom = statistics.mean(myList)
    print('\nPromedio de las 5 corridas con',h,'hilos:',prom)
    print('\n')
# Fin del ciclo anidado
