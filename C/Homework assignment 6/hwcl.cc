#include <stdio.h>

void printRectangleWhile(int col, int row) {
    if(row == 1 && col == 1)
    {
        printf("+\n");
    }
    else if(row == 1 && col > 0)
    {
        while(col > 0)
        {
            printf("+");
            col--;
        }
        printf("\n");
    }
    else if(row > 0 && col == 1)
    {
        while(row > 0)
        {
            printf("+\n");
            row--;
        }
    }   
    else if(row > 0 && col > 0)
    {
        row -= 2;
        int temp = col;
        while(col > 0)
        {
            printf("%c",'+');
            col--;
        }
        col = temp;
        printf("\n");
        while(row > 0)
        {
            col -= 2;
            printf("%c",'+');
            while(col > 0)
            {
                printf("%c", '-');
                col--;
            } 
            printf("%c",'+');
            printf("\n");
            col = temp;
            row--;
        }
        while(col > 0)
                {               
                        printf("%c",'+');
                        col--;
                }
                col = temp;
        printf("\n");
    }
}

void printTriangleFor(int height) {
    if (height < 1) {
        return;
    }

    if (height == 1) {
        printf("+\n");
        return;
    }
    
    // Print space and single plus for first level
    for (int i = 0; i < height - 1; i++) {
        printf(" ");
    }
    printf("+\n");
    
    // Print mid section
    for (int level = 2; level < height; level++) {
        for (int i = 0; i < height - level; i++) {
            printf(" ");
        }
        
        printf("+");
        
        for (int i = 0; i < level - 2; i++) {
            printf("-");
        }
        
        printf("+");
        printf("\n");
    }
    
    // Print last row
    for (int i = 0; i < height; i++) {
        printf("+");
    }
    printf("\n");
}