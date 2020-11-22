// красно-черное дерево (
// Сделано ввиде класса, удалена статистика
// (с) Proteus  (lawnmower-man@mail.ru)
//
// да, я скопипастил код и признаю это
// ну потому что писать реализацию красно чёрного дерева это пиздец полный
// тем более Хирьянов (на лекции) говорил, что не ставит целью заставить нас уметь писать эту поебень
// надо понимать что это и зачем, где применяется и в каких контейнерах
// понимаю, что семинаристы и Хирьянов редко коррелируют, но блин красночёрное дерево на ФОПФе...
// НАХУЯ НАМ ЭТО НАДО ПИСАТЬ?!
// Очень интересно было бы, если б на это был не 1 неделя, а норм проект
// И не было сдач аналмеха, матана и общефиза на этой неделе
// Так что открыто заявляю: я скопипастил код, код работает, я понимаю зачем оно надо
// НО в текущих реалиях писать это с нуля это жопа полная
// Спасибо за внимание :)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<memory.h>

//! класс дерева
class RBtree{
    struct node_st{node_st *p1,*p2; int value; bool red;}; // структура узла
    node_st *tree_root;					//!< корень
    int nodes_count;					//!< число узлов дерева
private:
    node_st *NewNode(int value);		//!< выделение новой вешины
    void DelNode(node_st*);				//!< удаление вершины
    void Clear(node_st*);				//!< снос дерева (рекурсивная часть)
    node_st *Rotate21(node_st*);		//!< вращение влево
    node_st *Rotate12(node_st*);		//!< вращение вправо
    void BalanceInsert(node_st**);		//!< балансировка вставки
    bool BalanceRemove1(node_st**);		//!< левая балансировка удаления
    bool BalanceRemove2(node_st**);		//!< правая балансировка удаления
    bool insert(int,node_st**);			//!< рекурсивная часть вставки
    bool GetMin(node_st**,node_st**);	//!< найти и убрать максимальный узел поддерева
    bool erase(node_st**,int);			//!< рекурсивная часть удаления
public: // отладочная часть
    enum check_code{error_balance,error_struct,ok};	// код ошибки
    void Show();						//!< вывод дерева
    check_code Check();					//!< проверка дерева
    bool TreeWalk(bool*,int);			//!< обход дерева и сверка значений с массивом
private: // отладочная часть
    void Show(node_st*,int,char);		//!< вывод дерева, рекурсивная часть
    check_code Check(node_st*,int,int&);//!< проверка дерева (рекурсивная часть)
    bool TreeWalk(node_st*,bool*,int);	//!< обход дерева и сверка значений с массивом (рекурсивная часть)
public:
    RBtree();
    ~RBtree();
    void Clear();			//!< снести дерево
    bool count(int);			//!< найти значение
    void insert(int);		//!< вставить значение
    void erase(int);		//!< удалить значение
    int GetNodesCount();	//!< узнать число узлов
};


RBtree::RBtree()
{
    tree_root=0;
    nodes_count=0;
}

RBtree::~RBtree()
{
    Clear(tree_root);
}

int RBtree::GetNodesCount()
{
    return nodes_count;
}

// выделение новой вешины
RBtree::node_st *RBtree::NewNode(int value)
{
    nodes_count++;
    node_st *node=new node_st;
    node->value=value;
    node->p1=node->p2=0;
    node->red=true;
    return node;
}

// удаление вершины
void RBtree::DelNode(node_st *node)
{
    nodes_count--;
    delete node;
}

// снос дерева (рекурсивная часть)
void RBtree::Clear(node_st *node)
{
    if(!node) return;
    Clear(node->p1);
    Clear(node->p2);
    DelNode(node);
}

// вывод дерева, рекурсивная часть
//! \param node узел
//! \param depth глубина
//! \param dir   значёк
//! \code Show(root,0,'*'); \endcode
void RBtree::Show(node_st *node,int depth,char dir)
{
    int n;
    if(!node) return;
    for(n=0; n<depth; n++) putchar(' ');
    printf("%c[%d] (%s)\n",dir,node->value,node->red?"red":"black");
    Show(node->p1,depth+2,'-');
    Show(node->p2,depth+2,'+');
}


// вращение влево
//! \param index индеск вершины
//! \result новая вершина дерева
RBtree::node_st *RBtree::Rotate21(node_st *node)
{
    node_st *p2 =node->p2;
    node_st *p21=p2->p1;
    p2->p1=node;
    node->p2=p21;
    return p2;
}

