//
// Created by Raz on 19/11/2020.
//


#include "SpreaderDetector.h"
#include <stdbool.h>


int PersonExist(Person *person, Person **people, size_t peopleSize);
int AddMeetingToPerson(Person* person, Meeting* meeting);
Person* GetPersonById(Person** people, size_t size, IdT id);
void UpdateRate(Person* root);
void CalcCrna(Person* person1, Person* person2, double measure, double distance);


/**
 * Allocates (dynamically) a spreader detector with no meetings and
 * no people.
 * @return pointer to dynamically allocated SpreaderDetector struct.
 * @if_fails returns NULL.
 * @assumption you can not assume anything.
 */
SpreaderDetector *SpreaderDetectorAlloc(){
    return calloc(1, sizeof(SpreaderDetector));
}


void SpreaderDetectorFree(SpreaderDetector **p_spreader_detector){
    if (!p_spreader_detector || !(*p_spreader_detector)){
        return;
    }
    free((*p_spreader_detector)->meetings);
    free((*p_spreader_detector)->people);
    free(*p_spreader_detector);
    *p_spreader_detector = NULL;
}

/**
 * Adds the given meeting to the spreader detector.
 * Important - the people in the meeting should exist in the spreader detector.
 * @param spreader_detector the spreader detector we wants to add the meeting to.
 * @param meeting the meeting we wants to add to the spreader detector.
 * @return 1 if the meeting was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddPerson(SpreaderDetector *spreader_detector, Person *person){
    if (!spreader_detector || !person){
        return 0;
    }

    if (PersonExist(person, spreader_detector->people,
                    spreader_detector->people_size) == true) return 0;

    if (spreader_detector->people_size == spreader_detector->people_cap){
        if (spreader_detector->people_cap == 0){
            spreader_detector->people_cap = SPREADER_DETECTOR_INITIAL_SIZE;
        }
        else {
            spreader_detector->people_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
        }

        Person **temp = realloc(spreader_detector->people,
                                spreader_detector->people_cap*sizeof(void *));
        if (!temp) return 0;

        spreader_detector->people = temp;
        temp = NULL;
    }

    spreader_detector->people[spreader_detector->people_size++] = person;
    return 1;
}

/**
 * This function gets person and persons array (with size) and return true if the
 * input person is in the input array
 * @param person Person to find
 * @param people array (pointer) of Person to search in it
 * @param peopleSize size of array
 * @return true if the person exist, false otherwise
 */
int PersonExist(Person *person, Person **people, size_t peopleSize){
    if (!person || !people) return false;
    for (size_t i = 0; i < peopleSize; ++i) {
        if (PersonCompareById(people[i], person) == 0){
            return true;
        }
    }
    return false;
}


/**
 * Adds the given meeting to the spreader detector.
 * Important - the people in the meeting should exist in the spreader detector.
 * @param spreader_detector the spreader detector we wants to add the meeting to.
 * @param meeting the meeting we wants to add to the spreader detector.
 * @return 1 if the meeting was added successfully, 0 otherwise.
 * @if_fails returns 0.
 * @assumption you can not assume anything.
 */
int SpreaderDetectorAddMeeting(SpreaderDetector *spreader_detector, Meeting *meeting){
    // validation
    if (!spreader_detector || !meeting || !spreader_detector->people){
        return 0;
    }
    if (PersonExist(meeting->person_1, spreader_detector->people,
                    spreader_detector->people_size) == false) return 0;

    if (PersonExist(meeting->person_2, spreader_detector->people,
                    spreader_detector->people_size) == false) return 0;

    // person1 has this meeting:
    if (meeting == PersonGetMeetingById(meeting->person_1, meeting->person_2->id)) return 0;

    // realoc meetings
    if (spreader_detector->meeting_size == spreader_detector->meeting_cap){
        if (spreader_detector->meeting_cap == 0){
            spreader_detector->meeting_cap = SPREADER_DETECTOR_INITIAL_SIZE;
        }
        else {
            spreader_detector->meeting_cap *= SPREADER_DETECTOR_GROWTH_FACTOR;
        }

        Meeting **temp = realloc(spreader_detector->meetings,
                                spreader_detector->meeting_cap*sizeof(void *));
        if (!temp) return 0;

        spreader_detector->meetings = temp;
        temp = NULL;
    }

    if (AddMeetingToPerson(meeting->person_1, meeting) == false) return 0;
    spreader_detector->meetings[spreader_detector->meeting_size++] = meeting;
    return 1;

}

