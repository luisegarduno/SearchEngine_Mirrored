#include "flightplanner.h"

FlightPlanner::FlightPlanner(){         // Default Constructor
}

// The names of the of the command line arguments are used to create &
// instantiate private DSStrings that are the file names
void FlightPlanner::setFileNames(char * argv[]){
    flightDataFile = argv[1];
    pathsToCalculateFile = argv[2];
    flightEfficiencyFile = argv[3];
}


void FlightPlanner::addFlightsData(){
    streamFlightData.open(flightDataFile.c_str());

    if(!streamFlightData.is_open()){
        cout << "Flight Data File is not open. Please check command line input" << endl;
        exit(-1);
    }

    int numberOfFlights;
    double theCost = 0.0,theTime = 0.0;
    DSString theOrigin(""),theDestination(""),theAirline("");
    streamFlightData >> numberOfFlights;

    char temp[100];
    streamFlightData.getline(temp,100);

    char * buffer = new char[100];

    cout << "-----------------FLIGHT FILE LIST----------------------" << endl;
    for(int i = 0; i < numberOfFlights; i++){
        for(int j = 0; j < 5; j++){
            if(j == 0){
                streamFlightData.getline(buffer,100,'|');
                theOrigin = buffer;
                cout << "Origin: " << theOrigin << endl;
            }
            if(j == 1){
                streamFlightData.getline(buffer,100,'|');
                theDestination = buffer;
                cout << "Destination: " << theDestination << endl;
            }
            if(j == 2){
                streamFlightData >> theCost;
                streamFlightData.getline(buffer,100,'|');
                cout << "Cost: " << theCost << endl;
            }
            if(j == 3){
                streamFlightData >> theTime;
                streamFlightData.getline(buffer,100,'|');
                cout << "Time: " << theTime << endl;
            }
            if(j == 4){
                streamFlightData.getline(buffer,100);
                theAirline = buffer;
                cout << "Airline: " << theAirline << endl;
            }
        }
        FlightData newFlight(theOrigin,theDestination,theCost,theTime,theAirline);
        FlightData reversedFlight(theDestination,theOrigin,theCost,theTime,theAirline);
        flightPaths.add(newFlight);
        flightPaths.add(reversedFlight);
        cout << endl;
    }

    streamFlightData.close();
    delete [] buffer;
}

void FlightPlanner::requestedRoutes(){
    streamPathsToCalculate.open(pathsToCalculateFile.c_str());

    if(!streamPathsToCalculate.is_open()){
        cout << "Paths To Calculate File is not open. Please check command line input" << endl;
        exit(-1);
    }

    int numberOfRequested;
    streamPathsToCalculate >> numberOfRequested;

    char temp[100];
    streamPathsToCalculate.getline(temp,100);

    DSString theOrigin(""),theDestination(""),sortType("");
    char * buffer = new char[100];

    for(int i = 0; i < numberOfRequested; i++){
        for(int j = 0; j < 3; j++){
            if(j == 0){
                streamPathsToCalculate.getline(buffer,100,'|');
                theOrigin = buffer;
                cout << "Origin: " << theOrigin << endl;
            }

            if(j == 1){
                streamPathsToCalculate.getline(buffer,100,'|');
                theDestination = buffer;
                cout << "Destination: " << theDestination << endl;
            }

            if(j == 2){
                streamPathsToCalculate.getline(buffer,100);
                sortType = buffer;
                cout << "Sort Flights by: " << sortType << endl;
            }
        }
        RequestRoute newRoute(theOrigin,theDestination,sortType);
        DSVector<FlightPlanner::iterator> findAllRoutes = findRoutes(newRoute);
        DSVector<Route> allFlightRoutes = getRouteFromStack(findAllRoutes);
        cout << endl;

    }

    streamPathsToCalculate.close();
    delete [] buffer;
}