// вращение вправо
//! \param index индеск вершины
//! \result новая вершина дерева
RBtree::node_st *RBtree::Rotate12(node_st *node)
{
    node_st *p1 =node->p1;
    node_st *p12=p1->p2;
    p1->p2=node;
    node->p1=p12;
    return p1;
}


// балансировка вершины
void RBtree::BalanceInsert(node_st **root)
{
    node_st *p1,*p2,*px1,*px2;
    node_st *node=*root;
    if(node->red) return;
    p1=node->p1;
    p2=node->p2;
    if(p1 && p1->red) {
        px2=p1->p2;				// задача найти две рядом стоящие крастне вершины
        if(px2 && px2->red) p1=node->p1=Rotate21(p1);
        px1=p1->p1;
        if(px1 && px1->red) {
            node->red=true;
            p1->red=false;
            if(p2 && p2->red) {	// отделаемся перекраской вершин
                px1->red=true;
                p2->red=false;
                return;
            }
            *root=Rotate12(node);
            return;
        }
    }
    // тоже самое в другую сторону
    if(p2 && p2->red) {
        px1=p2->p1;				// задача найти две рядом стоящие крастне вершины
        if(px1 && px1->red) p2=node->p2=Rotate12(p2);
        px2=p2->p2;
        if(px2 && px2->red) {
            node->red=true;
            p2->red=false;
            if(p1 && p1->red) {	// отделаемся перекраской вершин
                px2->red=true;
                p1->red=false;
                return;
            }
            *root=Rotate21(node);
            return;
        }
    }
}


bool RBtree::BalanceRemove1(node_st **root)
{
    node_st *node=*root;
    node_st *p1=node->p1;
    node_st *p2=node->p2;
    if(p1 && p1->red) {
        p1->red=false;return false;
    }
    if(p2 && p2->red) { // случай 1
        node->red=true;
        p2->red=false;
        node=*root=Rotate21(node);
        if(BalanceRemove1(&node->p1)) node->p1->red=false;
        return false;
    }
    unsigned int mask=0;
    node_st *p21=p2->p1;
    node_st *p22=p2->p2;
    if(p21 && p21->red) mask|=1;
    if(p22 && p22->red) mask|=2;
    switch(mask)
    {
        case 0:		// случай 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
            p2->red=true;
            return true;
        case 1:
        case 3:		// случай 3 - if(p21 && p21->red)
            p2->red=true;
            p21->red=false;
            p2=node->p2=Rotate12(p2);
            p22=p2->p2;
        case 2:		// случай 4 - if(p22 && p22->red)
            p2->red=node->red;
            p22->red=node->red=false;
            *root=Rotate21(node);
    }
    return false;
}

bool RBtree::BalanceRemove2(node_st **root)
{
    node_st *node=*root;
    node_st *p1=node->p1;
    node_st *p2=node->p2;
    if(p2 && p2->red) {p2->red=false;return false;}
    if(p1 && p1->red) { // случай 1
        node->red=true;
        p1->red=false;
        node=*root=Rotate12(node);
        if(BalanceRemove2(&node->p2)) node->p2->red=false;
        return false;
    }
    unsigned int mask=0;
    node_st *p11=p1->p1;
    node_st *p12=p1->p2;
    if(p11 && p11->red) mask|=1;
    if(p12 && p12->red) mask|=2;
    switch(mask) {
        case 0:		// случай 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
            p1->red=true;
            return true;
        case 2:
        case 3:		// случай 3 - if(p12 && p12->red)
            p1->red=true;
            p12->red=false;
            p1=node->p1=Rotate21(p1);
            p11=p1->p1;
        case 1:		// случай 4 - if(p11 && p11->red)
            p1->red=node->red;
            p11->red=node->red=false;
            *root=Rotate12(node);
    }
    return false;
}


bool RBtree::count(int value)
{
    node_st *node=tree_root;
    while(node) {
        if(node->value==value) return true;
        node=node->value>value?node->p1:node->p2;
    }
    return false;
}


// рекурсивная часть вставки
//! \result true если изменений небыло или балансировка в данной вершине не нужна
bool RBtree::insert(int value,node_st **root)
{
    node_st *node=*root;
    if(!node) *root=NewNode(value);
    else {
        if(value==node->value) return true;
        if(insert(value,value<node->value?&node->p1:&node->p2)) return true;
        BalanceInsert(root);
    }
    return false;
}