/**
 * This function gets person and meeting (in which he is person 1) and update
 * that meeting to the person meetings
 * @param person the person to update its meetings
 * @param meeting Meeting to add
 * @return true if the add succeed, false otherwise
 */
int AddMeetingToPerson(Person* person, Meeting* meeting){
    if (!person->meetings){
        person->meetings = malloc(sizeof(void*));
        if (!person->meetings) return false;
    }
    person->meetings[person->num_of_meetings++] = meeting;
    return true;
}


/**
 * This function reads the file of the meeting, parses to file into meetings,
 * and inserts it to the spreader detector.
 * @param spreader_detector the spreader detector we wants to read the meetings into.
 * @param path the path to the meetings file.
 * @assumption you can assume that the path to the file is ok (and anything but that).
 */
void SpreaderDetectorReadMeetingsFile(SpreaderDetector *spreader_detector, const char *path){
    FILE *file = fopen(path, "r"); // open the given file
    if (!file){ // check the file opened correctly
        return;
    }
    char buffer[MAX_LEN_OF_LINE];
    while (fgets(buffer, MAX_LEN_OF_LINE, file)) {
        IdT id1, id2;
        double distance, measure;
        sscanf(buffer, "%zd %zd %lf %lf", &id1, &id2, &distance, &measure);
        Person* p1 = GetPersonById(spreader_detector->people, spreader_detector->people_size, id1);
        Person* p2 = GetPersonById(spreader_detector->people, spreader_detector->people_size, id2);
        // todo - id doewn't exist - person is null
        // todo - measure and distance - 0? min and max
        // todo - if meeting exist - continue? return?
        Meeting* meeting = MeetingAlloc(p1, p2, measure, distance); // todo - free
        if (!meeting){
            fclose(file);
            return;
        }

        if (!SpreaderDetectorAddMeeting(spreader_detector, meeting)){
            MeetingFree(&meeting);
            fclose(file);
            return;
        }
    }
}

/**
 * This function search the input id in the input people array, if found it
 * returns the relevant Person
 * @param people array of Person to search in it
 * @param size size of array (pointer)
 * @param id the person id we are looking for
 * @return Person with the input id if exist, otherwise NULL
 */
Person* GetPersonById(Person** people, size_t size, IdT id){
    for (size_t i = 0; i < size; ++i) {
        if (people[i]->id == id){
            return people[i];
        }
    }
    return NULL;
}

/**
 * Returns the infection rate of the person with the given id.
 * @param spreader_detector the spreader detector contains the person.
 * @param id the id of the person we are looking for.
 * @return the infection rate of the person, if not person exists -
 * returns -1.
 * @if_fails returns -1.
 * @assumption you can assume anything.
 */
void SpreaderDetectorReadPeopleFile(SpreaderDetector *spreader_detector, const char *path){
    // todo - detector should be null? otherwise false?
    // todo - if error in line 5 - return spreader with 4? or zero?
    FILE *file = fopen(path, "r"); // todo - free
    if (!file){
        return;
    }
    char buffer[MAX_LEN_OF_LINE];
    while (fgets(buffer, MAX_LEN_OF_LINE, file)) {
        char name[MAX_LEN_OF_LINE], sick[6]; // todo - good name?
        IdT id;
        size_t age;
        sscanf(buffer, "%s %zd %zd %s", name, &id, &age, sick);
        char * pName = malloc(strlen(name)-1); // todo - free
        if (!pName){
            fclose(file);
            return;
        }
        strcpy(pName, name); // todo - needed?
        int sickVal = strcmp(sick, "SICK")==0 ? 1 : 0;
        Person* person = PersonAlloc(id, pName, age, sickVal); // todo - free
        if (!person){
            free(pName);
            fclose(file);
            return;
        }
        if (!SpreaderDetectorAddPerson(spreader_detector, person)){
            PersonFree(&person);
            fclose(file);
            return;
        }
    }
}

/**
 * Returns the infection rate of the person with the given id.
 * @param spreader_detector the spreader detector contains the person.
 * @param id the id of the person we are looking for.
 * @return the infection rate of the person, if not person exists -
 * returns -1.
 * @if_fails returns -1.
 * @assumption you can assume anything.
 */
