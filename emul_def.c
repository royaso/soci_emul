#include "emul_def.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


/**
 * global stat
 */
static unsigned long m_current_year;
static unsigned long m_um_male_size;
static unsigned long m_um_female_size;
static unsigned long m_fa_male_size;
static unsigned long m_fa_female_size;
static unsigned long m_couple_size;
static unsigned long m_couple_male_size;
static unsigned long m_couple_female_size;
static unsigned long m_all_fa_male_size;
static unsigned long m_all_fa_female_size;
static unsigned long m_all_couple_size;
static _person _m_um_male_list;
static _person _m_um_female_list;
static _person _m_fa_male_list;
static _person _m_fa_female_list;
static _couple _m_couple_list;

static unsigned long m_new_boy = 0;
static unsigned long m_new_girl = 0;

/**
 * queues
 */
void person_enqueue(_person *_lst,_person p);
_person person_top(_person lst);
_person person_dequeue(_person *_lst);
void person_destory(_person *_lst);

void couple_enqueue(_couple *_lst,_couple p);
_couple couple_top(_couple lst);
_couple couple_dequeue(_couple *_lst);
void couple_destory(_couple *_lst);

void person_enqueue(_person *_lst,_person p)
{
    if((*_lst) == NULL )
    {
        (*_lst) = p;
        (*_lst)->next = NULL;
    }else
    {
        _person tp = (*_lst);
        while(tp->next != NULL) tp = tp->next;
        tp->next = p;
        tp = tp->next;
        tp->next = NULL;
    }
}

_person person_top(_person lst)
{
    if(lst == NULL) return NULL;
    return lst;
}

_person person_dequeue(_person *_lst)
{
    if(*_lst == NULL) return NULL;
    _person _top = (*_lst);
    (*_lst) = (*_lst)->next;
    return _top;
}

void person_destory(_person *_lst)
{
    _person p1 = (*_lst);
    (*_lst) = NULL;
    while(p1 != NULL)
    {
        _person p2 = p1;
        p1 = p1->next;
        free(p2);
    }
}

void couple_enqueue(_couple *_lst,_couple p)
{
    if((*_lst) == NULL )
    {
        (*_lst) = p;
        (*_lst)->next = NULL;
    }else
    {
        _couple tp = (*_lst);
        while(tp->next != NULL) tp = tp->next;
        tp->next = p;
        tp = tp->next;
        tp->next = NULL;
    }
}

_couple couple_top(_couple lst)
{
    if(lst == NULL) return NULL;
    return lst;
}

_couple couple_dequeue(_couple *_lst)
{
    if(*_lst == NULL) return NULL;
    _couple _top = (*_lst);
    (*_lst) = (*_lst)->next;
    return _top;
}

void couple_destory(_couple *_lst)
{
    _couple p1 = (*_lst);
    (*_lst) = NULL;
    while(p1 != NULL)
    {
        _couple p2 = p1;
        p1 = p1->next;
        free(p2);
    }
}

/**
 * funs
 */
void emul_birth()
{
    _couple p = _m_couple_list;
    while(p != NULL)
    {
        if(p->hus != NULL && p->wif != NULL)
        {
            if(p->stop != 1 && m_current_year - p->last_birth_year > BRT_MD)
            {
                int n_sex = rand() % 2;
                if(n_sex == 1)
                {
                    m_new_boy ++;
                }else
                {
                    m_new_girl ++;
                }
                p->kid_count ++;
                if(p->kid_count >= 5 || n_sex == 1 ) p->stop = 1;
                _person np = (_person)malloc(sizeof(person));
                np->birth_year = m_current_year;
                np->sex = n_sex;
                if(n_sex == 1) // male
                {
                    person_enqueue(&_m_um_male_list,np);
                    m_um_male_size ++;
                }else
                {
                    person_enqueue(&_m_um_female_list,np);
                    m_um_female_size ++;
                }
            }
        }
        p = p->next;
    }
}

#define person_age(p) (p == NULL ? 0 : m_current_year - p->birth_year)
#define person_should_die(p) ( person_age(p) > MAX_AGE ? 1 : 0 )


void emul_death()
{
    _couple tcp = _m_couple_list;
    while(tcp != NULL)
    {
        if(person_should_die(tcp->hus))
        {
            free(tcp->hus);
            tcp->hus = NULL;
            m_couple_male_size -- ;
        }
        if(person_should_die(tcp->wif))
        {
            free(tcp->wif);
            tcp->wif = NULL;
            m_couple_female_size --;
        }
        tcp = tcp->next;
    }
    while(couple_top(_m_couple_list) != NULL)
    {
        if(couple_top(_m_couple_list)->hus == NULL && couple_top(_m_couple_list)->wif == NULL)
        {
            _couple fcp = couple_dequeue(&_m_couple_list);
            free(fcp);
            m_couple_size -- ;
        }else
        {
            break;
        }
    }
    while(person_should_die(person_top(_m_fa_female_list)))
    {
        _person fap = person_dequeue(&_m_fa_female_list);
        free(fap);
        m_fa_female_size -- ;
    }
    while(person_should_die(person_top(_m_fa_male_list)))
    {
        _person fap = person_dequeue(&_m_fa_male_list);
        free(fap);
        m_fa_male_size -- ;
    }
}

