#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
struct Trailer;

struct Lider{
    int key;     //Номер вершины
    int count;   //Число предшественников (число входящих дуг)
    Lider* nx;   //Указатель на следующий элемент в списке вершин (Lider)
    Trailer* tr; //указатель на ТRAILER (список указателей на вершины, к которым ведут исходящие дуги)
};

struct Trailer{
    Lider* ld;   //указатель на вершину в списке LEADER, к которой ведет
                 //исходящая дуга из вершины, к которой прикреплен TRAILER
    Trailer* nx; //указатель на следующий элемент
};

/*	
*   AddEdge ­ добавление ребра в граф
*	Входные параметры:
*	vertexes ­ список вершин графа
*	Ребро графа определяется парой вершин(vertexA,vertexB)
*/
void AddEdge(Lider*& vertexes, int vertexA, int vertexB) {
    Lider* ptrA = NULL, *ptrB = NULL;
    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);

    bool setEdge = true;
    // проверяем есть ли ребро или нет, если оно есть, то устанавливаем флаг false
    if (ptrA && ptrB) {
        Trailer* trTmp = ptrA->tr;
        for (trTmp = ptrA->tr; trTmp && trTmp->ld != ptrB; trTmp = trTmp->nx);
        setEdge = !((bool)trTmp);
    }
    else {
        //Создаем вершину А, добавляем в голову списка vertexes
        if (!ptrA) {
            ptrA = new Lider;
            ptrA->key = vertexA;
            ptrA->count = 0;
            ptrA->nx = NULL;
            ptrA->tr = NULL;

            ptrA->nx = vertexes;
            vertexes = ptrA;
        }

        //Создаем вершину B , добавляем в голову списка vertexes
        if (!ptrB) {
            ptrB = new Lider;
            ptrB->key = vertexB;
            ptrB->count = 0;
            ptrB->nx = NULL;
            ptrB->tr = NULL;

            ptrB->nx = vertexes;
            vertexes = ptrB;
        }

    }

    //Устанавливаем ребро
    if (setEdge) {
        ptrB->count++;
        Trailer* elemTr = new Trailer;
        elemTr->ld = ptrB;
        elemTr->nx = ptrA->tr;
        ptrA->tr = elemTr;
    }
}

/*
*   PrintGraph ­ Вывод графа
*	Входные параметры:
*	vertexes ­ список вершин графа
*/
void PrintGraph(Lider* vertexes) {
    if( vertexes == NULL){
        cout << "Graph is empty" << '\n';
    }
    while(vertexes){
        Trailer* ptrV = vertexes->tr;
        while(ptrV){
            cout << "(" << vertexes->key << "," << ptrV->ld->key << ")" << '\n';
            ptrV = ptrV->nx;
        }
        vertexes = vertexes->nx;
    }
}

/*
* 	DeleteGraph ­ удаление графа
*	Входные параметры:
*	vertexes ­ список вершин графа
*/
void DeleteGraph(Lider*& vertexes) {
    while (vertexes) {
        while (vertexes->tr) {
            Trailer* tmp = vertexes->tr;
            vertexes->tr = vertexes->tr->nx;
            delete tmp;
        }
        Lider* tmp = vertexes;
        vertexes = vertexes->nx;
        delete tmp;
    }

}

/*
*	ExcludeEdge ­ Функция удаления ребра из графа
*	Входные пареметры:
*	vertexes ­ список вершин графа
*	Ребро графа определяется парой вершин (vertexA,vertexB)
*	Возвращаемое значение:
*	true  ­ ребро удалено
*	false ­ ребро не удалено
*/
bool ExcludeEdge(Lider*& vertexes, int vertexA, int vertexB) {
    Lider* ptrA = NULL, * ptrB = NULL;
    for (ptrA = vertexes; ptrA && ptrA->key != vertexA; ptrA = ptrA->nx);
    for (ptrB = vertexes; ptrB && ptrB->key != vertexB; ptrB = ptrB->nx);
    if (ptrA && ptrA->tr && ptrB) {
        if (ptrA->tr->ld == ptrB) {
            Trailer* t = ptrA->tr;
            ptrA->tr = ptrA->tr->nx;
            delete t;
            ptrB->count--;

        }
        else {
            Trailer* trTmp;
            for (trTmp = ptrA->tr; trTmp->nx && trTmp->nx->ld != ptrB; trTmp = trTmp->nx);
            if (trTmp->nx) {
                Trailer* t = trTmp->nx;
                trTmp->nx = trTmp->nx-> nx;
                delete t;
                ptrB->count--;
            }
        }
        return true;
    }
    return false;
}


/*
*	delete_input_arc ­ Функция удаления всех входящих дуг
*	Входные пареметры:
*	graph ­ список вершин графа
*	Возвращаемое значение:
*	true  ­ все дуги удалены
*	false ­ дуги не удалены
*/
void delete_input_arc(Lider*& graph1, int Vx){
    //проверяем, передали мы граф в нашу функцию или нет, чтобы пользователь понял ошибку
    if(graph1 == NULL){
        std::cout << "Graph isn't exist" << '\n';
    }
    Lider* graph = graph1;
    //Идем по всем врешинам графа, и у каждой вершины просматриваем последователей
    //если в trailer(список последователей) найдется нужная нам вершина, то мы приравниваем к Null
    while(graph){
        Trailer* temp = graph->tr;  // обращаемся к списку исходящих вершин(последователей)
        //идем по списку последователей
        while (temp){
            if(temp->ld->key == Vx){
                temp->ld->count = temp->ld->count--;
                temp->ld = NULL; // когда найдем нужную нам вершину, то удаляем ее
                break;  //выход из цикла, так как других входящих ребер нет
            }
            else{
                    temp = temp->nx;
            }
        }
        graph = graph->nx;
    }
    /*while (graph)
    {
        Trailer* temp_tr_pred = NULL,
                *temp_tr_cur = graph->tr;
        while (temp_tr_cur)
        {

            if (temp_tr_cur->ld->key == Vx)
            {
                if (!temp_tr_pred)
                {
                    graph->tr = temp_tr_cur->nx;
                    delete temp_tr_cur;
                    break;
                }

                temp_tr_pred->nx = temp_tr_cur->nx;
                delete temp_tr_cur;
                break;
            }
            temp_tr_pred = temp_tr_cur;
            temp_tr_cur = temp_tr_cur->nx;
        }
        graph = graph->nx;
    }*/
}

