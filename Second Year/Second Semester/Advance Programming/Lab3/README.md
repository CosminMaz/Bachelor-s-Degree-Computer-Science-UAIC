Homeworks algorithm: for every flight it choses the first runways that is available for it's landing interval  

Bonus algorithm: Assigns the flight to a runway, cycling through runways "round-robin" style:  
 First flight → Runway 1  
 Second flight → Runway 2  
 Third flight → Runway 3 (if available)  
 Fourth flight → Runway 1 (back to the start)  
 Checks if scheduling is equitable: counts how many flights were assigned to each runway.
 Determine if additional runways are needed, If the difference between the most-loaded and least-loaded runway is greater than 1, then:  
-Equitable scheduling is not possible with the current runways.  
-The program calculates the minimum additional runways needed.