void emul_do_mrg()
{
    _couple nc = (_couple)malloc(sizeof(couple));
    nc->hus = person_dequeue(&_m_um_male_list);
    nc->wif = person_dequeue(&_m_um_female_list);
    nc->kid_count = 0;
    nc->stop = 0;
    nc->next = NULL;
    couple_enqueue(&_m_couple_list,nc);
    m_couple_size ++;
    m_couple_male_size ++;
    m_couple_female_size ++ ;
    m_um_male_size --;
    m_um_female_size --;
}

void emul_find_mrg()
{
    while(person_age(person_top(_m_um_male_list)) > MAX_MRG_AGE)
    {
        m_um_male_size --;
        m_fa_male_size ++;
        m_all_fa_male_size ++;
        person_enqueue(&_m_fa_male_list,person_dequeue(&_m_um_male_list));
    }
    while(person_age(person_top(_m_um_female_list)) > MAX_MRG_AGE)
    {
        m_um_female_size --;
        m_fa_female_size ++;
        m_all_fa_female_size ++;
        person_enqueue(&_m_fa_female_list,person_dequeue(&_m_um_female_list));
    }
    while(person_top(_m_um_female_list) != NULL && person_top(_m_um_male_list) != NULL)
    {
        if((person_age(person_top(_m_um_female_list)) >= 20 ) && (person_age(person_top(_m_um_male_list)) > 20 ))
            emul_do_mrg();
        else
            break;
    }
}

/**
 * export
 */

void init_emul(unsigned long start_year,unsigned long male_size,unsigned long female_size)
{
    srand((unsigned int)time(NULL)); // make a seed
    m_current_year = start_year ;
    m_um_male_size = 0;
    m_um_female_size = 0;
    m_fa_male_size = 0;
    m_fa_female_size = 0;
    m_couple_size = 0;
    m_couple_male_size = 0;
    m_couple_female_size = 0;
    m_all_fa_male_size = 0;
    m_all_fa_female_size = 0;
    m_all_couple_size = 0;
    _m_um_male_list = NULL;
    _m_um_female_list = NULL;
    _m_fa_male_list = NULL;
    _m_fa_female_list = NULL;
    _m_couple_list = NULL;
    int i ;
    for(i = 0 ; i < male_size ; i ++)
    {
        _person mp = (_person)malloc(sizeof(person));
        mp->birth_year = m_current_year;
        mp->sex = 1;
        mp->next = NULL;
        person_enqueue(&_m_um_male_list,mp);
    }
    m_um_male_size += male_size;
    for(i = 0 ; i < female_size ; i ++ )
    {
        _person mp = (_person)malloc(sizeof(person));
        mp->birth_year = m_current_year;
        mp->sex = 0;
        mp->next = NULL;
        person_enqueue(&_m_um_female_list,mp);
    }
    m_um_female_size += female_size;

}

void next_year(void)
{
    m_current_year ++ ;
    emul_death();
    emul_birth();
    emul_find_mrg();
}

void show_stat(void)
{
    printf("YEAR : %ld \n",m_current_year);
    printf("unmarried male : %ld , female : %ld\n",m_um_male_size,m_um_female_size);
    printf("married male : %ld , female : %ld\n",m_couple_male_size,m_couple_female_size);
    printf("FA male : %ld , female : %ld\n",m_fa_male_size,m_fa_female_size);
    unsigned long all_male_size = 0;
    unsigned long all_female_size = 0;
    all_male_size = m_um_male_size + m_couple_male_size + m_fa_male_size;
    all_female_size = m_um_female_size + m_couple_female_size + m_fa_female_size;
    printf("all people male :%ld , female : %ld \n",all_male_size,all_female_size);
    printf("new boy : %ld , new girl : %ld\n",m_new_boy,m_new_girl);
    printf("history FA male : %ld , female : %ld\n",m_all_fa_male_size,m_all_fa_female_size);
    printf("-------------------------------------\n");
}

void destory_emul(void)
{
    person_destory(&_m_um_male_list);
    person_destory(&_m_um_female_list);
    person_destory(&_m_fa_male_list);
    person_destory(&_m_fa_female_list);
    couple_destory(&_m_couple_list);
    m_um_male_size = 0;
    m_um_female_size = 0;
    m_fa_male_size = 0;
    m_fa_female_size = 0;
    m_couple_size = 0;
    m_all_fa_male_size = 0;
    m_all_fa_female_size = 0;
    m_all_couple_size = 0;
}