bool find_posledovatel(Lider*& graph, int Vx, int Vy){
    //проверяем, передали мы граф в нашу функцию или нет, чтобы пользователь понял ошибку
    if(graph == NULL){
        std::cout << "Graph isn't exist" << '\n';
    }
    Lider* ptrA = NULL, *ptrB = NULL;
    for (ptrA = graph; ptrA && ptrA->key != Vx; ptrA = ptrA->nx);
    for (ptrB = graph; ptrB && ptrB->key != Vy; ptrB = ptrB->nx);

    Trailer* temp = ptrA->tr;
    std::vector<int> trailerA;
    std::vector<int> trailerB;
    Trailer* temp1 = ptrB->tr;
    int res = 0;

    while (temp) {
        res = temp->ld->key;
        trailerA.push_back(res);
        temp = temp->nx;
    }

    while (temp1) {
        res = temp1->ld->key;
        trailerB.push_back(res);
        temp1 = temp1->nx;
    }
    for(auto i : trailerA){
        for(auto j : trailerB){
            if(i == j){
                std::cout << i << '\n';
                return true;
            }
        }
    }
    return false;
}

bool find_predecessor(Lider*& graph, int Vx, int Vy, int Vz){
    //проверяем, передали мы граф в нашу функцию или нет, чтобы пользователь понял ошибку
    if(graph == NULL){
        std::cout << "Grapg isn't exist" << '\n';
    }

    Lider* ptr_graph = graph;   // создаем новый граф, чтобы не повредить исходный
    int res = 0;
    //Идем по всем врешинам графа, и у каждой вершины просматриваем последователей
    //если в trailer(список последователей) найдутся все три вершины, то нашли нужную вершину.
    while(ptr_graph){
        Trailer* temp = ptr_graph->tr;
        //идем по списку последователей и ищем предшественника
        while (temp){
            //проверяем, чтобы у последователя были все предшественники
            if(temp->ld->key == Vz){
                res++;
            }
            if(temp->ld->key == Vy){
                res++;
            }
            if(temp->ld->key == Vx){
                res++;
            }
            temp = temp->nx;
        }
        if(res == 3){
            std::cout << "Vertex = " << ptr_graph->key << '\n';
            return true;
        }
        res = 0;
        ptr_graph = ptr_graph->nx;
    }

    return false;
}

bool check_cycle(Lider *&LD, int Vx, int Vy, int Vz){
    if (LD == NULL) {//если графа нет то выход
        std::cout << "Grapg isn't exist" << '\n';
        return 0;
    }
    Lider *current = NULL;
    Trailer *tmptr = NULL;
    //проверим существование ребра (Vx,Vy)
    for (current = LD; current && (current->key != Vx); current = current->nx);//прошлись по лидерам нашли начало дуги если нет то следующее условие отловит это

    if (current == NULL) {
        return 0;
    }

    for (tmptr = current->tr; (tmptr) && (tmptr->ld->key != Vy); tmptr = tmptr->nx);// прошлись по трейлерам вершины нашли конец дуги есил нет то следующее условие отловит это

    if (tmptr == NULL) {
        return 0;
    }
    //проверим существование ребра (Vy,Vz)
    for (current = LD; current && (current->key != Vy); current = current->nx);

    if (current == NULL) {
        return 0;
    }
    for (tmptr = current->tr; (tmptr) && (tmptr->ld->key != Vz); tmptr = tmptr->nx);

    if (tmptr == NULL) {
        return 0;
    }

    //проверим существование ребра (Vz,Vx)
    for (current = LD; current && (current->key != Vz); current = current->nx);

    if (current == NULL) {
        return 0;
    }
    for (tmptr = current->tr; (tmptr) && (tmptr->ld->key != Vx); tmptr = tmptr->nx);

    if (tmptr == NULL) {
        return 0;
    }

    return 1;// т.к есть все 3 дуги то  и есть цикл (Vx,Vy)(Vy,Vz)(Vz,Vx) вернуть 1
}



int main() {

    ifstream cin("graph.txt");
    Lider* graph = NULL;
    int vertexA = 0, vertexB = 0;
    int k = 0;

    if (!cin.eof())
    {
        while (!cin.eof()) {
            cin >> vertexA >> vertexB;
            printf("(%d,%d)\n", vertexA, vertexB);
            AddEdge(graph, vertexA, vertexB);
        }
        //delete_input_arc(graph,5);
        std::cout << check_cycle(graph, 6, 2, 1) << '\n';
        std::cout << "Print Graph" << '\n';
        PrintGraph(graph);
        /*ExcludeEdge(graph, 1, 5);
        ExcludeEdge(graph, 4, 5);
        
        if (!ExcludeEdge(graph, 5, 3))
        {
            cout << "Not del" << endl;
        }

        PrintGraph(graph);
        DeleteGraph(graph);
        PrintGraph(graph);*/
    }
    else {
        puts("File is not open");
    }

    return 0;
}