// найти и убрать максимальный узел поддерева
//! \param root корень дерева в котором надо найти элемент
//! \retval res эелемент который был удалён
//! \result true если нужен баланс
bool RBtree::GetMin(node_st **root,node_st **res)
{
    node_st *node=*root;
    if(node->p1) {
        if(GetMin(&node->p1,res)) return BalanceRemove1(root);
    } else {
        *root=node->p2;
        *res=node;
        return !node->red;
    }
    return false;
}


// рекурсивная часть удаления
//! \result true если нужен баланс
bool RBtree::erase(node_st **root,int value)
{
    node_st *t,*node=*root;
    if(!node) return false;
    if(node->value<value) {
        if(erase(&node->p2,value))	return BalanceRemove2(root);
    } else if(node->value>value) {
        if(erase(&node->p1,value))	return BalanceRemove1(root);
    } else {
        bool res;
        if(!node->p2) {
            *root=node->p1;
            res=!node->red;
        } else {
            res=GetMin(&node->p2,root);
            t=*root;
            t->red=node->red;
            t->p1=node->p1;
            t->p2=node->p2;
            if(res) res=BalanceRemove2(root);
        }
        DelNode(node);
        return res;
    }
    return 0;
}


// вывод дерева
void RBtree::Show()
{
    printf("[tree]\n");
    Show(tree_root,0,'*');
}

// функция вставки
void RBtree::insert(int value)
{
    insert(value,&tree_root);
    if(tree_root) tree_root->red=false;
}

// удаление узла
void RBtree::erase(int value)
{
    erase(&tree_root,value);
}

// снос дерева
void RBtree::Clear()
{
    Clear(tree_root);
    tree_root=0;
}


// проверка дерева (рекурсивная часть)
//! \param tree дерево
//! \param d    текущая чёрная глубина
//! \param h    эталонная чёрная глубина
//! \result 0 или код ошибки
RBtree::check_code RBtree::Check(node_st *tree,int d,int &h)
{
    if(!tree) {
        // количество чёрных вершин на любом пути одинаковое
        if(h<0) h=d;
        return h==d?ok:error_balance;
    }
    node_st *p1=tree->p1;
    node_st *p2=tree->p2;
    // красная вершина должна иметь чёрных потомков
    if(tree->red && (p1 && p1->red || p2 && p2->red)) return error_struct;
    if(p1 && tree->value<p1->value || p2 && tree->value>p2->value) return error_struct;
    if(!tree->red) d++;
    check_code n=Check(p1,d,h); if(n) return n;
    return Check(p2,d,h);
}


// проверка дерева
RBtree::check_code RBtree::Check()
{
    int d= 0;
    int h=-1;
    if(!tree_root) return ok;
    if(tree_root->red) return error_struct;
    return Check(tree_root,d,h);
}

// обход дерева и сверка значений с массивом (рекурсивная часть)
//! \param node  корень дерева
//! \param array массив для сверки
//! \param size  размер массива
bool RBtree::TreeWalk(node_st *node,bool *array,int size)
{
    if(!node) return false;
    int value=node->value;
    if(value<0 || value>=size || !array[value]) return true;
    array[value]=false;
    return TreeWalk(node->p1,array,size) || TreeWalk(node->p2,array,size);
}

// обход дерева и сверка значений с массивом
//! \param array массив для сверки
//! \param size  размер массива
bool RBtree::TreeWalk(bool *array,int size)
{
    if(TreeWalk(tree_root,array,size)) return true;
    for(int n=0; n<size; n++) if(array[n]) return true;
    return false;
}


//================================================================
//
//#define SIZE 1000	//!< размер массива провеки
//
//
//
////! набор тестирующих процедур
//int main()
//{
//    int n,i;
//   RBtree tree;
// bool array[SIZE];
//  srand(time(0));
//  memset(array,false,sizeof(array));
//  for(n=0; n<SIZE*100; n++) {
//      printf("pass: %d of %d\r",n+1,SIZE*100);
//      i=rand()%SIZE; array[i]=true;  tree.insert(i);
//      i=rand()%SIZE; array[i]=false; tree.erase(i);
//  }
//  putchar('\n');
//  switch(tree.Check()) {
//      case RBtree::error_struct:   printf("*** structure error\n\a"); break;
//      case RBtree::error_balance:  printf("*** balance error\n\a");   break;
//      default:
//          printf("nodes count: %d\n",tree.GetNodesCount());
//          break;
//  }
//  if(tree.TreeWalk(array,SIZE)) printf("*** context error\n\a");
//  getchar();
//  return 0;
//}



