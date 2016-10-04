
using System;
using Mono.CSharp;
using Mono.CSharp.Switch;
using System.Runtime.InteropServices;

namespace PLDotNet
{
	public class code_executor
	{
		public static void hello()
		{
			pldotnet.raise(PLDotNet.LOG_LEVEL_CODES.NOTICE, "haaaaaai");
		}

		public static object run(string code)
		{
			pldotnet.raise(LOG_LEVEL_CODES.NOTICE, "running codess {0}", code);

			object ret = null;

			try {
				CompilerSettings compsettings = null; //new CompilerSettings();
				ConsoleReportPrinter crpp = new ConsoleReportPrinter();

				CompilerContext ctx = new CompilerContext(compsettings, crpp);
			} catch (Exception e) { pldotnet.raise(LOG_LEVEL_CODES.ERROR, "exception {0}", e); }
			//Evaluator evaluator = new Evaluator(ctx);
			//ret = evaluator.Run(code);
			return ret;
		}

	}


	//This values comes from postgresql headers
	public enum LOG_LEVEL_CODES
	{
		DEBUG5 = 10, 	/* Debugging messages, in categories of decreasing detail. */
		DEBUG4 = 11,
		DEBUG3 = 12,
		DEBUG2 = 13,
		DEBUG1 = 14, 	/* used by GUC debug_* variables */
		LOG = 15,    	/* Server operational messages */
		COMMERROR = 16, /* Client communication problems; same as LOG for server reporting, but never sent to client. */
		INFO = 17,      /* Messages specifically requested by user */
		NOTICE = 18,    /* Helpful messages to users about query */
		WARNING = 19,   /* Warnings.  NOTICE is for expected messages */
		ERROR = 20,	/* user error - abort transaction; return to */
		FATAL = 21,	/* fatal error - abort process */
		PANIC = 22 	/* take down the other backends with me */
	}

	public class pldotnet
	{
		public static void raise (LOG_LEVEL_CODES level, string format, params object[] args)
		{
			string message = string.Format(format, args);
			_internal_elog((int)level, message);
		}

		[DllImport ("__Internal", EntryPoint="pldotnet_internal_elog")]
		static extern void _internal_elog(int level, string message);
	}
}

