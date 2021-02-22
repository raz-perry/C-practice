//
// Created by Raz on 19/11/2020.
//

#include "Person.h"
#include <stdbool.h>


/**
 * Allocates dynamically new person with (at least) the following
 * input data:
 * @param id (IdT) the id of the person.
 * @param name (char *) the name of the person (received as string) (ex: "Moshe Stam-shem").
 * @param age (size_t) the age of the person.
 * @param is_sick (int) boolean value (0/1) which indicates if the person is sick.
 * @return (struct Person *) pointer to dynamically allocated person,
 * @if_fails returns NULL.
 * @assumption the input would be valid.
 */
Person *PersonAlloc(IdT id, char *name, size_t age, int is_sick){
    Person *newPerson = calloc(1, sizeof(Person));
    if (newPerson){
        newPerson->id = id;
        newPerson->name = name;
        newPerson->age = age;
        newPerson->is_sick = is_sick;
        if (is_sick) newPerson->infection_rate = 1;
    }
    return newPerson;
}

/**
 * Frees everything the person has allocated and the pointer itself.
 * @param p_person (struct Person **) pointer to dynamically allocated person.
 * @assumption you can not assume anything.
 */
void PersonFree(Person **p_person){
    if (!p_person || !(*p_person)){
        return;
    }
    free((*p_person)->meetings);
    free(*p_person);
    *p_person = NULL;
}

/**
 * Returns a pointer to the meeting with the given person (the person who has the given ID).
 * @param person (struct Person *) the person we would like to get his/her meeting.
 * @par id (IdT) the id of the person she/he met with.
 * @return (Meeting *) a pointer to meeting, returns NULL
 * if no such exists.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
Meeting *PersonGetMeetingById(const Person *const person, IdT id){
    if (!person){
        return NULL;
    }
    Meeting **personMeetings = person->meetings;
    for (size_t i = 0; i < person->num_of_meetings; ++i) {
        if (personMeetings[i]->person_2->id == id){
            return personMeetings[i];
        }
    }
    return NULL;
}


// comparators:
/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareById(const Person *person_1, const Person *person_2){
    if (person_1->id < person_2->id){
        return -1;
    }
    return person_1->id != person_2->id;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByName(const Person *person_1, const Person *person_2){
    int res = strcmp(person_1->name, person_2->name);
    if (res < 0) return -1;
    return res != 0;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByInfectionRate(const Person *person_1, const Person *person_2){
    if (person_1->infection_rate > person_2->infection_rate){
        return -1;
    }
    return person_1->infection_rate != person_2->infection_rate;
}

/**
 * The function is used to compare people.
 * @param person_1 (struct Person *) person we would like to compare.
 * @param person_2 (struct Person *) person we would like to compare.
 * @return 0 if the two people are to be considered equal,
 *        -1 if person_1 should be before person_2.
 *         1 if person_1 should be after person_2.
 * @if_fails can not fail.
 * @assumption the input would be valid (pointers to existing people).
 */
int PersonCompareByAge(const Person *person_1, const Person *person_2){
    if (person_1->age > person_2->age){
        return -1;
    }
    return person_1->age != person_2->age;
}