#ifndef __EMUL_DEF_H__
#define __EMUL_DEF_H__

typedef struct person
{
    int birth_year;
    int sex;
    struct person * next;
}person,* _person;

typedef struct couple
{
    _person hus;
    _person wif;
    int last_birth_year;
    int kid_count;
    int stop;
    struct couple * next;
}couple,* _couple;

#define MAX_MRG_AGE 45 //最大婚龄
#define MAX_AGE 60
#define BRT_MD 2 //  生育间隔


void init_emul(unsigned long start_year,unsigned long male_size,unsigned long female_size);

void next_year(void);
void show_stat(void);
void destory_emul(void);

#endif // __EMUL_DEF_H__
