#include "util.h"

int main(int argc, char **argv)
{
    FILE *fs = fopen("../src/T.csv", "r"),
         *gs = fopen("../src/U.csv", "r");
    if (!fs || !gs)
        return -1;

    rewind(fs);
    rewind(gs);

    double input[m][n], output[m][n];

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(fs, "%lf,", &input[i][j]);
            fscanf(gs, "%lf,", &output[i][j]);
        }
    }
    
    fclose(fs);
    fclose(gs);

    fs = fopen("../src/_T.csv", "r");
    gs = fopen("../src/_U.csv", "r");
    if (!fs || !gs)
        return -1;

    rewind(fs);
    rewind(gs);
    
    double test_input[MAX_TST][n], test_output[MAX_TST][n];

    for (int i = 0; i < MAX_TST; i++)
    {
        for (int j = 0; j < n; j++)
        {
            fscanf(fs, "%lf,", &test_input[i][j]);
            fscanf(gs, "%lf,", &test_output[i][j]);
        }
    }

    printf("%.3f\t%E\n", test_input[1][0], test_output[1][0]);
    
    fclose(fs);
    fclose(gs);

    double result[MAX_TST][n];
    
    int debug = (argc > 1)? atoi(argv[1]) : 0, p, q; 
    if (debug)
    {
        p = atoi(argv[2]);
        q = atoi(argv[3]);
    }

    // Idea: Sugeno algorithm in fuzzy logic    
    for (int k = 0; k < MAX_TST; k++)
    {
        for (int j = 0; j < n; j++)
        {
            double value = test_input[k][j];
            double MY = 0.0, MX = 0.0;
            
            for (int i = 0; i < m-1; i++)
            {
                double b_coef = getBeliefCoef(value, input[i][j], input[m-1][j]);
                double approximatedValue = approximate(value, input[i][j], input[m-1][j], output[i][j], output[m-1][j]);
                MX += b_coef;
                MY += b_coef * approximatedValue;

                if (debug && k == p && j == q)
                {
                    printf("Step %d: In(%10.3f,%10.3f)\tOut(%E, %E)\n", i, input[i][j], input[m-1][j], output[i][j], output[m-1][j]);
                    printf("        x=%.3f\ty=%E\tb=%.4f\n\n", value, approximatedValue, b_coef);
                }
            }

            result[k][j] = MY / MX;
        }
    }

    if (debug)
        printf("Result: %E\tNeed: %E\n", result[p][q], test_output[p][q]);
    
    fs = fopen("../src/0310.csv", "w");
    for (int i = 0; i < MAX_TST; i++)
    {
        for (int j = 0; j < n - 1; j++)
            fprintf(fs, "%E,", result[i][j]);
        fprintf(fs, "%E\n", result[i][n-1]);

        for (int j = 0; j < n - 1; j++)
            fprintf(fs, "%E,", test_output[i][j]);
        fprintf(fs, "%E\n\n", test_output[i][n-1]);
    }
    fclose(fs);

    double mse[MAX_TST], min, max;
    for (int i = 0; i < MAX_TST; i++)
    {
        mse[i] = MSE(result[i], test_output[i]);
        if (i == 0)
        {
            min = max = mse[i];
        }
        else
        {
            if (mse[i] < min)
                min = mse[i];
            if (mse[i] > max)
                max = mse[i];
        }
    }

    if (debug)
        printf("pMSE: %E\n", mse[p]);

    printf("MSE: best=%E, worth=%E", min, max);

    return 0;
}