// REMEMBER LIFO, LAST IN, FIRST OUT ---> SO the 'TOP' will be the LAST IN,
// Thus we make the last item appended be declared as the head
// USING nodePtr = DSNode<FlightData>*;             USING customStackIterator = DSStack<nodePtr>;
DSVector<FlightPlanner::iterator> FlightPlanner::findRoutes(RequestRoute requestedRoute){ // TO SEE HIGH LEVEL STEPS FOR ITERATIVE BACKTRACKING
                                                                // GO TO LINE
    iterator currentIterator;
    DSStack< DSLinkedList<FlightData> > routeOnStack; // create a stack containing routes
    DSVector<iterator> currentStackInVector;


    routeOnStack.push(flightPaths.getAllOrigins(requestedRoute.getRequestedOrigin()));
    nodePtr currentNodeOnStack =  routeOnStack.topValue().head;


    do{
        if(currentNodeOnStack == nullptr){
            bool allRoutesFound = false;

            while(currentNodeOnStack == nullptr){
                routeOnStack.pop();

                if(routeOnStack.isEmpty()){
                    allRoutesFound = true;
                    break;
                }

                currentNodeOnStack = currentIterator.pop();
                currentNodeOnStack = currentNodeOnStack->next;
                while(currentNodeOnStack != nullptr && checkStack(currentNodeOnStack->data,currentIterator)){
                    currentNodeOnStack = currentNodeOnStack->next;
                }
            }

            if(allRoutesFound){
                break;
            }
        }

        // **********************TOP IS DESTINATION***********************************
        // If Node located on stack contains getDestination that's the same as the requested Destination
        if(currentNodeOnStack->data.getDestination() == requestedRoute.getRequestedDestination()){
            currentIterator.push(currentNodeOnStack);
            currentStackInVector.pushBack(currentIterator);

            currentIterator.pop();
            currentNodeOnStack = currentNodeOnStack->next;
            while(currentNodeOnStack != nullptr && checkStack(currentNodeOnStack->data,currentIterator)){
                currentNodeOnStack = currentNodeOnStack->next;
            }
        }
        // **********************************************************************

        // ********************Adding LinkedList to DSStack *********************//
        if(currentNodeOnStack != nullptr){
            currentIterator.push(currentNodeOnStack);

            routeOnStack.push(flightPaths.getAllOrigins(currentNodeOnStack->data.getDestination()));
            currentNodeOnStack = routeOnStack.topValue().head;

            while(currentNodeOnStack != nullptr && checkStack(currentNodeOnStack->data,currentIterator)){
                currentNodeOnStack = currentNodeOnStack->next;
            }
        }
        // *********************************************************************//


    }while(true);
    cout << "leaving" << endl;
    return currentStackInVector;
}

bool FlightPlanner::checkStack(FlightData& thisData,iterator currentStack){
    nodePtr theTop = nullptr;

    while(!currentStack.isEmpty()){
        theTop = currentStack.pop();

        if(thisData.getDestination() == theTop->getData().getDestination() || thisData.getDestination() == theTop->getData().getOrigin()){
                return true;
        }
    }
    return false;
}

DSVector<Route> FlightPlanner::getRouteFromStack(DSVector<iterator> routeOnStackVector){
    DSStack<FlightData> routeOnStack;
    DSVector<Route> routeOnVector;

    for(int counter = 0; counter < routeOnStackVector.getSize(); counter++){
        while(!routeOnStackVector.isEmpty()){
            DSNode<FlightData>* currentFlightPtr = routeOnStackVector.elementIndex(counter).pop();
            routeOnStack.push(currentFlightPtr->data);
        }

        Route newRoute = Route();
        newRoute.addRoute(routeOnStack);
        newRoute.print();


        routeOnVector.pushBack(newRoute);
    }

    return routeOnVector;
}


/*
1. Push source to DSStack

2. while(stackIsNotEmpty)

    3. if(top == destination)
            |
            --> if TRUE: Store path & pop
            |
            --> if FALSE: step thru iterators of top:
                     |
                     --> if iterator == nullptr
                     |          |
                     |          --> if TRUE: pop top & reset Iterator
                     |          |
                     |          --> if FALSE: continue
                     |
                     |
                     --> if city on stack
                                |
                                --> if TRUE: continue
                                |
                                --> if FALSE: push city, Go to step #3
*/
















