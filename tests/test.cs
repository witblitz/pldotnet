
using PLDotNet;

namespace PLMonoTest {

	public class Test {
		public static int test() 
		{
			return 10;
		}
		
		public static int test2() 
		{
			return 10;
		}

		public static int test_param_1(int p1) 
		{
			return p1;
		}

		public static string test_string_param(string p1) 
		{
			return p1;
		}

		public static int test_2_params(int p1, int p2)
		{
			pldotnet.raise(LOG_LEVEL_CODES.NOTICE, "HEY test_2_params HIER MET 2 PARAMS: {0} en {1}", p1, p2);
			return p1 * p2;
		}

		public static string multiply_string(string p1, int c) 
		{
			string ret = "";

			for (int i = 0; i < c; i++)
				ret = ret + (ret == "" ? "" : "\n") + p1;
			return ret;
		}
	}
}

