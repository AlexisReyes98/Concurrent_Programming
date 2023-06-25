import time, os
import statistics

hilos = [1, 2, 4, 8, 16, 32]
tamArr = 10

for h in hilos:
    cmd = "./sumaSub " + str(tamArr) + " " + str(h)
    myList = []
    for r in range(5):
        # Tomar tiempo de reloj de pared antes de la corrida r
        inicio = time.time()
        # Correr programa con h hilos
        print(cmd)
        os.system(cmd)
        # Tomar tiempo de reloj de pared al final de la corrida
        final = time.time()
        # Calcular tiempo de la corrida r (diferencia = final-inicio)
        diferencia = (final-inicio)
        # Agregarlo a una lista para calcular el promedio al final
        myList.append(diferencia)
        print('\nTiempo de la corrida',r+1,':',diferencia)
        print('\n')
    # Fin de las 5 corridas con h hilos
    # Calcular el promedio de las 5 corridas con h hilos
    prom = statistics.mean(myList)
    print('\nPromedio de las 5 corridas con',h,'hilos:',prom)
    print('\n')
# Fin del ciclo anidado
