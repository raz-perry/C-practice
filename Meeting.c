//
// Created by Raz on 19/11/2020.
//

#include "Meeting.h"
#include <stdbool.h>

/**
 * Allocating (dynamically) new meeting with (at least) the following
 * input data:
 * @param person_1 (struct Person *) pointer to the first person in the meeting.
 * @param person_2 (struct Person *) pointer to the second person in the meeting.
 * @param measure (double) the time of the meeting in minutes.
 * @param distance (double) the distance the two people where in.
 * @return (struct Meeting *) pointer to dynamically allocated meeting.
 * @if_fails returns NULL.
 * @assumption the inputs would be valid.
 */
Meeting *MeetingAlloc(Person *person_1, Person *person_2, double measure, double distance){
    Meeting *newMeeting = malloc(sizeof(Meeting));
    if (!newMeeting){
        return NULL;
    }
    newMeeting->person_1 = person_1;
    newMeeting->person_2 = person_2;
    newMeeting->measure = measure;
    newMeeting->distance = distance;
    return newMeeting;
}

/**
 * Frees everything the meeting has allocated and the pointer itself.
 * @param p_meeting (struct Meeting **) pointer to dynamically allocated meeting.
 * @assumption you can not assume anything.
 */
void MeetingFree(Meeting **p_meeting){
    if (!p_meeting){
        return;
    }
    free(*p_meeting);
    *p_meeting = NULL;
}

/**
 * Returns a pointer to one of the persons in the meeting.
 * @param meeting (struct Meeting *) the meeting we would like to
 * get its person.
 * @param person_ind (size_t) the index of the person we
 * want (can be either 1 or 2).
 * @return (struct Person *) pointer to the person we want.
 * person_ind == 1 ==> return person_1 (according the person_1 given in
 * MeetingAlloc).
 * person_ind == 2 ==> return person_2 (according to the person_2 given in
 * MeetingAlloc).
 * @if_failds return NULL
 * @assumption you can not assume anything.
 */
Person *MeetingGetPerson(const Meeting * const meeting, size_t person_ind){
    if (!meeting){
        return NULL;
    }
    // todo - meeting unvalid - only one person isn't null - return null?
    if (person_ind == 1){
        return meeting->person_1;
    }
    if (person_ind == 2){
        return meeting->person_2;
    }
    return NULL;
}
