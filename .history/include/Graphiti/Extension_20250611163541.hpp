#pragma once

#include <cstdio>
#include <iostream>
#include <chrono>

#include "API.hpp"

#include <Graphiti/Connection/Connection_VCP.hpp>

/**
 * @brief Class to improve the ease of use of the Graphiti
 * Graphiti_API can be used with or without GraphitExtension
 * GraphitiExtension can also be used as a reference to how
 * the Graphiti_API can be used
 */
class GraphitiExtension : public Graphiti_API {
public:
    /**
     * @brief Construct a new Graphiti Extension object
     * 
     */
    GraphitiExtension();

    /**
     * @brief Construct a new Graphiti Extension object
     * 
     * @param connection Connection to device
     */
    GraphitiExtension(GraphitiConnection* connection);   

    
    /**
     * @brief Destroy the Graphiti Extension:: Graphiti Extension object
     * 
     */
    ~GraphitiExtension();

    /**
     * @brief Starts up VCP connection and turns on events
     * 
     * @param port port name
     * @param keyEventsBool boolean for key events
     * @return true 
     * @return false 
     */
    bool startUpVCP(std::string port, bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Shuts down VCP connection and turns of events
     * 
     * @param port port name
     * @param keyEventsBool boolean for key events
     */
    void shutDownVCP(bool keyEventsBool, bool touchEventsBool);

    /**
     * @brief Screen indicies of pin heights
     * 
     */
    std::vector<uint8_t> screen;

    /**
     * @brief Key Loop for concurrent key responses
     * 
     * Can be used or be used as a reference
     * 
     * Note: Key cobinaitons that are reserved are: (5 + 6 + 8), (8 + Down), (7 + 8)
     * 
     * @param key_func function to use keys
     * @param loop_condition condition for looping
     * @param parameters pointer to other variables used by function
     */
    void keyLoop(
        std::function<void(const std::set<std::string>&, void*)> key_func, 
        bool *loop_condition,
        void* parameters = nullptr
    );

    /**
     * @brief Draw Loop for concurrent draw events
     * 
     * @param draw_func function using draw input
     * @param loop_condition condition for looping
     * @param parameters pointer to other variables used by function
     */
    void drawLoop(
        std::function<void(const Graphiti_API::DrawEvent&, void*)> draw_func,
        bool *loop_condition,
        void* parameters = nullptr
    );

    /**
     * @brief Set the Pin on screen
     * 
     * @param row row
     * @param col column
     * @param height pin heights 0-4
     */
    void setPin(int row, int col, int height);

    /**
     * @brief Gets index of pin on Graphiti
     * 
     * @param row row
     * @param col column
     * @return int index on Graphiti screen
     */
    int index(int row, int col);

    /**
     * @brief Clears Screen display data in GraphitiExtension
     * 
     */
    void clearScreen();

    /**
     * @brief Shorted sleep call
     * Can copy it and make it locally so you can call sleep(2);
     * rather than "graphitiExtension->sleep(2);" for simlpicity
     * 
     * @param time time seconds
     */
    void sleep(int time);

private:
    
    GraphitiConnectionVCP* vcpConn = nullptr;
};