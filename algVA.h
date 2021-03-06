//
// Created by kommyhap on 19.04.17.
//

#ifndef LAB6_ALGVA_H
#define LAB6_ALGVA_H

/*
 *              Алгоритм отсечения многоугольника Вейлера-Азертона
 * Каждый из многоугольников задан списком вершин, причем таким образом,
 *  что при движении по списку вершин в порядке их задания внутренняя
 *  область многоугольника находится справа от границы.
 *
 * 1  Строятся списки вершин отсекаемого многоугольника и окна
 * 2. Отыскиваются все точки пересечения.
 *      При этом расчете касания не считаются пересечением, т.е.
 *      когда вершина или ребро отсекаемого многоугольника
 *      инцидентна или совпадает со стороной окна.
 * 3. Списки координат вершин отсекаемого многоугольника и окна
 *      дополняются новыми вершинами - координатами точек
 *      пересечения. Причем если точка пересечения Pk находится
 *      на ребре, соединяющем вершины Vi, Vj, то
 *      последовательность точек Vi, Vj превращается в
 *      последовательность Vi, Pk, Vj.
 *      При этом устанавливаются двухсторонние связи между
 *      одноименными точками пересечения в списках вершин
 *      отсекаемого многоугольника и окна.
 *
 *      Входные и выходные точки пересечения образуют отдельные
 *      подсписки входных и выходных точек в списках вершин.
 * 4. Определение части обрабатываемого многоугольника, попавшей в окно выполняется следующим образом:
 *      Если не исчерпан список входных точек пересечения, то выбираем очередную входную точку.
 *          a) двигаемся по вершинам отсекаемого многоугольника пока не обнаружится следующая точка пересечения;
 *          b) все пройденные точки, не включая прервавшую просмотр, заносим в результат;
 *          c) используя двухстороннюю связь точек пересечения, переключаемся на просмотр списка вершин окна.
 *                  Двигаемся по вершинам окна до обнаружения следующей точки пересечения;
 *                  все пройденные точки, не включая последнюю, прервавшую просмотр, заносим в результат.
 *             используя двухстороннюю связь точек пересечения, переключаемся на список вершин обрабатываемого многоугольника.
 *      Эти действия повторяем пока не будет достигнута исходная вершина - очередная часть отсекаемого многоугольника,
 *      попавшая в окно, замкнулась. Переходим на выбор следующей входной точки в списке отсекаемого многоугольника.
 */


#endif //LAB6_ALGVA_H