double SpreaderDetectorGetInfectionRateById(SpreaderDetector *spreader_detector, IdT id){
    if (!spreader_detector || !spreader_detector->people){
        return -1;
    }
    for (size_t i = 0; i < spreader_detector->people_size; ++i) {
        if (spreader_detector->people[i]->id == id){
            return spreader_detector->people[i]->infection_rate;
        }
    }
    return -1;
}



/**
 * This function runs the algorithm which calculates the infection rates of the people.
 * When this algorithm ends, the user should be able to use the function
 * SpreaderDetectorGetInfectionRateById and get the infection rate of each person.
 * @param spreader_detector a spreader_detector.
 * @assumption you can not assume anything.
 */
void SpreaderDetectorCalculateInfectionChances(SpreaderDetector *spreader_detector){
    for (size_t i = 0; i < spreader_detector->people_size; ++i) {
        if (spreader_detector->people[i]->is_sick){
            UpdateRate(spreader_detector->people[i]);
            return;
        }
    }
}

/**
 * This function goes recursively over the meetings of each person (start from
 * the sick one) and update their infection rate
 * @param root
 */
void UpdateRate(Person* root){
    if (!root || !root->meetings){
        return;
    }
    for (size_t i = 0; i < root->num_of_meetings; ++i) {
        CalcCrna(root, root->meetings[i]->person_2, root->meetings[i]->measure, root->meetings[i]->distance);
        UpdateRate(root->meetings[i]->person_2);
    }
}

/**
 * This function calculates the Crna of person2 according to the formula in the exercise.
 * @param person1 the person with the updated infection rate
 * @param person2 the person we need to update his infection rate
 * @param measure the measure of their meeting
 * @param distance the distance in their meeting
 */
void CalcCrna(Person* person1, Person* person2, double measure, double distance){
    // todo - meeting distance == 0??
    person2->infection_rate = person1->infection_rate * ((measure*MIN_DISTANCE)/(distance*MAX_MEASURE));
    if (person2->age > AGE_THRESHOLD){
        person2->infection_rate += INFECTION_RATE_ADDITION_DUE_TO_AGE;
    }
    if (person2->infection_rate > 1){
        person2->infection_rate = 1;
    }
}

/**
 * Gets the recommendation for treatment for all people based on the parameters above,
 * and prints it to the given file path.
 * @param spreader_detector the spreader detector contains the person.
 * @param file_path the path to the output file.
 * @return returns 1 if printed successfully, 0 otherwise.
 * @if_fails return 0.
 * @assumption you can assume that the path to the file is ok (and anything but that).
 */
int SpreaderDetectorPrintRecommendTreatmentToAll(SpreaderDetector *spreader_detector, const char *file_path){
    FILE *file = fopen(file_path, "w"); // todo - free
    if (!file){
        return 0;
    }
    for (size_t i = 0; i < spreader_detector->people_size; ++i) {
        if (spreader_detector->people[i]->infection_rate > MEDICAL_SUPERVISION_THRESHOLD){
            fprintf(file, MEDICAL_SUPERVISION_THRESHOLD_MSG, spreader_detector->people[i]->name,
                    spreader_detector->people[i]->id, spreader_detector->people[i]->age, spreader_detector->people[i]->infection_rate);
        }
        else if (spreader_detector->people[i]->infection_rate > REGULAR_QUARANTINE_THRESHOLD){
            fprintf(file, REGULAR_QUARANTINE_MSG, spreader_detector->people[i]->name,
                    spreader_detector->people[i]->id, spreader_detector->people[i]->age, spreader_detector->people[i]->infection_rate);
        }
        else {
            fprintf(file, CLEAN_MSG, spreader_detector->people[i]->name,
                    spreader_detector->people[i]->id, spreader_detector->people[i]->age, spreader_detector->people[i]->infection_rate);
        }
    }
    fclose(file);
    return 1;
}


size_t SpreaderDetectorGetNumOfPeople(SpreaderDetector *spreader_detector){
    if (!spreader_detector){
        return 0;
    }
    return spreader_detector->people_size;

}


size_t SpreaderDetectorGetNumOfMeetings(SpreaderDetector *spreader_detector){
    if (!spreader_detector){
        return 0;
    }
    return spreader_detector->meeting_size;
}
