//
// Created by musta on 20.12.2022.
//

#ifndef PROJECT_2_GRAPH_H
#define PROJECT_2_GRAPH_H
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <iostream>
#include "Target.h"
#include "Airport.h"
#include "Airline.h"

using namespace std;

class Graph {

private:

    /// @brief Map with an airport as key and vector with all possible flights from it
    unordered_map <string, vector <Target> > g;
    // ["asd"] = {"qwe", "sdf"}   g["any airport"].size() = количество вылетов из аэропорт

    /// @brief Map with airport code as key and its Airport object
    unordered_map <string, Airport> airports;
    // g["any airport"]  for => set (airline)

    /// @brief Map with city name as key and the code of all its airports in an unordered_set
    unordered_map <string, unordered_set<string> > cities;

    /// @brief Map with country name as key and the name of all its cities in an unordered_set
    unordered_map <string, unordered_set<string> > countries;

    /// @brief Map with airline code as key and it Airline object
    unordered_map<string, Airline> airlines;

public:
    ///@brief Empty Constructor
    Graph();

    /**
     * @brief Checks if city is in city list
     * @param name -> Name of the city
     * @return Bool value
     */
    bool isCity(const string& name);

    /**
     * @brief Checks if country is in country list
     * @param name -> Name of the Country
     * @return Bool value
     */
    bool isCountry(const string& name);

    /**
     * @brief Gets path with lowest flight number between cities
     * @param from -> Name of origin city
     * @param to -> Name of destination city
     * @return Vector with airports names as strings
     */
    vector <string> getPathCities(const string& from, const string& to);

    /**
     * @brief Gets path with lowest flight number between countries
     * @param from -> Name of origin country
     * @param to -> Name of destination country
     * @return Vector with airports names as strings
     */
    vector <string> getPathCountries(const string& from, const string& to);

    /**
     * @brief Gets path with lowest flight number between coordinates
     * @param from -> Origin coordinate
     * @param to -> Destination coordinate
     * @return Vector with airports names as strings
     */
    vector <string> getPathByPoint(double lat, double lon, double dist);  // FALTA AQUIIIIIIIIIIIIIIIIIIII

    /**
     * @brief Gets path with lowest flight number between airports
     * @param from -> Origin airport code
     * @param to -> Destination airport code
     * @return Vector with airports names as strings
     */
    vector <string> getPathAirports(const string& from, const string& to);

    /**
     * @brief Gets path with lowest flight number between places
     * @param from -> Origin reference(can be name, airport code, coordinate)
     * @param to -> Destination reference(can be name, airport code, coordinate)
     * @return Vector with airports names as strings
     */
    vector <string> getUltimatePath(string from, string to);

    /**
     * @brief Gets path with lowest flight number between airports
     * @param from -> Vector with origin airports
     * @param to -> Vector with destination airports
     * @return Vector with airports names as strings
     */
    vector <string> getPathByVectors(vector <string> from, vector <string> to);

    /**
     * @brief Gets all airports you can go to with n flight number
     * @param from -> Origin airport code
     * @param num -> number of flights
     * @return Vector with airport names as strings
     */
    vector <string> targetAirports(const string& from, int num);   // todos aeroportos que podemos ir de um aeroporto para outro por n passos

    /// @brief Gets map with an airport as key and vector with all possible flights
    unordered_map <string, vector <Target> > getG();

    /// @brief Gets airport list
    unordered_map <string, Airport> getAirports();

    /// @brief Gets airline list
    unordered_map<string, Airline> getAirlines();

    /// @brief Gets city list
    unordered_map <string, unordered_set<string> > getCities();

    /// @brief Gets country list
    unordered_map <string, unordered_set<string> > getCountries();

    /**
     * @brief Gets number of flights in an airport
     * @param Airport -> Airport code
     * @return Number of flights in the Airport
     */
    unsigned getNumberOfFlights(const string& Airport);      // info about Airport number of flights

    /**
     * @brief Gets all airlines operating in given airport
     * @param Airport -> Airport code
     * @return Set of airlines as strings
     */
    unordered_set<string> getAirlinesFromAirport(const string& Airport);    // info Airport all companies
};

#endif //PROJECT_2_GRAPH_H
