using System;

public class Fact
{
    private const int FACT_MAX_INPUT = 180;

    public static double fact(int number) 
    {
        if (number < 0) {
            return Double.NaN;
        }
        if (number > FACT_MAX_INPUT) {
            return Double.PositiveInfinity;
        }
        double res = 1.0;
        for (int i = 2; i < number; ++i) {
            res *= number;
        }
        return res;
    }

    public static int Main(string[] args)
    {
        try {
            if (args.Length != 2) {
                throw new System.InvalidOperationException("Usage: fact <number>");
            }
            int number = Int32.Parse(args[1]);
            Console.WriteLine(fact(number));
        } catch (Exception ex) {
            Console.Error.WriteLine(ex.Message);
            return 1;
        }
        return 0;
    }
}
