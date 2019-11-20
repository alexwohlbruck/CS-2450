#include <stdio.h>

double getPositiveAverage(double myArray[], int numItems) {
    if (numItems == 0) {
        return 0;
    }
    else {
        double sum = 0;
        int total = 0;
        
        for (int i = 0; i < numItems; i++) {
            if (myArray[i] >= 0) {
                sum += myArray[i];
                total++;
            }
        }
        
        if (total > 0) {
            return sum / total;
        } else {
            return 0;
        }
    }
}

int countRangeValues(double myArray[], int numItems, double value) {
    int count = 0;
    double min = value - 0.5;
    double max = value + 0.5;
    
    for (int i = 0; i < numItems; i++) {
        if (myArray[i] >= min && myArray[i] < max) {
            count++;
        }
    }
    
    return count;
}

double getMaxAbsolute(double myArray[], int numItems) {
    double max = 0;
    int maxIndex = 0;
    
    for (int i = 0; i < numItems; i++) {
        double absVal = myArray[i] < 0 ? (myArray[i] * -1) : myArray[i];
        
        if (absVal == max) {
            // If + and - have the same magnitude, give the positive val
            maxIndex = myArray[i] > 0 ? i : maxIndex;
        }
        
        if (absVal > max) {
            max = absVal;
            maxIndex = i;
        }
    }
    
    return myArray[maxIndex];
}

int countInverses(int myArray[], int numItems) {
    int counter = 0;
    int usedIndicies[numItems];
    int appendIndex = 0;
    
    // Fill used indicies array with non-interfering values
    for (int i = 0; i < numItems; i++) {
        usedIndicies[i] = -32768;
    }
    
    for (int i = 0; i < numItems - 1; i++) {
        // Iterate each item in array, skip used indicies
            
        // If index is the last number in the array, break the loop
        if (i == numItems - 1) {
            i++;
        }
        
        // If current number is used, skip
        int current = i;
        int leftNumberHasBeenUsed = 0; // Can't use a boolean for some stupid fucking reason so it's an integer
        for (int j = 0; j < numItems; j++) {
            if (usedIndicies[j] == current) {
                leftNumberHasBeenUsed = 1;
            }
        }
        if (leftNumberHasBeenUsed == 0) {
            for (int j = i + 1; j < numItems; j++) {
                // Iterate each item after current item (i), skip used indicies
                // If current number is used, skip
                int current = j;
                int rightNumberHasBeenUsed = 0;
                for (int k = 0; k < numItems; k++) {
                    if (usedIndicies[k] == current) {
                        rightNumberHasBeenUsed = 1;
                    }
                }
                
                if (myArray[i] + myArray[j] == 0 && rightNumberHasBeenUsed == 0) {
                    // Pair found
                    counter++;
                    
                    // Add used indicies of used pair to usedIndicies list
                    usedIndicies[appendIndex] = i;
                    appendIndex++;
                    usedIndicies[appendIndex] = j;
                    appendIndex++;
                    
                    j += numItems; // Swanson banned me from using a fucking break statement so here's some bullshit to break the loop
                }
            }
        }
    }
    
    return counter;
}

int getMaxCount(double myArray[], int numItems) {
    double maxAbsolute = getMaxAbsolute(myArray, numItems);
    return countRangeValues(myArray, numItems, maxAbsolute);
}
